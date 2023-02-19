#include <QtWidgets/QApplication>
#include "QTAI.h"

#define DEBUG

#ifdef DEBUG
#include <windows.h>
#include <iostream>
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTAI qtai;

    #ifdef DEBUG
        if(AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole())
            freopen("CONOUT$", "w", stdout);
    #endif
    qtai.show();
    return app.exec();
}
