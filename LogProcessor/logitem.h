#ifndef LOGITEM_H
#define LOGITEM_H

#include <QtCore>

class LogItem {
public:
    QDateTime timestamp;
    QString filename;
    QString clientName;
    QString code;

    static QSharedPointer<LogItem> fromString(QString string);
};

#endif // LOGITEM_H
