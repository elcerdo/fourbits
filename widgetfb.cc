#include "widgetfb.h"

#include <QPainter>
#include <QPen>
#include <QKeySequence>
#include <QBrush>
#include <QDebug>
#include <QColor>

#define WIDTH 1000
#define HEIGHT 400
#define OUTTERRADIUS 80
#define OUTTERWIDTHMIN 20
#define OUTTERWIDTHMAX 50
#define INNERRADIUS 50
#define KERN 10
#define NBEATS 4

static const QColor colors[] = {
    QColor::fromCmykF(1,0,0,0),
    QColor::fromCmykF(0,1,0,0),
    QColor::fromCmykF(0,0,1,0),
    QColor::fromCmykF(0,0,0,1)
};

void drawPad(QPainter &painter,int index,int x,int y,bool state, int width) {
    painter.save();
    painter.translate(x,y);

    QBrush brush;

    {
        painter.save();
        QPen pen;
        pen.setWidth(width);
        //pen.setColor(qRgb(255,0,0));
        painter.setPen(pen);
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor("white");
        painter.setBrush(brush);
        painter.drawEllipse(-OUTTERRADIUS,-OUTTERRADIUS,2*OUTTERRADIUS,2*OUTTERRADIUS);
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

QColor WidgetFB::getColor() const {
    return QColor::fromCmykF(bits[0].state,bits[1].state,bits[2].state,bits[3].state);
}

void WidgetFB::on_timer_timeout() {
    qDebug() << "timeout";
    for (Bits::iterator iter=bits.begin(); iter!=bits.end(); iter++) {
	iter->width--;
	if (iter->width<OUTTERWIDTHMIN) iter->width=OUTTERWIDTHMIN;
    }
	
    update();
}

WidgetFB::WidgetFB(QWidget *parent) : QWidget(parent), style(0) {
    resize(WIDTH,HEIGHT);
    for (int k=0; k<NBEATS; k++) {
        bits.append(Bit());
    }
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
    qDebug() << bits[num].state << bits[num].width;
    bits[num].width = OUTTERWIDTHMAX;
    //qDebug() << "a" << kk << bits[kk];
    update();
}

void WidgetFB::keyPressEvent(QKeyEvent* event) {
    qDebug() << "pressed" << event->key();
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
    painter.translate(-((NBEATS-1)*(OUTTERRADIUS*2+KERN))/2,0);
    qDebug() << "style" << style;
    if (style==1) {
    } else {
	int kk=0;
	//qDebug() << "state =" << getState();
	for (Bits::const_iterator iter=bits.begin(); iter!=bits.end(); iter++) {
	    //qDebug() << kk << *iter;
	    drawPad(painter,kk,kk*(OUTTERRADIUS*2+KERN),0,iter->state,iter->width);
	    kk++;
	}
    }
}

WidgetFB::Bit::Bit() :
    state(false),
    width(OUTTERWIDTHMIN) {
}
