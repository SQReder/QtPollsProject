#include <QApplication>

#include "connectDialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectDialog w;
    w.show();

    return a.exec();
}
