#include "widgetfb.h"

#include <QPainter>
#include <QPen>
#include <QKeySequence>
#include <QBrush>
#include <QDebug>
#include <QColor>
#include <QMenu>

#define WIDTH 1000
#define HEIGHT 400
#define NBEATS 4

static const QColor colors[NBEATS] = {
    QColor::fromCmykF(1,0,0,0),
    QColor::fromCmykF(0,1,0,0),
    QColor::fromCmykF(0,0,1,0),
    QColor::fromCmykF(0,0,0,1)
};

#define OUTTERRADIUS 80
#define OUTTERWIDTHMIN 80
#define OUTTERWIDTHMAX 100
#define INNERRADIUS 55
#define KERN -20
#define FADESPEED 5

// layer style
void WidgetFB::drawLayers(QPainter &painter) const {
    //qDebug() << "drawing layer";
    painter.save();
    painter.translate(-((NBEATS-1)*(OUTTERRADIUS*2+KERN))/2,0);

    { // drawing back
	int index=0;
	for (Bits::const_iterator iter=bits.begin(); iter!=bits.end(); iter++) {
	    painter.save();
	    painter.translate(index*(OUTTERRADIUS*2+KERN),0);
	    QPen pen;
	    pen.setWidth(iter->width);
	    painter.setPen(pen);
	    painter.drawEllipse(-OUTTERRADIUS,-OUTTERRADIUS,2*OUTTERRADIUS,2*OUTTERRADIUS);
	    painter.restore();
	    index++;
	}
    }

    { // drawing dots
	int index=0;
	for (Bits::const_iterator iter=bits.begin(); iter!=bits.end(); iter++) {
	    painter.save();
	    painter.translate(index*(OUTTERRADIUS*2+KERN),0);
	    QBrush brush;
	    brush.setStyle(Qt::SolidPattern);
	    brush.setColor("white");
	    painter.setPen(Qt::NoPen);
	    painter.setBrush(brush);
	    painter.drawEllipse(-OUTTERRADIUS,-OUTTERRADIUS,2*OUTTERRADIUS,2*OUTTERRADIUS);
	    painter.restore();
	    index++;
	}
    }

    { // drawing color
	int index=0;
	for (Bits::const_iterator iter=bits.begin(); iter!=bits.end(); iter++) {
	    if (iter->state) {
		painter.save();
		painter.translate(index*(OUTTERRADIUS*2+KERN),0);
		QBrush brush;
		brush.setStyle(Qt::SolidPattern);
		brush.setColor(colors[index]);
		painter.setPen(Qt::NoPen);
		painter.setBrush(brush);
		painter.drawEllipse(-INNERRADIUS,-INNERRADIUS,2*INNERRADIUS,2*INNERRADIUS);
		painter.restore();
	    }
	    index++;
	}
    }

    painter.restore();
}

// default style
void WidgetFB::drawStatic(QPainter &painter) const {
    //qDebug() << "drawing static";
    painter.save();
    painter.translate(-((NBEATS-1)*(OUTTERRADIUS*2+KERN))/2,0);

    { // drawing back
	int index=0;
	for (Bits::const_iterator iter=bits.begin(); iter!=bits.end(); iter++) {
	    painter.save();
	    painter.translate(index*(OUTTERRADIUS*2+KERN),0);
	    QPen pen;
	    pen.setWidth(OUTTERWIDTHMIN);
	    painter.setPen(pen);
	    painter.drawEllipse(-OUTTERRADIUS,-OUTTERRADIUS,2*OUTTERRADIUS,2*OUTTERRADIUS);
	    painter.restore();
	    index++;
	}
    }

    { // drawing dots
	int index=0;
	for (Bits::const_iterator iter=bits.begin(); iter!=bits.end(); iter++) {
	    painter.save();
	    painter.translate(index*(OUTTERRADIUS*2+KERN),0);
	    QBrush brush;
	    brush.setStyle(Qt::SolidPattern);
	    brush.setColor("white");
	    painter.setPen(Qt::NoPen);
	    painter.setBrush(brush);
	    painter.drawEllipse(-OUTTERRADIUS,-OUTTERRADIUS,2*OUTTERRADIUS,2*OUTTERRADIUS);
	    painter.restore();
	    index++;
	}
    }

    { // drawing color
	int index=0;
	for (Bits::const_iterator iter=bits.begin(); iter!=bits.end(); iter++) {
	    if (iter->state) {
		painter.save();
		painter.translate(index*(OUTTERRADIUS*2+KERN),0);
		QBrush brush;
		brush.setStyle(Qt::SolidPattern);
		brush.setColor(colors[index]);
		painter.setPen(Qt::NoPen);
		painter.setBrush(brush);
		painter.drawEllipse(-INNERRADIUS,-INNERRADIUS,2*INNERRADIUS,2*INNERRADIUS);
		painter.restore();
	    }
	    index++;
	}
    }

    painter.restore();
}

