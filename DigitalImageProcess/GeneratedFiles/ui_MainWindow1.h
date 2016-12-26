/********************************************************************************
** Form generated from reading UI file 'MainWindow1.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW1_H
#define UI_MAINWINDOW1_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindow
{
public:
    QAction *openAction;
    QAction *saveAction;
    QAction *closeAction;
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLabel *label;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QMenuBar *menubar;
    QMenu *menu_File;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName(QStringLiteral("mainWindow"));
        mainWindow->resize(926, 534);
        mainWindow->setMinimumSize(QSize(900, 500));
        openAction = new QAction(mainWindow);
        openAction->setObjectName(QStringLiteral("openAction"));
        saveAction = new QAction(mainWindow);
        saveAction->setObjectName(QStringLiteral("saveAction"));
        closeAction = new QAction(mainWindow);
        closeAction->setObjectName(QStringLiteral("closeAction"));
        centralwidget = new QWidget(mainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayoutWidget_2 = new QWidget(centralwidget);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(840, 180, 82, 141));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        checkBox_2 = new QCheckBox(verticalLayoutWidget_2);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        verticalLayout_2->addWidget(checkBox_2);

        checkBox = new QCheckBox(verticalLayoutWidget_2);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        verticalLayout_2->addWidget(checkBox);

        checkBox_3 = new QCheckBox(verticalLayoutWidget_2);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));

        verticalLayout_2->addWidget(checkBox_3);

        checkBox_4 = new QCheckBox(verticalLayoutWidget_2);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));

        verticalLayout_2->addWidget(checkBox_4);

        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(0, 0, 840, 480));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setLineWidth(0);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 840, 480));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 841, 311));
        scrollArea->setWidget(scrollAreaWidgetContents);
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(840, 0, 82, 111));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_2 = new QPushButton(verticalLayoutWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(verticalLayoutWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        verticalLayout->addWidget(pushButton_3);

        mainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 926, 23));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        mainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(mainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        mainWindow->setStatusBar(statusbar);

        menubar->addAction(menu_File->menuAction());
        menu_File->addAction(openAction);
        menu_File->addAction(saveAction);
        menu_File->addAction(closeAction);

        retranslateUi(mainWindow);

        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindow)
    {
        mainWindow->setWindowTitle(QApplication::translate("mainWindow", "\345\233\276\345\275\242\350\257\206\345\210\253", 0));
        openAction->setText(QApplication::translate("mainWindow", "\346\211\223\345\274\200(open)", 0));
        saveAction->setText(QApplication::translate("mainWindow", "\344\277\235\345\255\230(save)", 0));
        closeAction->setText(QApplication::translate("mainWindow", "\345\205\263\351\227\255(close)", 0));
        checkBox_2->setText(QApplication::translate("mainWindow", "\346\243\200\346\265\213\346\216\245\350\276\271", 0));
        checkBox->setText(QApplication::translate("mainWindow", "\346\243\200\346\265\213\346\273\232\350\212\261", 0));
        checkBox_3->setText(QApplication::translate("mainWindow", "\346\243\200\346\265\213\345\272\225\347\216\257", 0));
        checkBox_4->setText(QApplication::translate("mainWindow", "\346\243\200\346\265\213\345\272\225\351\235\242", 0));
        label->setText(QApplication::translate("mainWindow", "showLabel", 0));
        pushButton_2->setText(QApplication::translate("mainWindow", "\346\255\243\351\235\242", 0));
        pushButton_3->setText(QApplication::translate("mainWindow", "\345\217\215\351\235\242", 0));
        menu_File->setTitle(QApplication::translate("mainWindow", "\346\226\207\344\273\266(File)", 0));
    } // retranslateUi

};

namespace Ui {
    class mainWindow: public Ui_mainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW1_H
