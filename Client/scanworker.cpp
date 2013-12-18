#include "scanworker.h"
#include <QDebug>
using namespace zbar;


ScanWorker::ScanWorker(QObject *parent):
    QObject(parent)
{
    try {
        processor = QSharedPointer<Processor>(new Processor(true, "", true));
    } catch (Exception& e) {
        MessageBoxA(0, e.what(), nullptr, 0);
    }
}


ScanWorker::~ScanWorker() {
    processor->set_active(false);
    processor->set_visible(false);
}


void ScanWorker::InitializeVideo() {
    processor->set_active(true);
}

std::string ScanWorker::Scan(int timeout = Processor::FOREVER) {
    processor->set_visible(true);

    qDebug() << "scan";
    try {
        processor->process_one(timeout);
    } catch (ClosedError& e) {
        qDebug() << e.what();
    }

    auto result = processor->get_results();
    processor->set_visible(false);

    if (result.get_size() != 0) {
        auto str = std::string();
        for(auto it = result.symbol_begin(); it != result.symbol_end(); ++it) {
            auto sym = *it;
            auto type = sym.get_type();
            if (type != ZBAR_QRCODE) {
                continue;
            }
            str = sym.get_data();

            qDebug() << qPrintable(str.c_str());
        }
        return str;
    } else {
        return "";
    }

    return "";
}

void ScanWorker::work() {
    InitializeVideo();

    forever {
        auto msg = Scan();
        if (msg != "")
            emit codeScanned(QString(msg.c_str()));
    }
}
