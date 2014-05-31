#pragma once
#include "pch.h"

class QListWidget;

class Logger
{
    Logger();
    QListWidget* target;
    void logItem(const QString, QColor) const;
    static Logger* instance();
public:

    static void assignList(QListWidget*);

    static void success(const QString message);
    static void error(const QString message);
    static void info(const QString message);
    static void debug(const QString message);
    static void warning(const QString message);
};