QColor WidgetFB::getColor() const {
    return QColor::fromCmykF(bits[0].state,bits[1].state,bits[2].state,bits[3].state);
}

void WidgetFB::setStyle(int style) {
    this->style = style;
    style %= 2;
}

void WidgetFB::on_timer_timeout() {
    //qDebug() << "timeout";
    for (Bits::iterator iter=bits.begin(); iter!=bits.end(); iter++) {
	iter->width-=FADESPEED;
	if (iter->width<OUTTERWIDTHMIN) iter->width=OUTTERWIDTHMIN;
    }
	
    update();
}

void WidgetFB::contextMenuEvent(QContextMenuEvent *event) {
    //qDebug() << "context";
    QMenu menu(this);
    menu.addAction(actionSetStyle);
    menu.exec(event->globalPos());
}

void WidgetFB::on_actionSetStyle_activated() {
    style++;
    style %= 2;
}

WidgetFB::WidgetFB(QWidget *parent) : QWidget(parent), style(0) {
    const int w = NBEATS*OUTTERRADIUS*2+(NBEATS-1)*KERN+OUTTERWIDTHMAX+10;
    const int h = (OUTTERRADIUS)*2+OUTTERWIDTHMAX+10;
    setMinimumHeight(h);
    setMinimumWidth(w);
    resize(WIDTH,HEIGHT);
    for (int k=0; k<NBEATS; k++) {
        bits.append(Bit());
    }
    actionSetStyle = new QAction("switch style",this);
    actionSetStyle->setObjectName("actionSetStyle");
    timer = new QTimer(this);
    timer->setObjectName("timer");
    timer->setInterval(50);
    timer->setSingleShot(false);
    timer->start();
    QMetaObject::connectSlotsByName(this);
}

void WidgetFB::keyReleaseEvent(QKeyEvent* /*event*/) {
    //qDebug() << "released" << event->key();
    emit stateReleased(getState());
}

void WidgetFB::toogleState(int num) {
    bits[num].state = !bits[num].state;
    //qDebug() << bits[num].state << bits[num].width;
    bits[num].width = OUTTERWIDTHMAX;
    //qDebug() << "a" << kk << bits[kk];
    update();
}

void WidgetFB::keyPressEvent(QKeyEvent* event) {
    //qDebug() << "pressed" << event->key();
    int kk=0;
    if (event->key()==65 || event->key()==81) {
        toogleState(kk);
        emit stateChanged(getState());
        emit padPressed(kk);
        event->accept();
    }
    kk++;
    if (event->key()==90 || event->key()==83) {
        toogleState(kk);
        emit stateChanged(getState());
        emit padPressed(kk);
        event->accept();
    }
    kk++;
    if (event->key()==69 || event->key()==68) {
        toogleState(kk);
        emit stateChanged(getState());
        emit padPressed(kk);
        event->accept();
    }
    kk++;
    if (event->key()==82 || event->key()==70) {
        toogleState(kk);
        emit stateChanged(getState());
        emit padPressed(kk);
        event->accept();
    }
    event->ignore();
}

int WidgetFB::getState() const {
    int base = 2;
    int current = 1;
    int total = 0;

    for (Bits::const_iterator iter=bits.begin(); iter!=bits.end(); iter++) {
        if (iter->state) total += current;
        current *= base;
    }
    return total;
}


void WidgetFB::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(width()/2,height()/2);
    if (style==0) {
	drawLayers(painter);
    } else {
	drawStatic(painter);
    }
}

WidgetFB::Bit::Bit() :
state(false),
width(OUTTERWIDTHMIN) { }
