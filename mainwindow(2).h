#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Nút tìm đường hiện tại
    void on_pushButton_clicked();


    // Nút GPS
    void on_pushButton_2_clicked();

    // Nút tìm đường xe máy
    void on_pushButton_3_clicked();

    // Nút tìm đường ô tô
    void on_pushButton_4_clicked();

    // Xử lý GPS
    void handlePositionUpdate(const QGeoPositionInfo &info);
    void handlePositionError(QGeoPositionInfoSource::Error error);


    private:
              Ui::MainWindow *ui;
    QGeoPositionInfoSource *positionSource = nullptr;
};

#endif // MAINWINDOW_H
