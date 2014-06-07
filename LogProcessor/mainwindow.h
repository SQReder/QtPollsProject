#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void analyseLog(QString filename);
    QVector<QSharedPointer<LogItem>> parseLogFile(QString filename);

private slots:
    void on_pbOpen_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
