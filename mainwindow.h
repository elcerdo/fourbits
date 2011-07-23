#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QSound>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, protected Ui::MainWindow {
    Q_OBJECT
    public:
	MainWindow(QWidget* parent = NULL);
	~MainWindow();
    public slots:
	void on_widgetFB_stateChanged(int state);
	void on_actionOpenDirectory_activated();
    protected:
	typedef QVector<QSound*> Sounds;
	Sounds sounds;
};

#endif

