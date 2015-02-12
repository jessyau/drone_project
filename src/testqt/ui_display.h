/********************************************************************************
** Form generated from reading UI file 'display.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Display
{
public:
    QWidget *centralWidget;
    QLabel *lblImage;
    QLineEdit *txtDisplay;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Display)
    {
        if (Display->objectName().isEmpty())
            Display->setObjectName(QStringLiteral("Display"));
        Display->resize(719, 502);
        centralWidget = new QWidget(Display);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        lblImage = new QLabel(centralWidget);
        lblImage->setObjectName(QStringLiteral("lblImage"));
        lblImage->setGeometry(QRect(5, 0, 641, 406));
        txtDisplay = new QLineEdit(centralWidget);
        txtDisplay->setObjectName(QStringLiteral("txtDisplay"));
        txtDisplay->setGeometry(QRect(15, 415, 696, 27));
        Display->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Display);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 719, 25));
        Display->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Display);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Display->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Display);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Display->setStatusBar(statusBar);

        retranslateUi(Display);

        QMetaObject::connectSlotsByName(Display);
    } // setupUi

    void retranslateUi(QMainWindow *Display)
    {
        Display->setWindowTitle(QApplication::translate("Display", "Display", 0));
        lblImage->setText(QApplication::translate("Display", "~~", 0));
    } // retranslateUi

};

namespace Ui {
    class Display: public Ui_Display {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAY_H
