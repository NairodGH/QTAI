#include "qtai.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QTAI window;
    window.resize(1200, 800);
    window.show();
    
    return app.exec();
}