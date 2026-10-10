#include "mainwindow(2).h"
#include "ui_mainwindow(2).h"
#include "BackEnd.cpp"
#include <QCoreApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QThread>
#include <QMetaObject>
#include <QString>
#include <QDebug>
#include <QElapsedTimer>
#include <QLineEdit>
#include <QVariant>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <QGeoCoordinate>
#include <QDir>
#include <QLibraryInfo>

#include <QCompleter>
#include <QStringList>
#include <QSet>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "Qt plugins path:"
             << QLibraryInfo::path(QLibraryInfo::PluginsPath);

    qDebug() << "Qt library paths:"
             << QCoreApplication::libraryPaths();

    qDebug() << "Available GPS sources:"
             << QGeoPositionInfoSource::availableSources();


    // Nguon dinh vi mac dinh cua Windows/Qt. Khong dung COM7.
    positionSource = QGeoPositionInfoSource::createDefaultSource(this);
    if (!positionSource)
    {
        qDebug() << "GPS source = NULL";
    }
    else
    {
        qDebug() << "GPS source created:"
                 << positionSource;
    }
    if (positionSource)
    {
        connect(positionSource, &QGeoPositionInfoSource::positionUpdated,
                this, &MainWindow::handlePositionUpdate);

        connect(positionSource, &QGeoPositionInfoSource::errorOccurred,
                this, &MainWindow::handlePositionError);
    }

    // Khi nguoi dung go lai diem bat dau, bo node GPS cu.
    connect(ui->lineEdit, &QLineEdit::textEdited, this, [this]()
            {
                ui->lineEdit->setProperty("gpsNode", QVariant());
            });

    // =====================================================
    // LOAD DỮ LIỆU BẢN ĐỒ
    // =====================================================

    loadData(
        graphNodes,
        graph
        );
    if (graphNodes.empty() || graph.empty()) {
        QMessageBox::critical(
            this,
            "Lỗi dữ liệu",
            "Không tải được bản đồ.\n"
            "Hãy kiểm tra file Data/Saigon.osm."
            );
        return;
    }
    //Hàm để gợi ý danh sách cho điểm bắt đầu và kết thúc khi nhập
    QStringList locationList;
    QSet<QString> uniqueLocations;
    for (const Node &node : graphNodes) {
        QString name = QString::fromStdString(node.name).trimmed();
        if (name.isEmpty()) continue;
        if (!uniqueLocations.contains(name)) {
            uniqueLocations.insert(name);
            locationList << name;
        }
    }
    locationList.sort(Qt::CaseInsensitive);
    QCompleter *startCompleter = new QCompleter(locationList, this);
    startCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    startCompleter->setFilterMode(Qt::MatchContains);
    startCompleter->setCompletionMode(QCompleter::PopupCompletion);
    ui->lineEdit->setCompleter(startCompleter);

    QCompleter *endCompleter = new QCompleter(locationList, this);
    endCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    endCompleter->setFilterMode(Qt::MatchContains);
    endCompleter->setCompletionMode(QCompleter::PopupCompletion);
    ui->lineEdit_2->setCompleter(endCompleter);

    ui->textEdit->setText(
        "Đã load dữ liệu bản đồ.\n"
        "Sẵn sàng tìm đường."
        );

    ui->listWidget->clear();

    ui->listWidget->addItem(
        "HỆ THỐNG SẴN SÀNG"
        );

    ui->listWidget->addItem(
        "Đã tải dữ liệu bản đồ."
        );

    ui->listWidget->addItem(
        "Vui lòng nhập điểm đi và điểm đến."
        );
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    if (!positionSource)
    {
        ui->textEdit->setText(
            "KHÔNG LẤY ĐƯỢC VỊ TRÍ\n\n"
            "Máy tính hiện không cung cấp dịch vụ định vị.\n"
            "Hãy bật Location trong Windows rồi thử lại."
            );
        return;
    }

    ui->pushButton_2->setEnabled(false);
    ui->pushButton_2->setText("Đang lấy vị trí...");
    ui->textEdit->setText(
        "ĐANG XÁC ĐỊNH VỊ TRÍ HIỆN TẠI...\n\n"
        "Vui lòng chờ vài giây."
        );

    ui->lineEdit->setProperty("gpsNode", QVariant());

    // Yêu cầu một lần cập nhật, timeout 10 giây.
    positionSource->requestUpdate(10000);
}

