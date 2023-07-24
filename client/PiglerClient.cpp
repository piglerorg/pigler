#include "PiglerClient.h"

_LIT(KServerName, "PiglerServer");

TInt PiglerClient::Connect()
{
    return CreateSession(KServerName, TVersion(1, 0, 0));
}

TInt PiglerClient::Post(TPiglerNotification& notification) {
    return this->SendStruct(EPostItem, notification);
}

TInt PiglerClient::Remove(TPiglerNotification notification) {
    return this->SendStruct(ERemoveItem, notification);
}

TInt PiglerClient::Update(TPiglerNotification notification) {
    return this->SendStruct(EUpdateItem, notification);
}

TInt PiglerClient::SendStruct(const TInt function, TPiglerNotification& notification)
{
    TPckg<TPiglerNotification> data(notification);
    TIpcArgs args(&data);
    return SendReceive(function, args);
}
