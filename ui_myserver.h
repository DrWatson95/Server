/********************************************************************************
** Form generated from reading UI file 'myserver.ui'
**
** Created by: Qt User Interface Compiler version 6.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYSERVER_H
#define UI_MYSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyServer
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MyServer)
    {
        if (MyServer->objectName().isEmpty())
            MyServer->setObjectName(QString::fromUtf8("MyServer"));
        MyServer->resize(400, 300);
        menuBar = new QMenuBar(MyServer);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        MyServer->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MyServer);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MyServer->addToolBar(mainToolBar);
        centralWidget = new QWidget(MyServer);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MyServer->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MyServer);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MyServer->setStatusBar(statusBar);

        retranslateUi(MyServer);

        QMetaObject::connectSlotsByName(MyServer);
    } // setupUi

    void retranslateUi(QMainWindow *MyServer)
    {
        MyServer->setWindowTitle(QCoreApplication::translate("MyServer", "MyServer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyServer: public Ui_MyServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYSERVER_H
