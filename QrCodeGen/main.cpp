//#include <stdio.h>
#include <io.h>
#include <QtCore>
#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QCryptographicHash>

#define QT_NO_DEBUG_OUTPUT

QTextStream cin(stdin);
QTextStream cout(stdout);

static unsigned _CountToGenerate = 0;
static unsigned _HashLength = 10;
static enum Action { aNothing, aEncode, aVerify } action = aEncode;


void print_help_and_exit() {
    const QString help =
            "Truthful code generator for qr polls\n"
            "\n"
            "Usage: pipe | qrcodegen [options]\n"
            "Options:\n"
            "   -h, --help                      Print this message and exit.\n"
            "   -v, --verify                    Validate input codes\n"
//            "   -g [N], --generate [N]          Count of items to generate\n"
            "\n"
            "Report bugs to <sqreder@gmail.com>";
    cout << qPrintable(help) << "\n";
    exit(0);
}


void parse_args(int argc, char *argv[]) {
    for(auto i = 1; i != argc; ++i) {
        QString arg(argv[i]);
        //qDebug() << qPrintable(arg);
        if(arg == "-h" || arg == "--help")
            print_help_and_exit();
        else if (arg == "-g" || arg == "--generate") {
            ++i;
            auto tmp = QString(argv[i]);
            _CountToGenerate = tmp.toInt();
        } else if (arg == "-v" || arg == "-verify") {
            action = aVerify;
        } else {
            qWarning() << "Error: Unknown argument " << arg;
            exit(1);
        }
    }
}

QByteArray encode_string(QByteArray data, unsigned hash_length) {
    auto partToHash = data.left(hash_length);

    QCryptographicHash hasher(QCryptographicHash::Sha256);
    hasher.addData(partToHash);
    auto partOne = hasher.result().toHex().mid(1,hash_length);

    hasher.reset();
    hasher.addData(partOne);

    auto partTwo = hasher.result().toHex();



    return partOne + partTwo.mid(1,hash_length);
}

bool verify(QByteArray data, unsigned hash_length) {
    auto partToHash = data.left(hash_length);
    auto expectedHash = data.mid(hash_length, hash_length);

    QCryptographicHash hasher(QCryptographicHash::Sha256);
    hasher.addData(partToHash);
    auto hash = hasher.result().toHex();
    auto hashPart = hash.mid(1,hash_length);

//    qDebug() << "data: " << data << "\t part: " << partToHash << "\t hash_part: " << hashPart << "\t expected: " << expectedHash << "\t hash" << hash;

    return hashPart == expectedHash;
}

void GenerateNCodes(unsigned __n)
{
    for (unsigned i = 0; i != __n; ++i)
        qDebug() << encode_string("abcd", _HashLength);
}

int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    if(_isatty(_fileno(stdin))) {
        print_help_and_exit();
    } else {
        forever {
            QByteArray line = cin.readLine().toLocal8Bit();
            if (line == "")
                break;

            switch (action) {
            case Action::aEncode:
                cout << qPrintable(encode_string(line, _HashLength))  << "\n";
                break;
            case Action::aVerify:
                cout << (verify(line, _HashLength) ? "true" : "false") << "\n";
                cout.flush();
                break;
            default:
                break;
            }

        }
    }

    return 0;
}
