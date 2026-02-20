#include "algorithm.hpp"
#include "qtai.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    qmlRegisterUncreatableType<Algorithm>("QTAI", 1, 0, "Algorithm", "Abstract base class");
    qRegisterMetaType<AlgorithmArray>("AlgorithmArray");

    QTAI window;
    window.resize(1200, 800);
    window.show();

    return app.exec();
}