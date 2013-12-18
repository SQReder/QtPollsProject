#include <QFileDialog>
#include <QTextStream>
#include <QMap>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lwResults->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbOpen_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this, tr("Open log file"), ".", tr("Log (*.txt)"));
    if(filename != "")
        analyseLog(filename);
}

void MainWindow::analyseLog(QString filename) {
    ui->lwResults->clear();
    QFile log(filename);
    log.open(QIODevice::ReadOnly);

    QTextStream stream(&log);

    QStringList lines;
    forever {
        auto line = stream.readLine();
        if(line != "")
            lines.push_back(line);

        if (line.isNull())
            break;
    }

    log.close();

    QMap<QString, QStringList> votes;
    for(QString & item : lines) {
        auto tokenz = item.split(" ");
        auto code = tokenz[0];
        auto voteFile = tokenz[1];
        votes[voteFile].push_back(code);
    }

    for (auto it = votes.begin(); it != votes.end(); ++it) {
        QString count;
        count.setNum(it.value().count());
        ui->lwResults->addItem(it.key() + " " + count);
    }

    ui->lwResults->setVisible(true);
}
