#include <QApplication>
#include "myserver.h"

int main(int argc, char** argv){
    QApplication app(argc, argv);
    MyServer server(2323);
    if(!server.connectDB())
        exit(1);
    server.show();

    return app.exec();
}
