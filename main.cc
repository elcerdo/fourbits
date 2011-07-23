#include <QApplication>
#include <QWidget>

int main(int argc,char * argv[])
{
    QApplication app(argc,argv);

    QWidget main;
    main.show();

    return app.exec();
}


