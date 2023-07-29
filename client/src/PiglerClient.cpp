#include "PiglerClient.h"

_LIT(KRequestsServerName, "PiglerServer");
TInt PiglerClient::Connect()
{
    return CreateSession(KRequestsServerName, TVersion(1, 0, 0));
}

TInt PiglerClient::InitApp(TBuf<64> aAppName)
{
	iAppName = aAppName;
	TPiglerMessage message;
	message.appName = aAppName;
    return this->SendMessage(EInitApp, message);
}

TInt PiglerClient::SetItem(const TInt aUid, TBuf<128> aText)
{
	TPiglerMessage message;
	message.uid = aUid;
	message.appName = iAppName;
	message.text = aText;
    return this->SendMessage(ESetItem, message);
}

TInt PiglerClient::RemoveItem(const TInt aUid)
{
	TPiglerMessage message;
	message.uid = aUid;
	message.appName = iAppName;
    return this->SendMessage(ERemoveItem, message);
}

TInt PiglerClient::RemoveAppItems()
{
	TPiglerMessage message;
	message.appName = iAppName;
    return this->SendMessage(ERemoveAppItems, message);
}

TInt PiglerClient::GetLastTappedAppItem()
{
	TPiglerMessage message;
	message.appName = iAppName;
    return this->SendMessage(EGetLastTappedAppItem, message);
}

TInt PiglerClient::SetRemoveItemOnTap(const TInt aUid, const TBool aRemove)
{
	TPiglerMessage message;
	message.appName = iAppName;
	message.uid = aUid;
	message.remove = aRemove;
    return this->SendMessage(EGetLastTappedAppItem, message);
}

TInt PiglerClient::SendMessage(TInt function, const TPiglerMessage aMessage)
{
    TPckg<TPiglerMessage> data(aMessage);
    TIpcArgs args(&data);
    return SendReceive(function, args);
}