void MainWindow::on_pushButton_3_clicked()
{
    // Nút tìm đường cho xe máy
    on_pushButton_clicked();
}

void MainWindow::on_pushButton_4_clicked()
{
    // Nút tìm đường cho ô tô
    on_pushButton_clicked();
}

void MainWindow::handlePositionUpdate(const QGeoPositionInfo &info)
{
    const QGeoCoordinate coordinate = info.coordinate();

    if (!coordinate.isValid())
    {
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_2->setText("GPS");
        ui->textEdit->setText("Không lấy được tọa độ GPS.");
        return;
    }

    const double lat = coordinate.latitude();
    const double lon = coordinate.longitude();

    // Dùng hàm có sẵn của backend để tìm node OSM gần vị trí hiện tại.
    const int nearestNode =
        closestNode(graphNodes, latIndex, lat, lon);

    if (nearestNode < 0 ||
        nearestNode >= static_cast<int>(graphNodes.size()))
    {
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_2->setText("GPS");
        ui->textEdit->setText(
            "Đã lấy được tọa độ nhưng không tìm thấy node OSM gần đó."
            );
        return;
    }

    const QString nearestName =
        QString::fromStdString(graphNodes[nearestNode].name).trimmed();

    const QString startText = nearestName.isEmpty()
                                  ? "Vị trí hiện tại"
                                  : "Vị trí hiện tại - " + nearestName;

    // Lưu node GPS để on_pushButton_clicked() dùng trực tiếp.
    ui->lineEdit->setProperty("gpsNode", nearestNode);

    // Hiện vị trí ngay trong ô điểm bắt đầu.
    ui->lineEdit->setText(startText);

    ui->textEdit->setText(
        "ĐÃ XÁC ĐỊNH VỊ TRÍ HIỆN TẠI\n\n"
        "Điểm bắt đầu:\n" + startText +
        "\n\nTọa độ:\n" +
        QString::number(lat, 'f', 6) + ", " +
        QString::number(lon, 'f', 6) +
        "\n\nBấm Tìm đường đi tối ưu để bắt đầu."
        );

    ui->listWidget->clear();
    ui->listWidget->addItem("GPS ĐÃ SẴN SÀNG");
    ui->listWidget->addItem("Điểm bắt đầu: " + startText);
    ui->listWidget->addItem(
        "Tọa độ: " + QString::number(lat, 'f', 6) +
        ", " + QString::number(lon, 'f', 6)
        );

    ui->pushButton_2->setEnabled(true);
    ui->pushButton_2->setText("GPS");
}

void MainWindow::handlePositionError(QGeoPositionInfoSource::Error error)
{
    QString message;

    switch (error)
    {
    case QGeoPositionInfoSource::AccessError:
        message =
            "Windows không cho phép ứng dụng truy cập vị trí.\n"
            "Hãy bật quyền Location trong Windows.";
        break;

    case QGeoPositionInfoSource::UpdateTimeoutError:
        message =
            "Hết thời gian chờ vị trí.\n"
            "Hãy kiểm tra Location và Internet/Wi-Fi rồi thử lại.";
        break;

    case QGeoPositionInfoSource::ClosedError:
        message = "Dịch vụ định vị đã bị đóng.";
        break;

    case QGeoPositionInfoSource::UnknownSourceError:
        message = "Không tìm thấy nguồn định vị trên máy tính này.";
        break;

    default:
        message = "Không xác định được vị trí hiện tại.";
        break;
    }

    ui->pushButton_2->setEnabled(true);
    ui->pushButton_2->setText("GPS");
    ui->textEdit->setText(message);

    ui->listWidget->clear();
    ui->listWidget->addItem("GPS KHÔNG THÀNH CÔNG");
    ui->listWidget->addItem(message);
}

