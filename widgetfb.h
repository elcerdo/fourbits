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
    public slots:
	void setStyle(int style);
    signals:
	void padPressed(int number);
	void stateChanged(int state);
	void stateReleased(int state);
    protected slots:
	void on_timer_timeout();
        void on_actionSetStyle_activated();
    protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
	virtual void contextMenuEvent(QContextMenuEvent *event);
	void toogleState(int bit);
	void drawLayers(QPainter &painter) const;
	void drawStatic(QPainter &painter) const;
	struct Bit {
	    Bit();
	    bool state;
	    float width;
	};
	typedef QVector<Bit> Bits;
	Bits bits;
	QTimer *timer;
	QAction *actionSetStyle;
	int style;
};


#endif
