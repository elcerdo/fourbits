#ifndef __WIDGETFB_H__
#define __WIDGETFB_H__

#include <QVector>
#include <QWidget>
#include <QKeyEvent>

class WidgetFB : public QWidget {
    Q_OBJECT
    public:
	WidgetFB(QWidget *parent = NULL);
    protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	typedef QVector<bool> Bits;
	Bits bits;

};


#endif
