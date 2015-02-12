/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "widget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QLabel *lblVideofeed;
    QLabel *lblStatus;
    QTextEdit *txtStatus;
    QPushButton *btnResetTracking;
    QLabel *lblScreenshot;
    QLabel *lblScreenshot2;
    QGridLayout *gridLayout;
    QPushButton *btnTurnLeft;
    QPushButton *btnForward;
    QPushButton *btnTurnRight;
    QPushButton *btnLeft;
    QPushButton *btnBackward;
    QPushButton *btnRight;
    QPushButton *btnUp;
    QPushButton *btnHover;
    QPushButton *btnDown;
    QPushButton *btnTakeoffOrLand;
    QPushButton *btnEmergOrRegular;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(963, 662);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        lblVideofeed = new QLabel(centralWidget);
        lblVideofeed->setObjectName(QStringLiteral("lblVideofeed"));
        lblVideofeed->setMinimumSize(QSize(400, 0));

        gridLayout_2->addWidget(lblVideofeed, 0, 0, 3, 2);

        lblStatus = new QLabel(centralWidget);
        lblStatus->setObjectName(QStringLiteral("lblStatus"));

        gridLayout_2->addWidget(lblStatus, 0, 2, 1, 1);

        txtStatus = new QTextEdit(centralWidget);
        txtStatus->setObjectName(QStringLiteral("txtStatus"));
        txtStatus->setMaximumSize(QSize(400, 16777215));

        gridLayout_2->addWidget(txtStatus, 1, 2, 1, 1);

        btnResetTracking = new QPushButton(centralWidget);
        btnResetTracking->setObjectName(QStringLiteral("btnResetTracking"));

        gridLayout_2->addWidget(btnResetTracking, 2, 2, 1, 1);

        lblScreenshot = new QLabel(centralWidget);
        lblScreenshot->setObjectName(QStringLiteral("lblScreenshot"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblScreenshot->sizePolicy().hasHeightForWidth());
        lblScreenshot->setSizePolicy(sizePolicy);
        lblScreenshot->setMinimumSize(QSize(0, 200));
        lblScreenshot->setMaximumSize(QSize(350, 16777215));

        gridLayout_2->addWidget(lblScreenshot, 3, 0, 1, 1);

        lblScreenshot2 = new QLabel(centralWidget);
        lblScreenshot2->setObjectName(QStringLiteral("lblScreenshot2"));
        sizePolicy.setHeightForWidth(lblScreenshot2->sizePolicy().hasHeightForWidth());
        lblScreenshot2->setSizePolicy(sizePolicy);
        lblScreenshot2->setMinimumSize(QSize(0, 200));
        lblScreenshot2->setMaximumSize(QSize(350, 16777215));

        gridLayout_2->addWidget(lblScreenshot2, 3, 1, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        btnTurnLeft = new QPushButton(centralWidget);
        btnTurnLeft->setObjectName(QStringLiteral("btnTurnLeft"));

        gridLayout->addWidget(btnTurnLeft, 0, 0, 1, 1);

        btnForward = new QPushButton(centralWidget);
        btnForward->setObjectName(QStringLiteral("btnForward"));

        gridLayout->addWidget(btnForward, 0, 1, 1, 1);

        btnTurnRight = new QPushButton(centralWidget);
        btnTurnRight->setObjectName(QStringLiteral("btnTurnRight"));

        gridLayout->addWidget(btnTurnRight, 0, 2, 1, 1);

        btnLeft = new QPushButton(centralWidget);
        btnLeft->setObjectName(QStringLiteral("btnLeft"));

        gridLayout->addWidget(btnLeft, 1, 0, 1, 1);

        btnBackward = new QPushButton(centralWidget);
        btnBackward->setObjectName(QStringLiteral("btnBackward"));

        gridLayout->addWidget(btnBackward, 1, 1, 1, 1);

        btnRight = new QPushButton(centralWidget);
        btnRight->setObjectName(QStringLiteral("btnRight"));

        gridLayout->addWidget(btnRight, 1, 2, 1, 1);

        btnUp = new QPushButton(centralWidget);
        btnUp->setObjectName(QStringLiteral("btnUp"));

        gridLayout->addWidget(btnUp, 2, 0, 1, 1);

        btnHover = new QPushButton(centralWidget);
        btnHover->setObjectName(QStringLiteral("btnHover"));

        gridLayout->addWidget(btnHover, 2, 1, 1, 1);

        btnDown = new QPushButton(centralWidget);
        btnDown->setObjectName(QStringLiteral("btnDown"));

        gridLayout->addWidget(btnDown, 2, 2, 1, 1);

        btnTakeoffOrLand = new QPushButton(centralWidget);
        btnTakeoffOrLand->setObjectName(QStringLiteral("btnTakeoffOrLand"));

        gridLayout->addWidget(btnTakeoffOrLand, 3, 0, 1, 3);

        btnEmergOrRegular = new QPushButton(centralWidget);
        btnEmergOrRegular->setObjectName(QStringLiteral("btnEmergOrRegular"));

        gridLayout->addWidget(btnEmergOrRegular, 4, 0, 1, 3);


        gridLayout_2->addLayout(gridLayout, 3, 2, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 963, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        lblVideofeed->setText(QString());
        lblStatus->setText(QApplication::translate("MainWindow", "Status", 0));
        btnResetTracking->setText(QApplication::translate("MainWindow", "Reset Tracking", 0));
        lblScreenshot->setText(QString());
        lblScreenshot2->setText(QString());
        btnTurnLeft->setText(QApplication::translate("MainWindow", "Turn left", 0));
        btnForward->setText(QApplication::translate("MainWindow", "Forward", 0));
        btnTurnRight->setText(QApplication::translate("MainWindow", "Turn right", 0));
        btnLeft->setText(QApplication::translate("MainWindow", "Left", 0));
        btnBackward->setText(QApplication::translate("MainWindow", "Backward", 0));
        btnRight->setText(QApplication::translate("MainWindow", "Right", 0));
        btnUp->setText(QApplication::translate("MainWindow", "Up", 0));
        btnHover->setText(QApplication::translate("MainWindow", "Hover", 0));
        btnDown->setText(QApplication::translate("MainWindow", "Down", 0));
        btnTakeoffOrLand->setText(QApplication::translate("MainWindow", "Takeoff", 0));
        btnEmergOrRegular->setText(QApplication::translate("MainWindow", "Emergency", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
