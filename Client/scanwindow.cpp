#include <QMessageBox>
#include <QSound>
#include <QCloseEvent>
#include <QKeyEvent>

#include "scanwindow.h"
#include "ui_scanwindow.h"
#include "../Server/protocol.h"
#include "clientconnection.h"
#include "scrollgrid.h"
#include <QTimer>

//#define DISABLE_SCANNER

ScanWindow::ScanWindow(ClientConnection* client, QWidget *parent) :
    QDialog(),
    ui(new Ui::ScanWindow),
    _client(client)
{
    ui->setupUi(this);

    ui->lUsed->setVisible(false);

    canBeClosed = true;
    waitCodeApprovement = false;
    this->setWindowFlags(Qt::CustomizeWindowHint);// | Qt::WindowStaysOnTopHint);

    connect(client, SIGNAL(CodeVerified()), SLOT(onCodeVerified()));
    connect(client, SIGNAL(CodeNotVerified()), SLOT(onCodeNotVerified()));
    connect(client, SIGNAL(CodeAlreadyUsed()), SLOT(onCodeAlreadyUsed()));

#ifndef DISABLE_SCANNER
    _scanThread = new QThread();
    auto _core = new ScanWorker();
    _core->moveToThread(_scanThread);
    connect(_scanThread, SIGNAL(started()), _core, SLOT(work()));
    connect(_core, SIGNAL(finished()), _scanThread, SLOT(quit()));
    connect(_core, SIGNAL(codeScanned(QString)), this, SLOT(onCodeScanned(QString)));

    ui->leCode->setVisible(false);
    ui->pbSendCode->setVisible(false);
#endif

    picWindow = new ScrollGrid(_client);
    picWindow->hide();
    picWindow->LoadPics();
    connect(picWindow, SIGNAL(hided()), this, SLOT(show()));

}

ScanWindow::~ScanWindow()
{
    delete ui;
}

void ScanWindow::on_pushButton_clicked()
{
//    _client->doSendCommand(PollsClient::comCodeVerify, ui->lineEdit->text());
}

void ScanWindow::closeEvent(QCloseEvent *e) {
    e->setAccepted(canBeClosed);
}

void ScanWindow::keyPressEvent(QKeyEvent *e) {
//    auto cs = Qt::ShiftModifier | Qt::ControlModifier;
    auto key = e->key();
    auto mod = e->modifiers();
    if (mod.testFlag(Qt::ControlModifier) && mod.testFlag(Qt::ShiftModifier))
        if (key == Qt::Key_T)
            canBeClosed = true;
}

void ScanWindow::onCodeScanned(QString code) {
    if (waitCodeApprovement == true)
        return;

    waitCodeApprovement = true;
    lastCode = code;
    QStringList params;
    params << code;
    _client->doSendCommand(ProtocolCommand::comCodeVerify, params);
}

void ScanWindow::onCodeVerified() {
    QWidget widget;
    _client->AddToLog("Verified" , Qt::darkGreen);

    picWindow->_code = lastCode;
    picWindow->showMaximized();
    this->hide();
    //QMessageBox::information(&widget, "info", "valid", QMessageBox::Ok);
    waitCodeApprovement = false;
}

void ScanWindow::showEvent(QShowEvent *){
#ifndef DISABLE_SCANNER
    _scanThread->start();
#endif
}

void ScanWindow::onCodeNotVerified() {
    waitCodeApprovement = false;
}

void ScanWindow::onCodeAlreadyUsed() {
    waitCodeApprovement = false;
    ui->lUsed->setVisible(true);
#ifdef DISABLE_SCANNER
    ui->pbSendCode->setVisible(false);
    ui->leCode->setVisible(false);
#endif

    QTimer* timer = new QTimer(this);
    this->connect(timer, SIGNAL(timeout()), SLOT(onTimerStop()));
    timer->setInterval(2000);
    timer->setSingleShot(true);
    timer->start();
}

void ScanWindow::on_pbSendCode_clicked()
{
    onCodeScanned(ui->leCode->text());
}

void ScanWindow::onTimerStop() {
    ui->lUsed->setVisible(false);
#ifdef DISABLE_SCANNER
    ui->pbSendCode->setVisible(true);
    ui->leCode->setVisible(true);
#endif
}
