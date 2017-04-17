/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
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
    QCheckBox *checkBox1;
    QMenuBar *menuBar;
    QMenu *menu;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(960, 534);
        MainWindowClass->setMinimumSize(QSize(960, 534));
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
        scrollArea->setStyleSheet(QLatin1String("QScrollBar:vertical {\n"
"background: #E6E6E6;\n"
"width: 14px;\n"
"margin: 0px;\n"
"border-radius: 7px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"background: #cfcfcf;\n"
"min-height: 14px;\n"
"border-radius: 7px;\n"
"}\n"
"QScrollBar::handle:pressed{\n"
"background:#a0a0a0;\n"
"}\n"
"QScrollBar::add-line:vertical {\n"
"background: #E6E6E6;\n"
"height: 0px;\n"
"border-radius: 7px;\n"
"}\n"
"QScrollBar::sub-line:vertical {\n"
"background: #E6E6E6;\n"
"height: 0px;\n"
"border-radius: 7px;\n"
"}\n"
"QScrollBar:horizontal{\n"
"background: #E6E6E6;\n"
"height: 14px;\n"
"margin: 0px;\n"
"border-radius: 7px;\n"
"}\n"
"QScrollBar::handle:horizontal{\n"
"background: #cfcfcf;\n"
"min-width: 14px;\n"
"border-radius: 7px;\n"
"}\n"
"QScrollBar::handle:pressed{\n"
"background:#a0a0a0;\n"
"}\n"
"QScrollBar::add-line:horizontal{\n"
"background: #E6E6E6;\n"
"width: 0px;\n"
"border-radius: 7px;\n"
"}\n"
"QScrollBar::sub-line:horizontal{\n"
"background: #E6E6E6;\n"
"width: 0px;\n"
"border-radius: 7px;\n"
"}"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setLineWidth(0);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 840, 480));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 171, 61));
        scrollArea->setWidget(scrollAreaWidgetContents);
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(840, 0, 111, 131));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        positiveButton = new QPushButton(verticalLayoutWidget);
        positiveButton->setObjectName(QStringLiteral("positiveButton"));
        positiveButton->setFont(font);
        positiveButton->setStyleSheet(QLatin1String("QPushButton{\n"
"  padding: 4px 12px;\n"
"  margin-bottom: 0;\n"
"  font-size: 20px;\n"
"  font-family:Microsoft YaHei UI;\n"
"  line-height: 20px;\n"
"  color: #333333;\n"
"  text-align: center;\n"
"  vertical-align: middle;\n"
"  background-color: #cfcfcf;\n"
"  background-repeat: repeat-x;\n"
"  border-color: #cfcfcf;\n"
"  border-bottom-color:#cfcfcf;\n"
"  border-radius: 8px;\n"
"border-style:inset;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: #a0a0a0;\n"
"}\n"
"QPushButton:pressed{\n"
"  background-color: #888888;\n"
"border-style:outset;\n"
"}"));

        verticalLayout->addWidget(positiveButton);

        negativeButton = new QPushButton(verticalLayoutWidget);
        negativeButton->setObjectName(QStringLiteral("negativeButton"));
        negativeButton->setFont(font);
        negativeButton->setStyleSheet(QLatin1String("QPushButton{\n"
"  padding: 4px 12px;\n"
"  margin-bottom: 0;\n"
"  font-size: 20px;\n"
"  font-family:Microsoft YaHei UI;\n"
"  line-height: 20px;\n"
"  color: #333333;\n"
"  text-align: center;\n"
"  vertical-align: middle;\n"
"  background-color: #cfcfcf;\n"
"  background-repeat: repeat-x;\n"
"  border-color: #cfcfcf;\n"
"  border-bottom-color:#cfcfcf;\n"
"  border-radius: 8px;\n"
"border-style:inset;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: #a0a0a0;\n"
"}\n"
"QPushButton:pressed{\n"
"  background-color: #888888;\n"
"border-style:outset;\n"
"}"));

        verticalLayout->addWidget(negativeButton);

        verticalLayoutWidget_2 = new QWidget(centralWidget);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(840, 180, 112, 108));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        checkBox1 = new QCheckBox(verticalLayoutWidget_2);
        checkBox1->setObjectName(QStringLiteral("checkBox1"));
        checkBox1->setFont(font);
        checkBox1->setStyleSheet(QLatin1String("QCheckBox{\n"
"	font-size:18px;\n"
"font-family:Microsoft YaHei UI;\n"
"}\n"
"QCheckBox::indicator { \n"
"    width: 26px;\n"
"    height: 50px;\n"
"}\n"
"QCheckBox::indicator::unchecked {   \n"
"    image: url(images/unchecked.png);\n"
"}\n"
"QCheckBox::indicator::checked { \n"
"    image: url(images/checked.png);\n"
"}"));

        verticalLayout_2->addWidget(checkBox1);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 960, 23));
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
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "\345\233\276\345\275\242\350\257\206\345\210\253", Q_NULLPTR));
        openAction->setText(QApplication::translate("MainWindowClass", "\346\211\223\345\274\200(open)", Q_NULLPTR));
        saveAction->setText(QApplication::translate("MainWindowClass", "\344\277\235\345\255\230(save)", Q_NULLPTR));
        closeAction->setText(QApplication::translate("MainWindowClass", "\345\205\263\351\227\255(close)", Q_NULLPTR));
        label->setText(QString());
        positiveButton->setText(QApplication::translate("MainWindowClass", "\346\255\243\351\235\242", Q_NULLPTR));
        negativeButton->setText(QApplication::translate("MainWindowClass", "\345\217\215\351\235\242", Q_NULLPTR));
        checkBox1->setText(QApplication::translate("MainWindowClass", "\346\243\200\346\265\213\345\234\206", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindowClass", "\346\226\207\344\273\266(File)", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
