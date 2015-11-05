#include "Application.h"
#include "SocketServer.h"
#include "SocketClient.h"

int main(int argc, char *argv[])
{
    QFile file("dataLog.txt");

    Application app(argc, argv, 3333, file);
    return app.exec();
}
