#include "widgettab.h"

#include <QPainter>
#include <QDebug>

#define WIDTH 1000
#define HEIGHT 400
#define NBEATS 4

//static const QColor colors[NBEATS] = {
//    QColor::fromCmykF(1,0,0,0),
//    QColor::fromCmykF(0,1,0,0),
//    QColor::fromCmykF(0,0,1,0),
//    QColor::fromCmykF(0,0,0,1)
//};

#define OUTTERRADIUS 20
#define OUTTERWIDTHMIN 12
#define INNERRADIUSMAX 10
//#define OUTTERWIDTHMIN 10
//#define INNERRADIUSMAX 14
#define INNERRADIUSMIN 0
#define KERN 3
#define FADESPEED .3

// layer style
void WidgetTab::drawLayers(QPainter &painter) const {
    painter.save();
    painter.translate(-((NBEATS-1)*(OUTTERRADIUS*2+KERN))/2,-((NBEATS-1)*(OUTTERRADIUS*2+KERN))/2);

    { // drawing back
	for (int ii=0; ii<NBEATS; ii++) {
	    for (int jj=0; jj<NBEATS; jj++) {
		painter.save();
		painter.translate(ii*(OUTTERRADIUS*2+KERN),jj*(OUTTERRADIUS*2+KERN));
		QPen pen;
		pen.setWidth(OUTTERWIDTHMIN);
		pen.setJoinStyle(Qt::RoundJoin);
		painter.setPen(pen);
		painter.drawRect(-OUTTERRADIUS,-OUTTERRADIUS,2*OUTTERRADIUS,2*OUTTERRADIUS);
		painter.restore();
		//int radius = OUTTERWIDTHMIN+OUTTERRADIUS;
		//painter.save();
		//painter.translate(ii*(OUTTERRADIUS*2+KERN),jj*(OUTTERRADIUS*2+KERN));
		//QBrush brush;
		//brush.setColor("black");
		//brush.setStyle(Qt::SolidPattern);
		//painter.setBrush(brush);
		//painter.setPen(Qt::NoPen);
		//painter.drawEllipse(-radius,-radius,2*radius,2*radius);
		//painter.restore();
	    }
	}
    }

    { // drawing dots
	for (int ii=0; ii<NBEATS; ii++) {
	    for (int jj=0; jj<NBEATS; jj++) {
		painter.save();
		painter.translate(ii*(OUTTERRADIUS*2+KERN),jj*(OUTTERRADIUS*2+KERN));
		QBrush brush;
		brush.setStyle(Qt::SolidPattern);
		brush.setColor("white");
		painter.setBrush(brush);
		QPen pen;
		pen.setWidth(5);
		pen.setColor("white");
		pen.setJoinStyle(Qt::RoundJoin);
		painter.setPen(pen);
		painter.drawRect(-OUTTERRADIUS+5,-OUTTERRADIUS+5,2*OUTTERRADIUS-10,2*OUTTERRADIUS-10);
		painter.restore();
	    }
	}
    }

    { // drawing color
	for (int cstate=0; cstate<16; cstate++) {
	    painter.save();
	    int ii = cstate%4;
	    if (ii==2) ii=3;
	    else if (ii==3) ii=2;
	    int jj = cstate/4;
	    if (jj==2) jj=3;
	    else if (jj==3) jj=2;
	    painter.translate(ii*(OUTTERRADIUS*2+KERN),jj*(OUTTERRADIUS*2+KERN));
	    QBrush brush;
	    brush.setStyle(Qt::SolidPattern);
	    if (cstate==state) brush.setColor("red");
	    QPen pen;
	    pen.setJoinStyle(Qt::RoundJoin);
	    pen.setWidth(5);
	    pen.setColor(brush.color());
	    painter.setBrush(brush);
	    painter.setPen(pen);
	    painter.drawRect(-radiuses[cstate],-radiuses[cstate],2*radiuses[cstate],2*radiuses[cstate]);
	    painter.restore();
	}
    }

    painter.restore();
}

void WidgetTab::on_timer_timeout() {
    for (int kk=0; kk<16; kk++) {
		radiuses[kk] -= FADESPEED;
		if (radiuses[kk]<INNERRADIUSMIN) radiuses[kk]=INNERRADIUSMIN;
    }
    update();
}

WidgetTab::WidgetTab(QWidget* parent) : QWidget(parent) {
	const int s = NBEATS*OUTTERRADIUS*2+(NBEATS-1)*KERN+2*OUTTERWIDTHMIN+10;
    setMinimumHeight(s);
    setMinimumWidth(s);

    state = 0;
    for (int kk=0; kk<16; kk++) {
		radiuses[kk] = 0;
    }

    timer = new QTimer(this);
    timer->setObjectName("timer");
    timer->setInterval(50);

    QMetaObject::connectSlotsByName(this);

    timer->start();
}

void WidgetTab::setState(int state) {
    this->state = state;
    radiuses[state] = INNERRADIUSMAX;
    update();
}

void WidgetTab::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(width()/2,height()/2);
    drawLayers(painter);
}

