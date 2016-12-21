/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *openAction;
    QAction *saveAction;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(739, 400);
        MainWindowClass->setMinimumSize(QSize(739, 400));
        QFont font;
        font.setFamily(QStringLiteral("Microsoft YaHei UI"));
        MainWindowClass->setFont(font);
        MainWindowClass->setToolButtonStyle(Qt::ToolButtonTextOnly);
        openAction = new QAction(MainWindowClass);
        openAction->setObjectName(QStringLiteral("openAction"));
        openAction->setFont(font);
        saveAction = new QAction(MainWindowClass);
        saveAction->setObjectName(QStringLiteral("saveAction"));
        saveAction->setFont(font);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 739, 23));
        menuBar->setAutoFillBackground(false);
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        MainWindowClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(openAction);
        menu->addAction(saveAction);

        retranslateUi(MainWindowClass);
        QObject::connect(menuBar, SIGNAL(triggered(QAction*)), MainWindowClass, SLOT(openImage()));
        QObject::connect(menuBar, SIGNAL(triggered(QAction*)), MainWindowClass, SLOT(saveImage()));

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "\345\233\276\345\275\242\350\257\206\345\210\253", 0));
        openAction->setText(QApplication::translate("MainWindowClass", "\346\211\223\345\274\200(open imageFile)", 0));
        saveAction->setText(QApplication::translate("MainWindowClass", "\344\277\235\345\255\230(save imae)", 0));
        menu->setTitle(QApplication::translate("MainWindowClass", "\346\226\207\344\273\266(File)", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
