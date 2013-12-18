#pragma once

#include <QtCore>
#include "zbar.h"
using namespace zbar;

class ScanWorker: public QObject
{
    Q_OBJECT

    QSharedPointer<Processor> processor;
public:
    explicit ScanWorker(QObject *parent = nullptr);
    ~ScanWorker();

    void InitializeVideo();
    std::string Scan(int timeout);

public slots:
    void work();

signals:
    void finished();
    void codeScanned(QString code);
};
