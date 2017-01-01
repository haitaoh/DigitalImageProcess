/********************************************************************************
** Form generated from reading UI file 'MainWindow1.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
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
    QCheckBox *checkBox1;
    QCheckBox *checkBox2;
    QCheckBox *checkBox3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLabel *label;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *positiveButton;
    QPushButton *negativeButton;
    QMenuBar *menubar;
    QMenu *menu_File;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName(QStringLiteral("mainWindow"));
        mainWindow->resize(960, 534);
        mainWindow->setMinimumSize(QSize(960, 534));
        openAction = new QAction(mainWindow);
        openAction->setObjectName(QStringLiteral("openAction"));
        QFont font;
        font.setFamily(QStringLiteral("Microsoft YaHei UI"));
        openAction->setFont(font);
        saveAction = new QAction(mainWindow);
        saveAction->setObjectName(QStringLiteral("saveAction"));
        closeAction = new QAction(mainWindow);
        closeAction->setObjectName(QStringLiteral("closeAction"));
        centralwidget = new QWidget(mainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayoutWidget_2 = new QWidget(centralwidget);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(840, 180, 112, 164));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
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

        checkBox2 = new QCheckBox(verticalLayoutWidget_2);
        checkBox2->setObjectName(QStringLiteral("checkBox2"));
        checkBox2->setFont(font);
        checkBox2->setStyleSheet(QLatin1String("QCheckBox{\n"
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

        verticalLayout_2->addWidget(checkBox2);

        checkBox3 = new QCheckBox(verticalLayoutWidget_2);
        checkBox3->setObjectName(QStringLiteral("checkBox3"));
        checkBox3->setFont(font);
        checkBox3->setStyleSheet(QLatin1String("QCheckBox{\n"
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

        verticalLayout_2->addWidget(checkBox3);

        scrollArea = new QScrollArea(centralwidget);
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
        label->setGeometry(QRect(0, 0, 841, 481));
        label->setFont(font);
        scrollArea->setWidget(scrollAreaWidgetContents);
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(840, 0, 111, 121));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
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

        mainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 960, 23));
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
        mainWindow->setWindowTitle(QApplication::translate("mainWindow", "\345\233\276\345\275\242\350\257\206\345\210\253", Q_NULLPTR));
        openAction->setText(QApplication::translate("mainWindow", "\346\211\223\345\274\200(open)", Q_NULLPTR));
        saveAction->setText(QApplication::translate("mainWindow", "\344\277\235\345\255\230(save)", Q_NULLPTR));
        closeAction->setText(QApplication::translate("mainWindow", "\345\205\263\351\227\255(close)", Q_NULLPTR));
        checkBox1->setText(QApplication::translate("mainWindow", "\346\243\200\346\265\213\345\234\206", Q_NULLPTR));
        checkBox2->setText(QApplication::translate("mainWindow", "\346\243\200\346\265\213\346\216\245\350\276\271", Q_NULLPTR));
        checkBox3->setText(QApplication::translate("mainWindow", "\346\243\200\346\265\213\346\273\232\350\212\261", Q_NULLPTR));
        label->setText(QString());
        positiveButton->setText(QApplication::translate("mainWindow", "\346\255\243\351\235\242", Q_NULLPTR));
        negativeButton->setText(QApplication::translate("mainWindow", "\345\217\215\351\235\242", Q_NULLPTR));
        menu_File->setTitle(QApplication::translate("mainWindow", "\346\226\207\344\273\266(File)", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mainWindow: public Ui_mainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW1_H
