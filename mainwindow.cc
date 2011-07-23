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

    //Q_ASSERT(QSound::isAvailable());

    sounds.clear();
    int kk = 0;
    while (kk<99 && sounds.size()<4) {
	QString filename = directory + QString("/%1.wav").arg(kk,2,10,QChar('0'));
	qDebug() << "**" << filename << sounds.size();
	QSound* sound = new QSound(filename);
	sound->play();

	sounds.append(sound);
	kk++;
    }

}
