#ifndef __WIDGETTAB_H__
#define __WIDGETTAB_H__

#include <QWidget>
#include <QPaintEvent>
#include <QTimer>

class WidgetTab : public QWidget {
    Q_OBJECT
    public:
	WidgetTab(QWidget* parent=0);
    public slots:
	void setState(int state);
    protected slots:
	void on_timer_timeout();
    protected:
	virtual void paintEvent(QPaintEvent *event);
	void drawLayers(QPainter &painter) const;
	int state;
	float radiuses[16];
	QTimer* timer;
};

#endif

