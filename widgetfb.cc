#include "widgetfb.h"

#include <QPainter>
#include <QPen>
#include <QKeySequence>
#include <QBrush>
#include <QDebug>
#include <QColor>

#define WIDTH 1000
#define HEIGHT 400
#define OUTERRADIUS 100
#define INNERRADIUS 80
#define KERN 10
#define NBEATS 4

static const QColor colors[] = {
    QColor::fromCmykF(1,0,0,0),
    QColor::fromCmykF(0,1,0,0),
    QColor::fromCmykF(0,0,1,0),
    QColor::fromCmykF(0,0,0,1)
};

void drawPad(QPainter &painter,int index,int x,int y,bool state) {
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
	brush.setColor(colors[index]);
	painter.setPen(Qt::NoPen);
	painter.setBrush(brush);
	painter.drawEllipse(-INNERRADIUS,-INNERRADIUS,2*INNERRADIUS,2*INNERRADIUS);
	painter.restore();
    }

    painter.restore();
}

QColor WidgetFB::getColorFromState(int state) {
    Bits bits;
    for (int kk=0; kk<NBEATS; kk++) {
	if  (state & (1<<kk)) {
	    bits.append(true);
	} else {
	    bits.append(false);
	}
    }
    return QColor::fromCmykF(bits[0],bits[1],bits[2],bits[3]);
}

WidgetFB::WidgetFB(QWidget *parent) : QWidget(parent) {
    resize(WIDTH,HEIGHT);
    for (int k=0; k<NBEATS; k++) {
	bits.append(false);
    }
}

void WidgetFB::keyReleaseEvent(QKeyEvent* /*event*/) {
    //qDebug() << "released" << event->key();
    emit stateReleased(getState());
}

void WidgetFB::keyPressEvent(QKeyEvent* event) {
    qDebug() << "pressed" << event->key();
    int kk=0;
    if (event->key()==65 || event->key()==81) {
	bits[kk] = !bits[kk];
	//qDebug() << "a" << kk << bits[kk];
	update();
	emit stateChanged(getState());
	emit padPressed(0);
	event->accept();
    }
    kk++;
    if (event->key()==90 || event->key()==83) {
	bits[kk] = !bits[kk];
	//qDebug() << "b" << kk << bits[kk];
	update();
	emit stateChanged(getState());
	emit padPressed(1);
	event->accept();
    }
    kk++;
    if (event->key()==69 || event->key()==68) {
	bits[kk] = !bits[kk];
	//qDebug() << "c" << kk << bits[kk];
	update();
	emit stateChanged(getState());
	emit padPressed(2);
	event->accept();
    }
    kk++;
    if (event->key()==82 || event->key()==70) {
	bits[kk] = !bits[kk];
	//qDebug() << "d" << kk << bits[kk];
	update();
	emit stateChanged(getState());
	emit padPressed(3);
	event->accept();
    }
    event->ignore();
}

int WidgetFB::getState() const {
    int base = 2;
    int current = 1;
    int total = 0;

    for (Bits::const_iterator iter=bits.begin(); iter!=bits.end(); iter++) {
	if (*iter) total += current;	
	current *= base;
    }
    return total;
}


void WidgetFB::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(width()/2,height()/2);
    painter.translate(-((NBEATS-1)*(OUTERRADIUS*2+KERN))/2,0);
    int kk=0;
    //qDebug() << "state =" << getState();
    for (Bits::const_iterator iter=bits.begin(); iter!=bits.end(); iter++) {
	//qDebug() << kk << *iter;
	drawPad(painter,kk,kk*(OUTERRADIUS*2+KERN),0,*iter);
	kk++;
    }
}

