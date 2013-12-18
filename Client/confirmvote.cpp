#include "confirmvote.h"
#include "ui_confirmvote.h"
#include "connectDialog.h"

#include <QtGui>
#include <QtWidgets>

ConfirmVote::ConfirmVote(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmVote)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint);// | Qt::WindowStaysOnTopHint);
}

ConfirmVote::~ConfirmVote()
{
    delete ui;
}

void ConfirmVote::onOpenConfirm(QString filename) {
    this->filename = filename;
    QPixmap pixmap(picsPath + filename);
    auto scaled = pixmap.scaled(QSize(1340, 550), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->lPic->setPixmap(scaled);
    ui->lFilename->setText(filename);
    this->setWindowFlags(Qt::CustomizeWindowHint);// | Qt::WindowStaysOnTopHint);
    this->showMaximized();
    this->raise();
}

void ConfirmVote::onVoteConfirmed() {
    emit VoteConfirmed(filename);
}

void ConfirmVote::onVoteRejected() {
    emit VoteRejected();
}

void ConfirmVote::showEvent(QShowEvent *) {
    this->raise();
}
