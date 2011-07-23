#include "mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QColor>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);
    widgetFB->setFocus();
}

MainWindow::~MainWindow() {
}

void MainWindow::on_widgetFB_stateChanged(int state) {
    qDebug() << "new state" << state;
    //FIXME
    qDebug() << "*********************";
    QPalette pal;
    pal.setColor(QPalette::Text,QColor::fromRgbF(1,1,0));
    label->setPalette(pal);
    label->update();
    qDebug() << "*********************";
}

void MainWindow::on_actionOpenDirectory_activated() {
    QString directory = QFileDialog::getExistingDirectory();
    if (directory.isNull()) return;
    qDebug() << "open directory" << directory;
}
