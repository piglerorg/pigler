#include "PiglerClient.h"

_LIT(KServerName, "PiglerServer");

TInt PiglerClient::Connect()
{
    return CreateSession(KServerName, TVersion(1, 0, 0));
}

TInt PiglerClient::Post(TPiglerNotification notification) {
    return this->SendStruct(1, notification);
}

TInt PiglerClient::Remove(TPiglerNotification notification) {
    return this->SendStruct(2, notification);
}

TInt PiglerClient::Update(TPiglerNotification notification) {
    return this->SendStruct(3, notification);
}

TInt PiglerClient::SendStruct(TInt function, const TPiglerNotification notification)
{
    TPckg<TPiglerNotification> data(notification);
    TIpcArgs args(&data);
    return SendReceive(function, args);
}
