#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QString>

enum ProtocolCommand: quint8{
    comAuthReq      = 1,
    comAuthSuccess  = 2,
	comCodeVerify      = 100,
	comCodeVerified    = 101,
	comCodeNotVerified = 102,
	comVoteUp          = 103,
	comAuthError        = 201,
	comCodeAlreadyUsed  = 202,
    comServerTurningOff = 255
};

const QString ReadableProtocolCommand(const ProtocolCommand& command);

#endif // PROTOCOL_H
