#include "configmanager.h"

ConfigManager::ConfigManager()
{
    // set default config
    config.insert("ip","localhost");
    config.insert("port", "1234");
    config.insert("images", "images");
}

ConfigManager *ConfigManager::Instance()
{
    static ConfigManager *instance = new ConfigManager();
    return instance;
}

void ConfigManager::Load(QString filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while(!file.atEnd()) {
            auto line = file.readLine();
            auto pair = line.split('=');
            auto name = pair[0].simplified();
            auto value = pair[1].simplified();
            config.insert(name,value);
        }
    }
    else
    {
        qDebug() << file.errorString();
    }
}

QString ConfigManager::Get(QString name)
{
    return Instance()->config.value(name);
}
