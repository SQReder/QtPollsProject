#include "connectDialog.h"
#include "ui_connectDialog.h"

#include "scanwindow.h"

#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

QString picsPath;

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    _client = new ClientConnection(this);
    connect(_client, SIGNAL(AuthSuccess()), SLOT(onAuthSuccess()));
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::on_pbConnect_toggled(bool checked)
{
    if (checked) {
        if (ui->lePicsPath->text() == "") {
            onAddToLog("Select folder with pics", Qt::darkRed);
            return;
        }

        picsPath = ui->lePicsPath->text();
        if (picsPath.right(1) != "/")
            picsPath.append("/");

        _client->doConnectToHost(ui->leHost->text(), ui->sbPort->value());
        ui->pbConnect->setText("Process...");
    } else {
        _client->doDisconnect();
    }

}

void ConnectDialog::onAuthSuccess() {
    try {
        _scanWindow = new ScanWindow(_client ,this);
        _scanWindow->raise();
        _scanWindow->showMaximized();

        this->hide();
    } catch (std::exception &e){
        emit onAddToLog("blah blah");
        emit onAddToLog(QString(e.what()));
    }
}

void ConnectDialog::onAddToLog(QString text, QColor color)
{
    ui->lwLog->insertItem(0, QTime::currentTime().toString()+" "+text);
    ui->lwLog->item(0)->setTextColor(color);
}

void ConnectDialog::on_pbSelectFolder_clicked()
{
    QString path;
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    if(dialog.exec()) {
        path = dialog.selectedFiles()[0];
        ui->lePicsPath->setText(path);
    }
}
