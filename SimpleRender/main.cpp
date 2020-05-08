
#include<qapplication.h>
#include"GLwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GLwindow window;
    window.setWidth(500);
    window.setHeight(500);
    window.show(); 
    return app.exec();
}
