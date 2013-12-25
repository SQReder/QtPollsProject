#include "protocol.h"

const QString ReadableProtocolCommand(const ProtocolCommand& command) {
    switch (command) {
    case comAuthRequest: return QString("Auth requested");
    case comAuthSuccess: return QString("Auth successfull");
    case comCodeVerify: return QString("Code verifying requested");
    case comCodeVerified: return QString("Code verifyed");
    case comCodeNotVerified: return QString("Code not verifyed");
    case comVoteUp: return QString("Vote up");
    case comAuthError: return QString("Auth error");
    case comCodeAlreadyUsed: return QString("Code already used");
    case comPing: return QString("Ping");
    case comServerTurningOff: return QString("Server goes off");
    }
    return QString();
}
