/********************************************************************************
** Form generated from reading UI file 'mainwindow(2).ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_28_2_29__H
#define UI_MAINWINDOW_28_2_29__H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *pushButton_5;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QTextEdit *textEdit;
    QListWidget *listWidget;
    QTextEdit *textEdit_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(840, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        pushButton_5 = new QGroupBox(centralwidget);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(30, 110, 360, 231));
        lineEdit = new QLineEdit(pushButton_5);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(20, 35, 210, 35));
        lineEdit_2 = new QLineEdit(pushButton_5);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(20, 80, 320, 35));
        pushButton = new QPushButton(pushButton_5);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(20, 130, 320, 40));
        pushButton_2 = new QPushButton(pushButton_5);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(240, 30, 101, 41));
        pushButton_3 = new QPushButton(pushButton_5);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(240, 180, 101, 29));
        pushButton_4 = new QPushButton(pushButton_5);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(20, 180, 90, 29));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(30, 350, 360, 290));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(410, 120, 401, 521));
        textEdit_2 = new QTextEdit(centralwidget);
        textEdit_2->setObjectName("textEdit_2");
        textEdit_2->setGeometry(QRect(190, 0, 491, 111));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 840, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "H\341\273\207 th\341\273\221ng T\303\254m \304\221\306\260\341\273\235ng \304\221i TP.HCM", nullptr));
        pushButton_5->setTitle(QCoreApplication::translate("MainWindow", " Th\303\264ng tin l\341\273\231 tr\303\254nh ", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nh\341\272\255p \304\221i\341\273\203m b\341\272\257t \304\221\341\272\247u...", nullptr));
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nh\341\272\255p \304\221i\341\273\203m \304\221\341\272\277n...", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215 T\303\254m \304\221\306\260\341\273\235ng \304\221i t\341\273\221i \306\260u", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "GPS", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "\303\224 t\303\264 ", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Xe m\303\241y", nullptr));
        textEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Th\303\264ng b\303\241o v\303\240 kho\341\272\243ng c\303\241ch s\341\272\275 hi\341\273\203n th\341\273\213 \341\273\237 \304\221\303\242y...", nullptr));
        textEdit_2->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:700;\">BUILDING A PATHFINDING PROGRAM TO FIND ROUTES BETWEEN MAJOR LOCATIONS IN HO CHI MINH CITY USING DIJKSTRA'S ALGORITHM</span></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt; font-weight:700;\"><br /></p"
                        "></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_28_2_29__H
