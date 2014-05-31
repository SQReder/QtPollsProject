#pragma once
#include "pch.h"

class ConfigManager
{
    ConfigManager();
    QMap<QString, QString> config;

public:
    static ConfigManager* Instance();

    void Load(QString filname);
    static QString Get(QString name);
};
