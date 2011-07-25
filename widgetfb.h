#ifndef __WIDGETFB_H__
#define __WIDGETFB_H__

#include <QVector>
#include <QTimer>
#include <QWidget>
#include <QKeyEvent>

class WidgetFB : public QWidget {
    Q_OBJECT
    public:
	WidgetFB(QWidget *parent = NULL);
	QColor getColor() const;
	int getState() const;
    signals:
	void padPressed(int number);
	void stateChanged(int state);
	void stateReleased(int state);
    protected slots:
	void on_timer_timeout();
    protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
	void toogleState(int bit);
	struct Bit {
	    Bit();
	    bool state;
	    int width;
	};
	typedef QVector<Bit> Bits;
	Bits bits;
	QTimer *timer;
	int style;
};


#endif