// =========================================================
// NÚT TÌM ĐƯỜNG
// =========================================================

void MainWindow::on_pushButton_clicked()
{

    // =====================================================
    // 1. LẤY TÊN ĐIỂM ĐI + ĐIỂM ĐẾN
    // =====================================================

    QString startName =
        ui->lineEdit->text().trimmed();

    QString endName =
        ui->lineEdit_2->text().trimmed();


    // =====================================================
    // 2. KIỂM TRA INPUT
    // =====================================================

    if (startName.isEmpty())
    {
        ui->textEdit->setText(
            "Vui lòng nhập điểm bắt đầu."
            );

        ui->listWidget->clear();

        ui->listWidget->addItem(
            "Chưa nhập điểm bắt đầu."
            );

        return;
    }


    if (endName.isEmpty())
    {
        ui->textEdit->setText(
            "Vui lòng nhập điểm đến."
            );

        ui->listWidget->clear();

        ui->listWidget->addItem(
            "Chưa nhập điểm đến."
            );

        return;
    }


    // =====================================================
    // 3. TÌM NODE CỦA ĐIỂM ĐI
    // =====================================================

    int startLocation = -1;


    // -----------------------------------------------------
    // Nếu điểm đi được lấy từ GPS
    // -----------------------------------------------------

    bool gpsOK = false;

    int gpsNode =
        ui->lineEdit->property("gpsNode").toInt(&gpsOK);


    if (gpsOK &&
        gpsNode >= 0 &&
        gpsNode < (int)graphNodes.size())
    {
        startLocation = gpsNode;
    }


    // -----------------------------------------------------
    // Nếu không có GPS thì tìm theo tên
    // -----------------------------------------------------

    if (startLocation == -1)
    {
        // Tìm chính xác trước
        for (int i = 0;
             i < (int)graphNodes.size();
             i++)
        {
            QString nodeName =
                QString::fromStdString(
                    graphNodes[i].name
                    );

            if (nodeName.compare(
                    startName,
                    Qt::CaseInsensitive) == 0)
            {
                startLocation = i;
                break;
            }
        }
    }


    // -----------------------------------------------------
    // Nếu không tìm chính xác được thì tìm gần đúng
    // -----------------------------------------------------

    if (startLocation == -1)
    {
        for (int i = 0;
             i < (int)graphNodes.size();
             i++)
        {
            QString nodeName =
                QString::fromStdString(
                    graphNodes[i].name
                    );

            if (nodeName.isEmpty())
                continue;


            if (nodeName.contains(
                    startName,
                    Qt::CaseInsensitive) ||
                startName.contains(
                    nodeName,
                    Qt::CaseInsensitive))
            {
                startLocation = i;
                break;
            }
        }
    }


    // =====================================================
    // 4. TÌM NODE CỦA ĐIỂM ĐẾN
    // =====================================================

    int endLocation = -1;


    // -----------------------------------------------------
    // Tìm chính xác
    // -----------------------------------------------------

    for (int i = 0;
         i < (int)graphNodes.size();
         i++)
    {
        QString nodeName =
            QString::fromStdString(
                graphNodes[i].name
                );

        if (nodeName.compare(
                endName,
                Qt::CaseInsensitive) == 0)
        {
            endLocation = i;
            break;
        }
    }


    // -----------------------------------------------------
    // Tìm gần đúng
    // -----------------------------------------------------

    if (endLocation == -1)
    {
        for (int i = 0;
             i < (int)graphNodes.size();
             i++)
        {
            QString nodeName =
                QString::fromStdString(
                    graphNodes[i].name
                    );

            if (nodeName.isEmpty())
                continue;


            if (nodeName.contains(
                    endName,
                    Qt::CaseInsensitive) ||
                endName.contains(
                    nodeName,
                    Qt::CaseInsensitive))
            {
                endLocation = i;
                break;
            }
        }
    }


    // =====================================================
    // 5. KHÔNG TÌM THẤY ĐIỂM ĐI
    // =====================================================

    if (startLocation == -1)
    {
        ui->textEdit->setText(
            "KHÔNG TÌM THẤY ĐIỂM ĐI\n\n"
            "Tên đã nhập:\n" +
            startName
            );

        ui->listWidget->clear();

        ui->listWidget->addItem(
            "Không tìm thấy điểm đi."
            );

        ui->listWidget->addItem(
            startName
            );

        return;
    }


    // =====================================================
    // 6. KHÔNG TÌM THẤY ĐIỂM ĐẾN
    // =====================================================

    if (endLocation == -1)
    {
        ui->textEdit->setText(
            "KHÔNG TÌM THẤY ĐIỂM ĐẾN\n\n"
            "Tên đã nhập:\n" +
            endName
            );

        ui->listWidget->clear();

        ui->listWidget->addItem(
            "Không tìm thấy điểm đến."
            );

        ui->listWidget->addItem(
            endName
            );

        return;
    }


    // =====================================================
    // 7. TÌM GRAPH NODE GẦN NHẤT CHO ĐIỂM ĐI
    //
    // Chỉ xét node có cạnh trong graph.
    // =====================================================

    auto findNearestRoadNode =
        [&](int locationNode) -> int
    {
        double lat =
            graphNodes[locationNode].lat;

        double lon =
            graphNodes[locationNode].lon;


        int nearest = -1;

        double minDistance = INF;


        for (int i = 0;
             i < (int)graphNodes.size();
             i++)
        {
            // Node không có cạnh thì không phải
            // node đường của graph
            if (graph[i].empty())
                continue;


            double distance =
                calculateDistance(
                    lat,
                    lon,
                    graphNodes[i].lat,
                    graphNodes[i].lon
                    );


            if (distance < minDistance)
            {
                minDistance = distance;
                nearest = i;
            }
        }


        return nearest;
    };


    // =====================================================
    // 8. ĐỔI ĐỊA ĐIỂM -> GRAPH NODE
    // =====================================================

    int startGraphNode =
        findNearestRoadNode(
            startLocation
            );


    int endGraphNode =
        findNearestRoadNode(
            endLocation
            );


    // =====================================================
    // 9. KIỂM TRA GRAPH NODE ĐIỂM ĐI
    // =====================================================

    if (startGraphNode == -1)
    {
        ui->textEdit->setText(
            "KHÔNG TÌM THẤY NODE ĐƯỜNG\n\n"
            "Không tìm thấy node đường gần:\n" +
            startName
            );

        ui->listWidget->clear();

        ui->listWidget->addItem(
            "Không tìm thấy node đường cho điểm đi."
            );

        return;
    }


    // =====================================================
    // 10. KIỂM TRA GRAPH NODE ĐIỂM ĐẾN
    // =====================================================

    if (endGraphNode == -1)
    {
        ui->textEdit->setText(
            "KHÔNG TÌM THẤY NODE ĐƯỜNG\n\n"
            "Không tìm thấy node đường gần:\n" +
            endName
            );

        ui->listWidget->clear();

        ui->listWidget->addItem(
            "Không tìm thấy node đường cho điểm đến."
            );

        return;
    }


    // =====================================================
    // 11. CHẠY DIJKSTRA
    // =====================================================
    QElapsedTimer routeTimer;
    routeTimer.start();
    auto result =
        dijkstraCar(
            graph,
            startGraphNode,
            endGraphNode
            );

    auto Result = dijkstraMotorCycle(
        graph,
        startGraphNode,
        endGraphNode
        );
    qint64 executionTime = routeTimer.nsecsElapsed();
    double executionTimeMs =
        executionTime / 1000000.0;
    // =====================================================
    // 12. KHÔNG CÓ ĐƯỜNG ĐI
    // =====================================================

    if (result.first == 0)
    {
        ui->textEdit->setText(
            "KHÔNG TÌM THẤY ĐƯỜNG ĐI\n\n"

            "Điểm đi:\n" +
            startName +

            "\n\n"

            "Điểm đến:\n" +
            endName +

            "\n\n"

            "Không có tuyến đường kết nối "
            "hai địa điểm này."
            );


        ui->listWidget->clear();

        ui->listWidget->addItem(
            "KHÔNG CÓ LỘ TRÌNH"
            );

        ui->listWidget->addItem(
            ""
            );

        ui->listWidget->addItem(
            "Điểm đi: " +
            startName
            );

        ui->listWidget->addItem(
            "Điểm đến: " +
            endName
            );

        return;
    }


    // =====================================================
    // 13. LẤY KHOẢNG CÁCH
    // =====================================================

    double distanceMeter =
        result.first;


    double distanceKm =
        distanceMeter / 1000.0;


    // =====================================================
    // 14. HIỂN THỊ THÔNG TIN LỘ TRÌNH
    // =====================================================

    QString info;


    info +=
        "THÔNG TIN LỘ TRÌNH\n";

    info +=
        "==============================\n\n";


    info +=
        "ĐIỂM ĐI:\n";

    info +=
        startName;

    info +=
        "\n\n";


    info +=
        "ĐIỂM ĐẾN:\n";

    info +=
        endName;

    info +=
        "\n\n";


    info +=
        "TỔNG QUÃNG ĐƯỜNG NGẮN NHẤT:\n";


    info +=
        QString::number(
            distanceKm,
            'f',
            3
            );


    info +=
        " km\n";


    info +=
        "(";


    info +=
        QString::number(
            distanceMeter,
            'f',
            2
            );


    info +=
        " m)";
    info +=
        "\n\nTHỜI GIAN THỰC THI:\n";

    info +=
        QString::number(
            executionTimeMs,
            'f',
            3
            );

    info +=
        " ms";

    ui->textEdit->setText(info);


    // =====================================================
    // 15. HIỂN THỊ LỘ TRÌNH BÊN PHẢI
    // =====================================================

    ui->listWidget->clear();


    ui->listWidget->addItem(
        "LỘ TRÌNH NGẮN NHẤT"
        );


    ui->listWidget->addItem(
        "=============================="
        );


    ui->listWidget->addItem(
        "ĐI TỪ: " +
        startName
        );


    ui->listWidget->addItem(
        "ĐẾN: " +
        endName
        );


    ui->listWidget->addItem("");


    ui->listWidget->addItem(
        "QUÃNG ĐƯỜNG: " +
        QString::number(
            distanceKm,
            'f',
            3
            ) +
        " km"
        );


    ui->listWidget->addItem("");


    ui->listWidget->addItem(
        "CÁC TUYẾN ĐƯỜNG:"
        );
    info +=
        "\n\nTHỜI GIAN THỰC THI:\n";

    info +=
        QString::number(
            executionTimeMs,
            'f',
            3
            );

    info +=
        " ms";

    // =====================================================
    // 16. HIỂN THỊ ROAD NAME
    // =====================================================

    int roadNumber = 1;


    for (int i = 0;
         i < (int)result.second.size();
         i++)
    {
        QString road =
            QString::fromStdString(
                result.second[i]
                );


        // Bỏ road name rỗng
        if (road.trimmed().isEmpty())
            continue;


        // Không hiển thị cùng một đường
        // liên tiếp nhiều lần
        if (i == 0 ||
            result.second[i] !=
                result.second[i - 1])
        {
            ui->listWidget->addItem(
                QString::number(
                    roadNumber
                    ) +
                ". " +
                road
                );

            roadNumber++;
        }
    }
    // =====================================================
    // 17. KẾT THÚC
    // =====================================================

    ui->listWidget->addItem("");


    ui->listWidget->addItem(
        "ĐÃ ĐẾN ĐIỂM CUỐI: " +
        endName
        );
}

