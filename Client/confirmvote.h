#ifndef CONFIRMVOTE_H
#define CONFIRMVOTE_H

#include <QDialog>

namespace Ui {
class ConfirmVote;
}

class ConfirmVote : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmVote(QWidget *parent = 0);
    ~ConfirmVote();

protected:
    void showEvent(QShowEvent *);

signals:
    void VoteConfirmed(QString filename);
    void VoteRejected();

public slots:
    void onOpenConfirm(QString filename);
    void onVoteConfirmed();
    void onVoteRejected();

private:
    Ui::ConfirmVote *ui;
    QString filename;
};

#endif // CONFIRMVOTE_H
