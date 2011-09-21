#ifndef __WIDGETTAB_H__
#define __WIDGETTAB_H__

#include <QMenu>
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
        void selectTune();
    protected:
	virtual void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent* event);
	void contextMenuEvent(QContextMenuEvent* event);

	void drawLayers(QPainter& painter) const;
	void updateTransform();

	int state;
	float radiuses[16];
	QTimer* timer;
	QTransform transform;
	QMenu* tuneMenu;
};

#endif

