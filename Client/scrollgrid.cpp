#include <QScroller>
#include <QDir>
#include <QtWidgets>
#include "scrollgrid.h"
#include "ui_scrollgrid.h"
#include "clientconnection.h"
#include "clicablelabel.h"
#include "confirmvote.h"
#include "../Server/protocol.h"

extern QString picsPath;

ScrollGrid::ScrollGrid(ClientConnection *client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScrollGrid),
    _client(client)
{
    ui->setupUi(this);
    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);

    QObject::connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)),
                     ui->scrollArea->verticalScrollBar(), SLOT(setValue(int)));

    QObject::connect(ui->scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     ui->verticalScrollBar, SLOT(setValue(int)));

    QObject::connect(ui->verticalScrollBar, SIGNAL(rangeChanged(int,int)),
                     ui->scrollArea->verticalScrollBar(), SLOT(setRange(int, int)));

    this->setWindowFlags(Qt::CustomizeWindowHint);// | Qt::WindowStaysOnTopHint);

    ConfirmVote* confirmDialog = new ConfirmVote(parent);
    connect(this, SIGNAL(OpenConfirm(QString)), confirmDialog, SLOT(onOpenConfirm(QString)));
    connect(confirmDialog, SIGNAL(VoteConfirmed(QString)), this, SLOT(onVoteConfirmed(QString)));
    connect(confirmDialog, SIGNAL(VoteRejected()), this, SLOT(onVoteRejected()));
    confirmDialog->hide();
}

ScrollGrid::~ScrollGrid()
{
    delete ui;
}

void ScrollGrid::showEvent(QShowEvent *) {
}

void ScrollGrid::resizeEvent(QResizeEvent *) {
    ui->verticalScrollBar->setMaximum(ui->scrollArea->verticalScrollBar()->maximum());
    ui->verticalScrollBar->setMinimum(ui->scrollArea->verticalScrollBar()->minimum());
}

void ScrollGrid::hideEvent(QHideEvent *) {
    emit hided();
}

void ScrollGrid::on_pushButton_clicked()
{
    this->hide();
}

void ScrollGrid::LoadPics() {
    auto dir = QDir::current();
    bool gotcha = dir.cd(picsPath);
    if(!gotcha)
        return;
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);

    QVector<ClickableLabel*> labels;

    auto list = dir.entryInfoList();
    for(auto item = list.begin(); item != list.end(); ++item) {
        try {
//            label->setText(item->fileName());
            QPixmap pixmap(item->absoluteFilePath());
            if (pixmap.isNull())
                continue;

            ClickableLabel *label = new ClickableLabel;
            auto thumb = pixmap.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            label->setPixmap(thumb);
            label->setAlignment(Qt::AlignCenter);
            label->setScaledContents(false);
            labels.push_back(label);

            picToFile.insert(label, item->fileName());
            connect(label, SIGNAL(clicked(ClickableLabel*)), this, SLOT(on_labelImage_clicked(ClickableLabel*)));
        } catch (...){
            qDebug() << "error reading " << item->fileName();
        }
    }


    QGridLayout* grid = new QGridLayout();
    for(int i = 0; i != labels.size(); ++i) {
        grid->addWidget(labels[i], i / 5, i % 5);
    }

    delete ui->scrollAreaWidgetContents->layout();
    ui->scrollAreaWidgetContents->setLayout(grid);
}

void ScrollGrid::on_labelImage_clicked(ClickableLabel *who) {
    emit OpenConfirm(picToFile[who]);
    //this->hide();
}

void ScrollGrid::onVoteConfirmed(QString filename) {
    QStringList params;
    params.append(_code);
    params.append(filename);
    _client->doSendCommand(ProtocolCommand::comVoteUp, params);
    this->hide();
    ui->scrollArea->verticalScrollBar()->setValue(0);
}

void ScrollGrid::onVoteRejected() {
    this->show();
}
