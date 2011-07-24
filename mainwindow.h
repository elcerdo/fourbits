#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "player.h"

class MainWindow : public QMainWindow, protected Ui::MainWindow {
    Q_OBJECT
    public:
	MainWindow(QWidget* parent = NULL);
	~MainWindow();
    public slots:
	void on_widgetFB_stateChanged(int state);
        void on_widgetFB_padPressed(int number);
	void on_widgetFB_stateReleased(int state);
	void on_actionOpenDirectory_activated();
    protected:
	Player player;
};

#endif

