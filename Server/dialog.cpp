#include <QTime>
#include <QDateTime>

#include "dialog.h"
#include "ui_dialog.h"
#include "pollsserver.h"

QTextStream log();

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    startServer();

    QString timestamp;
    timestamp.setNum(QDateTime::currentDateTimeUtc().toTime_t());
    _logFile.setFileName("votes-" + timestamp + ".txt");
    _logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream log(&_logFile);
    QString message("Votes logs in votes-%1.txt");

    addToLog(message.arg(timestamp));
}

Dialog::~Dialog()
{
    _logFile.close();
    delete ui;
}

void Dialog::startServer() {
    _serv = new PollsServer(this);
    connect(_serv, SIGNAL(addLogToGui(QString,QColor)), this, SLOT(onAddLogToGui(QString,QColor)));

    //по умолчанию запускаем сервер на 127.0.0.1:1234
    if (_serv->doStartServer(QHostAddress::LocalHost, 1234))
    {
        addToLog(QTime::currentTime().toString()+" server strated at "+_serv->serverAddress().toString()+":"+QString::number(_serv->serverPort()), Qt::darkGreen);
    }
    else
    {
        addToLog(QTime::currentTime().toString()+" server fail to start at "+_serv->serverAddress().toString()+":"+QString::number(_serv->serverPort()), Qt::darkRed);
        addToLog(QTime::currentTime().toString() + "error is: " + _serv->errorString(), Qt::darkRed);
        ui->pbStartStop->setChecked(true);
    }
}

void Dialog::onAddUserToGui(QString name)
{
    ui->lwClients->addItem(name);
    addToLog(QTime::currentTime().toString()+" "+name+" has joined", Qt::darkGreen);
}

void Dialog::onRemoveUserFromGui(QString name)
{
    for (int i = 0; i < ui->lwClients->count(); ++i)
        if (ui->lwClients->item(i)->text() == name)
        {
            ui->lwClients->takeItem(i);
            addToLog(QTime::currentTime().toString()+" "+name+" has left", Qt::darkGreen);
            break;
        }
}

void Dialog::onMessageToGui(QString message, QString from)
{
    addToLog(QTime::currentTime().toString()+" message from "+from+": "+message);
}

void Dialog::onAddLogToGui(QString string, QColor color)
{
    addToLog(string, color);
}

void Dialog::addToLog(QString text, QColor color)
{
    ui->lwLog->addItem(QTime::currentTime().toString()+" "+text);

    int lastItemIndex = ui->lwLog->count() - 1;
    ui->lwLog->item(lastItemIndex)->setTextColor(color);
}

void Dialog::on_pbStartStop_toggled(bool checked)
{
    if (checked) {
        addToLog(" server stopped at " + _serv->serverAddress().toString() +
                 ":" + QString::number(_serv->serverPort()), Qt::darkGreen);
        _serv->close();
        ui->pbStartStop->setText("Start server");
    }
    else
    {
        QHostAddress addr;
        if (!addr.setAddress(ui->leHost->text()))
        {
            addToLog(" invalid address "+ui->leHost->text(), Qt::darkRed);
            return;
        }
        if (_serv->doStartServer(addr, ui->lePort->text().toInt()))
        {
            addToLog(" server started at "+ui->leHost->text()+":"+ui->lePort->text(), Qt::darkGreen);
            ui->pbStartStop->setText("Stop server");
        }
        else
        {
            addToLog(" server not started at "+ui->leHost->text()+":"+ui->lePort->text(), Qt::darkRed);
            ui->pbStartStop->setChecked(true);
        }
    }
}

void Dialog::onVoteUp(QString code, QString filename) {
    _votes.insert(code, filename);
    QTextStream log(&_logFile);
    log << code << " " << filename << "\n";
    _logFile.flush();
}

bool Dialog::isCodeAlreadyUsed(QString code) {
    return _votes.contains(code);
}
