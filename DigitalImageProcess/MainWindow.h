#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include "opencv\cv.hpp"
#include "opencv2\highgui\highgui.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::MainWindowClass ui;
};
