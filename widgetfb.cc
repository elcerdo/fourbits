#include "widgetfb.h"

#include <QPainter>
#include <QPen>
#include <QKeySequence>
#include <QBrush>
#include <QDebug>

#define WIDTH 1000
#define HEIGHT 400
#define OUTERRADIUS 100
#define INNERRADIUS 80
#define KERN 10
#define NBEATS 4

void drawPad(QPainter &painter,int x,int y,bool state) {
    painter.save();
    painter.translate(x,y);

    QBrush brush;

    {
	painter.save();
	QPen pen;
	pen.setWidth(4);
	//pen.setColor(qRgb(255,0,0));
	painter.setPen(pen);
	painter.drawEllipse(-OUTERRADIUS,-OUTERRADIUS,2*OUTERRADIUS,2*OUTERRADIUS);
	painter.restore();
    }

    if (state) {
	painter.save();
	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	//brush.setColor(qRgb(255,0,0));
	painter.setBrush(brush);
	painter.drawEllipse(-INNERRADIUS,-INNERRADIUS,2*INNERRADIUS,2*INNERRADIUS);
	painter.restore();
    }

    painter.restore();
}

WidgetFB::WidgetFB(QWidget *parent) : QWidget(parent) {
    resize(WIDTH,HEIGHT);
    for (int k=0; k<NBEATS; k++) {
	bits.append(false);
    }
    bits[0] = true;
}

void WidgetFB::keyPressEvent(QKeyEvent *event) {
    qDebug() << "pressed" << event->key();
    int kk=0;
    if (event->key()==65) {
	bits[kk] = !bits[kk];
	qDebug() << "a" << kk << bits[kk];
	update();
    }
    kk++;
    if (event->key()==90) {
	bits[kk] = !bits[kk];
	qDebug() << "b" << kk << bits[kk];
	update();
    }
    kk++;
    if (event->key()==69) {
	bits[kk] = !bits[kk];
	qDebug() << "c" << kk << bits[kk];
	update();
    }
    kk++;
    if (event->key()==82) {
	bits[kk] = !bits[kk];
	qDebug() << "d" << kk << bits[kk];
	update();
    }
}

void WidgetFB::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(width()/2,height()/2);
    painter.translate(-((NBEATS-1)*(OUTERRADIUS*2+KERN))/2,0);
    int kk=0;
    for (Bits::const_iterator iter=bits.begin(); iter!=bits.end(); iter++) {
	qDebug() << kk << *iter;
	drawPad(painter,kk*(OUTERRADIUS*2+KERN),0,*iter);
	kk++;
    }
}

