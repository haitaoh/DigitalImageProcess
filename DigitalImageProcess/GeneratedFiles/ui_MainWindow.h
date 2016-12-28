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

class Ui_MainWindowClass
{
public:
    QAction *openAction;
    QAction *saveAction;
    QAction *closeAction;
    QWidget *centralWidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLabel *label;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *positiveButton;
    QPushButton *negativeButton;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QMenuBar *menuBar;
    QMenu *menu;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(926, 533);
        MainWindowClass->setMinimumSize(QSize(900, 500));
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
        closeAction = new QAction(MainWindowClass);
        closeAction->setObjectName(QStringLiteral("closeAction"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        scrollArea = new QScrollArea(centralWidget);
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
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(840, 0, 82, 111));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        positiveButton = new QPushButton(verticalLayoutWidget);
        positiveButton->setObjectName(QStringLiteral("positiveButton"));
        positiveButton->setStyleSheet(QString::fromUtf8("QPushButton#fileBtn,#helpBtn,#settingBtn{  \n"
"    border: 1px solid #C0C0C0;  \n"
"    background-color:#C0C0C0;  \n"
"    border-style: solid;  \n"
"    border-radius:0px;  \n"
"    width: 40px;  //\345\222\214\344\275\240\347\232\204\346\216\247\344\273\266\345\244\247\345\260\217\344\270\200\350\207\264\n"
"    height:20px;  //\345\222\214\344\275\240\347\232\204\346\216\247\344\273\266\345\244\247\345\260\217\344\270\200\350\207\264\n"
"    padding:0 0px;  \n"
"    margin:0 0px;  \n"
"}  \n"
"QPushButton#expBtn{  \n"
"    border: 1px solid #C0C0C0;  \n"
"    background-color:#FFFFFF;  \n"
"    border-style: solid;  \n"
"    border-radius:0px;  \n"
"    width: 100px;  \n"
"    height:20px;  \n"
"    padding:0 0px;  \n"
"}  \n"
"QPushButton#fileBtn:hover,#helpBtn:hover,#settingBtn:hover,#expBtn:hover{     \n"
"    border: 1px solid #E3C46F;  \n"
"    background-color:#FEF4BF;  \n"
"    border-style: solid;  \n"
"    border-radius:2px;  \n"
"    width: 40px;  \n"
"    height:20px;  \n"
"    padding:0 0px;  "
                        "\n"
"}  \n"
"QPushButton#fileBtn:pressed,#helpBtn:pressed,#settingBtn:pressed,#expBtn:pressed{  \n"
"    background-color:#EAF0FF;  \n"
"    border: 1px solid #AAB4C4;  \n"
"    width: 40px;  \n"
"    height:20px;  \n"
"    padding:0 0px;  \n"
"    border-radius:1px;  \n"
"}"));

        verticalLayout->addWidget(positiveButton);

        negativeButton = new QPushButton(verticalLayoutWidget);
        negativeButton->setObjectName(QStringLiteral("negativeButton"));

        verticalLayout->addWidget(negativeButton);

        verticalLayoutWidget_2 = new QWidget(centralWidget);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(840, 180, 82, 181));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        checkBox_2 = new QCheckBox(verticalLayoutWidget_2);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        verticalLayout_2->addWidget(checkBox_2);

        checkBox = new QCheckBox(verticalLayoutWidget_2);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        verticalLayout_2->addWidget(checkBox);

        checkBox_5 = new QCheckBox(verticalLayoutWidget_2);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));

        verticalLayout_2->addWidget(checkBox_5);

        checkBox_3 = new QCheckBox(verticalLayoutWidget_2);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));

        verticalLayout_2->addWidget(checkBox_3);

        checkBox_4 = new QCheckBox(verticalLayoutWidget_2);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));

        verticalLayout_2->addWidget(checkBox_4);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 926, 23));
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
        menu->addAction(closeAction);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "\345\233\276\345\275\242\350\257\206\345\210\253", 0));
        openAction->setText(QApplication::translate("MainWindowClass", "\346\211\223\345\274\200(open)", 0));
        saveAction->setText(QApplication::translate("MainWindowClass", "\344\277\235\345\255\230(save)", 0));
        closeAction->setText(QApplication::translate("MainWindowClass", "\345\205\263\351\227\255(close)", 0));
        label->setText(QApplication::translate("MainWindowClass", "showLabel", 0));
        positiveButton->setText(QApplication::translate("MainWindowClass", "\346\255\243\351\235\242", 0));
        negativeButton->setText(QApplication::translate("MainWindowClass", "\345\217\215\351\235\242", 0));
        checkBox_2->setText(QApplication::translate("MainWindowClass", "\346\243\200\346\265\213\346\216\245\350\276\271", 0));
        checkBox->setText(QApplication::translate("MainWindowClass", "\346\243\200\346\265\213\345\215\267\345\217\243", 0));
        checkBox_5->setText(QApplication::translate("MainWindowClass", "\346\243\200\346\265\213\345\206\205\345\243\201", 0));
        checkBox_3->setText(QApplication::translate("MainWindowClass", "\346\243\200\346\265\213\345\272\225\347\216\257", 0));
        checkBox_4->setText(QApplication::translate("MainWindowClass", "\346\243\200\346\265\213\345\272\225\351\235\242", 0));
        menu->setTitle(QApplication::translate("MainWindowClass", "\346\226\207\344\273\266(File)", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
