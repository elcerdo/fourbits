#ifndef __WIDGETFB_H__
#define __WIDGETFB_H__

#include <QVector>
#include <QWidget>
#include <QKeyEvent>

class WidgetFB : public QWidget {
    Q_OBJECT
    public:
	WidgetFB(QWidget *parent = NULL);
	static QColor getColorFromState(int state);
	int getState() const;
    signals:
	void stateChanged(int state);
	void stateReleased(int state);
    protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
	typedef QVector<bool> Bits;
	Bits bits;

};


#endif
