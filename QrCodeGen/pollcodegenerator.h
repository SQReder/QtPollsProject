#pragma once
#include <QtCore>

class PollCodeGenerator
{
public:
    enum CodeType { ctTest, ctMaster, ctEvent };

private:
    QByteArray _eventTitle;
    quint8 _codePartLength;
    quint8 _checkCodeLength;

    static QByteArray encode_string(CodeType codeType, QByteArray data, QByteArray salt, quint8 hash_length, quint8 checkCodeLength);
    static bool verify(QByteArray data, QByteArray salt, quint8 hash_length, quint8 check_length);

public:
    PollCodeGenerator(quint8 hashLength = 7, quint8 checkCodeLength = 7);
    QByteArray encodeString(CodeType type, QByteArray data);
    bool verify(QByteArray data);

    void setEventTitle(QByteArray eventTitle);

};
