#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, protected Ui::MainWindow {
    Q_OBJECT
    public:
	MainWindow(QWidget* parent = NULL);
	~MainWindow();
};

#endif

