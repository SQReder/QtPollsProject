#include <io.h>
#include <QtCore>
#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QCryptographicHash>

#include "pollcodegenerator.h"

#define QT_NO_DEBUG_OUTPUT

QTextStream cin(stdin);
QTextStream cout(stdout);

static quint8 _CountToGenerate = 0;
static quint8 _HashLength = 10;
static quint8 _CheckCodeLength = 10;
static enum Action { aNothing, aEncode, aVerify } action = aEncode;
static PollCodeGenerator::CodeType codeType = PollCodeGenerator::ctTest;
static QByteArray eventTitle = QByteArray();


void print_help_and_exit() {
    const QString help =
            "Truthful code generator for qr polls\n"
            "\n"
            "Usage: pipe | qrcodegen [options]\n"
            "Options:\n"
            "    -h, --help                      Print this message and exit.\n"
            "    -v, --verify                    Validate input codes\n"
            "    -t, --type                      Set code type [test, master, event]\n"
            "                                    custom type require event title\n"
            "    -e, --event                     Set event title. Also set code type to event\n"
            "                                    if specified value of -t switcher ingnoring\n"
//            "   -g [N], --generate [N]          Count of items to generate\n"
            "\n"
            "Report bugs to <sqreder@gmail.com>";
    cout << qPrintable(help) << "\n";
    exit(0);
}


void parse_args(int argc, char *argv[]) {
    for(auto i = 1; i != argc; ++i) {
        QString arg(argv[i]);

        if(arg == "-h" || arg == "--help")
            print_help_and_exit();
        else if (arg == "-g" || arg == "--generate") {
            ++i;
            auto tmp = QString(argv[i]);
            _CountToGenerate = tmp.toInt();
        } else if (arg == "-v" || arg == "--verify") {
            action = aVerify;
        } else if (arg == "-t" || arg == "--type") {
            ++i;

            if (!eventTitle.isEmpty())
                continue;

            auto type = QString(argv[i]);
            if (type == "master")
                codeType = PollCodeGenerator::ctMaster;
            else if (type == "test")
                codeType = PollCodeGenerator::ctTest;
            else if (type == "event")
                codeType = PollCodeGenerator::ctEvent;
            else {
                qWarning() << "Error: unknown code type " << type;
                exit(1);
            }
        } else if (arg == "-e" || arg == "--event") {
             ++i;

            codeType = PollCodeGenerator::ctEvent;
            eventTitle = QByteArray(argv[i]);
        } else {
            qWarning() << "Error: Unknown argument " << arg;
            exit(1);
        }
    }
}


int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    PollCodeGenerator codeGen(_HashLength, _CheckCodeLength);
    if (codeType == PollCodeGenerator::ctEvent)
        codeGen.setEventTitle(eventTitle);

    if(_isatty(_fileno(stdin))) {
        print_help_and_exit();
    } else {
        forever {
            QByteArray line = cin.readLine().toLocal8Bit();
            if (line == "")
                break;

            switch (action) {
            case Action::aEncode:
                cout << codeGen.encodeString(codeType, line) << endl;
                break;
            case Action::aVerify:
                cout << qPrintable(codeGen.verify(line) ? "true" : "false") << endl;
                break;
            default:
                break;
            }

        }
    }

    return 0;
}
