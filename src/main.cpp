#include "utility/LogManager.h"
#include "app/Application.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    Singleton<LogManager>::GetInstance()->Init();
    QApplication a(argc, argv);

    Application app;
    app.Run();

    a.exec();
}