#include "logger.h"
#include "pch.h"

const QColor colorError = Qt::darkRed;
const QColor colorSuccess = Qt::darkGreen;
const QColor colorInfo = Qt::darkGray;
const QColor colorDebug = Qt::darkBlue;
const QColor colorWarning =   QColor::fromHsl(34, 1, 0.35);

Logger::Logger()
{ }

void Logger::logItem(const QString message, QColor color) const
{
    auto timestamp = QTime::currentTime().toString();
    target->addItem(timestamp + " " + message);

    int lastItemIndex = target->count() - 1;
    target->item(lastItemIndex)->setTextColor(color);
    target->scrollToBottom();
}

Logger * Logger::instance()
{
    static Logger* instance_ = new Logger();
    return instance_;
}

void Logger::assignList(QListWidget *widget)
{
    Logger::instance()->target = widget;
}

void Logger::success(const QString message)
{
    instance()->logItem(message, colorSuccess);
}

void Logger::error(const QString message)
{
    instance()->logItem(message, colorError);
}

void Logger::info(const QString message)
{
    instance()->logItem(message, colorInfo);
}

void Logger::debug(const QString message)
{
    instance()->logItem(message, colorDebug);
}

void Logger::warning(const QString message)
{
    instance()->logItem(message, colorWarning);
}
