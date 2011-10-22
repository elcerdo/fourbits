#include "mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QColor>
#include <QPalette>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);
    widgetFB->setFocus();
}

MainWindow::~MainWindow() {
}

void MainWindow::on_widgetFB_stateChanged(int state) {
    //qDebug() << "new state" << state;
    player.clearNotes();
    player.setNote(state,true);
    //FIXME
    //qDebug() << "*********************";
    QPalette pal;
    pal.setColor(QPalette::Text,QColor::fromRgbF(1,1,0));
    label->setPalette(pal);
    label->update();
    //qDebug() << "*********************";
}

void MainWindow::on_widgetFB_stateReleased(int /*state*/) {
    //qDebug() << "new state released" << state;
    player.fade();
}

void MainWindow::on_widgetFB_padPressed(int number) {
    //qDebug() << "hitted" << number;
    player.playSample(number);
}

void MainWindow::on_actionResetVolumes_activated() {
    sliderSampleVolume0->setValue(0);
    sliderSampleVolume1->setValue(0);
    sliderSampleVolume2->setValue(0);
    sliderSampleVolume3->setValue(0);
    sliderSynthVolume->setValue(0);
}

float computeVolume(int value) {
    return powf(2.,value/32.);
}
void MainWindow::on_sliderSampleVolume0_valueChanged(int value) {
    player.setSampleVolume(0,computeVolume(value));
}
void MainWindow::on_sliderSampleVolume1_valueChanged(int value) {
    player.setSampleVolume(1,computeVolume(value));
}
void MainWindow::on_sliderSampleVolume2_valueChanged(int value) {
    player.setSampleVolume(2,computeVolume(value));
}
void MainWindow::on_sliderSampleVolume3_valueChanged(int value) {
    player.setSampleVolume(3,computeVolume(value));
}
void MainWindow::on_sliderSynthVolume_valueChanged(int value) {
    player.setSynthVolume(computeVolume(value));
}

void MainWindow::on_actionOpenDirectory_activated() {
    QString directory = QFileDialog::getExistingDirectory(this,"Open Samples Directory","/home");
    if (directory.isNull()) return;
    bool ok = player.loadSamplesFromDirectory(qPrintable(directory));
    if (ok) qDebug() << "load ok" << directory;
    //qDebug() << "open directory" << directory;

    //Q_ASSERT(QSound::isAvailable());

    //int kk = 0;
    //while (kk<99) {
    //    QString filename = directory + QString("/%1.wav").arg(kk,2,10,QChar('0'));
    //    qDebug() << "**" << filename;

    //    kk++;
    //}

}
