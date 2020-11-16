#include <QtWidgets/QApplication>
#include <fstream>
#include <vector>

#include "TCPClientUI.h"
#include "TCPClient.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    TCPClientUI w;
    w.setWindowTitle("Settings");
    w.setWindowFlags(Qt::WindowCloseButtonHint);
    w.show();

    return a.exec();
}