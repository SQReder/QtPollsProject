#include <QTime>
#include <QDateTime>

#include "dialog.h"
#include "ui_dialog.h"
#include "pollsserver.h"
#include "configmanager.h"
#include "logger.h"

QTextStream log();

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    loadConfig();
    initializeLogging();
    _serv= QSharedPointer<PollsServer>(nullptr);
}


Dialog::~Dialog()
{
    _logFile.close();
    delete ui;
}


void Dialog::startServer() {

    if (!_serv.isNull())
        _serv.clear();
    _serv = QSharedPointer<PollsServer>(new PollsServer(this));

    auto ip = ui->leHost->text();
    auto port = ui->lePort->text();

    QHostAddress addr;
    if (!addr.setAddress(ip))
    {
        Logger::error("Invalid address " + ip);
        return;
    }

    if (_serv->doStartServer(addr, port.toInt()))
    {
        Logger::success("Server started at " + ip + ":" + port);
        ui->pbStartStop->setText("Stop server");
    }
    else
    {
        Logger::error("Can't start server at " + ip + ":" + port);
        ui->pbStartStop->setChecked(true);
    }
}


void Dialog::loadConfig()
{
    ConfigManager::Instance()->Load("server.config");
    ui->leHost->setText(ConfigManager::Get("ip"));
    ui->lePort->setText(ConfigManager::Get("port"));
}


void Dialog::initializeLogging()
{
    QString timestamp = QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd-hh-mm-ss");
    _logFile.setFileName("votes-" + timestamp + ".txt");
    _logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream log(&_logFile);
    QString message("Votes logs in votes-%1.txt");

    Logger::assignList(ui->lwLog);
    Logger::info(message.arg(timestamp));
}


void Dialog::onAddUserToGui(QString name)
{
    ui->lwClients->addItem(name);
    Logger::success(QTime::currentTime().toString()+" "+name+" has joined");
}


void Dialog::onRemoveUserFromGui(QString name)
{
    for (int i = 0; i < ui->lwClients->count(); ++i)
        if (ui->lwClients->item(i)->text() == name)
        {
            ui->lwClients->takeItem(i);
            Logger::success(QTime::currentTime().toString()+" "+name+" has left");
            break;
        }
}


void Dialog::onMessageToGui(QString message, QString from)
{
    Logger::info(QTime::currentTime().toString()+" message from "+from+": "+message);
}


void Dialog::on_pbStartStop_toggled(bool checked)
{
    if (!checked) {
        Logger::success("Server stopped at " + _serv->serverAddress().toString() +
                 ":" + QString::number(_serv->serverPort()));
        _serv->close();
        ui->pbStartStop->setText("Start server");
    }
    else
    {
        this->startServer();
    }
}


void Dialog::onVoteUp(QString category, QString code, QString filename, QString peerName) {
    _votes.insert(code, filename);

    if(!_usedCodes.contains(code))
        _usedCodes.insert(code, QVector<QString>());

    if (!_usedCodes[code].contains(category))
        _usedCodes[code].push_back(category);

    Logger::debug("Vote " + category + " " + code + " " + filename);

    QTextStream log(&_logFile);
    log << QDateTime::currentDateTimeUtc().toString("dd.MM.yy hh:mm:ss") << ", " << peerName << ", " << code << ", " << filename << endl;
    _logFile.flush();
}

bool Dialog::isCodeAlreadyUsed(QString category, QString code) {
    bool haveKey = _usedCodes.contains(code);
    if (!haveKey)
        return false;

    bool voteCategory = _usedCodes[code].contains(category);
    if (!voteCategory)
        return false;

    return true;
}

void Dialog::on_pushButton_clicked()
{

}
