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

bool rankingCompare(const QPair<QString, int> &a, const QPair<QString, int> &b) {
    return a.second < b.second;
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

    QMap<QString, int> votes;
    QRegExp regex("^(\\w+)\\s(.+)$");
    for(QString & item : lines) {
        regex.indexIn(item);
        //auto code = regex.cap(1);
        auto voteFile = regex.cap(2);
        votes[voteFile] = votes[voteFile] + 1;
    }


    QVector<QPair<QString, int>> ranking;
    for (auto it = votes.begin(); it != votes.end(); ++it) {
        ranking.push_back(QPair<QString, int>(it.key(), it.value()));
    };

    qSort(ranking.begin(), ranking.end(), rankingCompare);


    for (auto it = ranking.begin(); it != ranking.end(); ++it) {
        QString count;
        count.setNum(it->second);
        ui->lwResults->addItem(it->first + " " + count);
    }

    ui->lwResults->setVisible(true);
}





