#include<qapplication.h>
#include"GLwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GLwindow window;
    window.setWidth(1200);
    window.setHeight(800);
    window.show(); 
    return app.exec();
}
