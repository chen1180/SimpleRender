#include<qapplication.h>
#include"GLwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
 
    GLwindow window;
    window.setFormat(format); // must be called before the widget or its parent window gets shown
    window.setWidth(1200);
    window.setHeight(800);
    window.show(); 
    return app.exec();
}
