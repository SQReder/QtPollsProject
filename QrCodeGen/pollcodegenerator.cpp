#include "pollcodegenerator.h"

PollCodeGenerator::PollCodeGenerator(quint8 codePartLength, quint8 checkCodeLength):
    _codePartLength(codePartLength),
    _checkCodeLength(checkCodeLength)
{ }

QByteArray PollCodeGenerator::encodeString(PollCodeGenerator::CodeType type, QByteArray data)
{
    return encode_string(type, data, _eventTitle, _codePartLength, _checkCodeLength);
}

bool PollCodeGenerator::verify(QByteArray data)
{
    return verify(data, _eventTitle, _codePartLength, _checkCodeLength);
}

void PollCodeGenerator::setEventTitle(QByteArray eventTitle)
{
    _eventTitle = eventTitle;
}

QByteArray PollCodeGenerator::encode_string(CodeType codeType, QByteArray data, QByteArray salt, quint8 hash_length, quint8 checkCodeLength) {
    QByteArray type = (codeType == ctMaster)
            ? "m"
            : (codeType == ctEvent)
              ? "e"
              : "t";

    QCryptographicHash hasher(QCryptographicHash::Sha256);
    hasher.addData(data);
    auto code = hasher.result().toBase64(QByteArray::OmitTrailingEquals).mid(0, hash_length);
    code.replace("/", "-");
    hasher.reset();


    if (type == "e")
        hasher.addData(salt);
    hasher.addData(type);
    hasher.addData(code);
    auto hashedCodeWithSalt = hasher.result().toBase64(QByteArray::OmitTrailingEquals).mid(0, checkCodeLength);

    auto result = type + code + hashedCodeWithSalt;
    result.replace("/", "-");
    return result;
}

bool PollCodeGenerator::verify(QByteArray data, QByteArray salt, quint8 hash_length, quint8 check_length) {
    if (data.length() < 1 + hash_length + check_length)
        return false;

    auto type = data.left(1);
    auto code = data.mid(1, hash_length);
    auto expectedHash = data.mid(1 + hash_length, check_length);

    QCryptographicHash hasher(QCryptographicHash::Sha256);
    if (type == "e")
        hasher.addData(salt);
    hasher.addData(type);
    hasher.addData(code);
    auto hashedCodeWithSalt = hasher.result();
    auto hashInBase64Cropped = hashedCodeWithSalt.toBase64(QByteArray::OmitTrailingEquals).mid(0, check_length);
    hashInBase64Cropped.replace("/", "-");

    return hashInBase64Cropped == expectedHash;
}
