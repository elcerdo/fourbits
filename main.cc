#include <QApplication>
#include "widgetfb.h"

int main(int argc,char * argv[])
{
    QApplication app(argc,argv);

    WidgetFB main;
    main.show();

    return app.exec();
}


