#include "LogViewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogViewer log_viever_window;
    log_viever_window.show();
    return a.exec();
}
