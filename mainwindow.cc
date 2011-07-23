#include "mainwindow.h"

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);
    widgetFB->setFocus();
}

MainWindow::~MainWindow() {
}

void MainWindow::on_widgetFB_stateChanged(int state) {
    qDebug() << "new state" << state;
}

void MainWindow::on_actionOpenDirectory_activated() {
    QString directory = QFileDialog::getExistingDirectory();
    if (directory.isNull()) return;
    qDebug() << "open directory" << directory;
}
