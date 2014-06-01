#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");
    QApplication::setLibraryPaths(paths);

    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}
