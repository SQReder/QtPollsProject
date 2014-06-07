#include "logitem.h"

QSharedPointer<LogItem> LogItem::fromString(QString string)
{
    LogItem* result = nullptr;

    QStringList tokenz = string.split(',');
    if (tokenz.count() == 4) {
        result = new LogItem();
        result->timestamp = QDateTime::fromString(tokenz[0].simplified(),"dd.MM.yy hh:mm:ss");
        result->clientName = tokenz[1];
        result->code = tokenz[2];
        result->filename = tokenz[3];
    }

    return QSharedPointer<LogItem>(result);
}
