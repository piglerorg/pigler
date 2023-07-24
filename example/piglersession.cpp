#include "piglersession.h"

_LIT(KServerName, "PiglerServer");

TInt PiglerSession::Connect()
{
    return CreateSession(KServerName, TVersion(1, 0, 0));
}

TInt PiglerSession::SendStruct(const PiglerRequest request)
{
    TPckg<PiglerRequest> data(request);
    TIpcArgs args(&data);
    return SendReceive(1337, args);
}
