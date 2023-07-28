#include "PiglerClient.h"

_LIT(KRequestsServerName, "PiglerRequestsServer");

TInt PiglerClient::Connect()
{
    return CreateSession(KRequestsServerName, TVersion(1, 0, 0));
}

TInt PiglerClient::Init(TDesC aAppName) {
	*iAppName = aAppName;
	TPiglerMessage message;
	message.appName = aAppName;
    return this->SendMessage(1, message);
}

TInt PiglerClient::SetNotification(TInt aUid, TDesC aText) {
	TPiglerMessage message;
	message.uid = aUid;
	message.appName = *iAppName;
	message.text = aText;
    return this->SendMessage(2, message);
}

TInt PiglerClient::RemoveNotification(TInt aUid) {
	TPiglerMessage message;
	message.uid = aUid;
	message.appName = *iAppName;
    return this->SendMessage(3, message);
}
TInt PiglerClient::RemoveAllNotifications() {
	TPiglerMessage message;
	message.appName = *iAppName;
    return this->SendMessage(4, message);
}

TInt PiglerClient::SendMessage(TInt function, const TPiglerMessage aMessage)
{
    TPckg<TPiglerMessage> data(aMessage);
    TIpcArgs args(&data);
    return SendReceive(function, args);
}
