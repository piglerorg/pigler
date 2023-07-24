#include "PiglerClient.h"

_LIT(KServerName, "PiglerServer");

TInt PiglerClient::Connect()
{
    return CreateSession(KServerName, TVersion(1, 0, 0));
}

TInt PiglerClient::Post(HBufC* text) {
    PiglerRequest request;
    request.text = text;
    return this->SendStruct(EPostItem, request);
}

TInt PiglerClient::Remove(const TInt uid) {
    PiglerRequest request;
    request.uid = uid;
    return this->SendStruct(ERemoveItem, request);
}

TInt PiglerClient::Update(const TInt uid) {
    PiglerRequest request;
    request.uid = uid;
    return this->SendStruct(EUpdate, request);
}

TInt PiglerClient::SendStruct(const TInt function, const PiglerRequest request)
{
    TPckg<PiglerRequest> data(request);
    TIpcArgs args(&data);
    return SendReceive(function, args);
}
