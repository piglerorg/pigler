#include "PiglerAPI.h"
#include "PiglerProtocol.h"

class PiglerClient : public RSessionBase
{
public:
    TInt Connect();
    TInt InitApp(TBuf<64> aAppName);
    TInt SetItem(const TInt aUid, TBuf<128> aText);
    TInt RemoveItem(const TInt aUid);
    TInt RemoveAppItems();
    TInt GetLastTappedAppItem();
    TInt SetRemoveItemOnTap(const TInt aUid, const TBool aRemove);
private:
    TBuf<64> iAppName;
    TInt SendMessage(TInt function, const TPiglerMessage aMessage);
};

TInt PiglerAPI::Init(TBuf<64> name)
{
	iClient = new PiglerClient;
	TInt err = iClient->Connect();
	if (err == KErrNone) {
		return iClient->InitApp(name);
	}
	return err;
}

TInt PiglerAPI::SetNotification(TInt uid, TBuf<128> text)
{
	return iClient->SetItem(uid, text);
}

TInt PiglerAPI::RemoveNotification(TInt uid)
{
	return iClient->RemoveItem(uid);
}

TInt PiglerAPI::RemoveAllNotifications()
{
	return iClient->RemoveAppItems();
}

TInt PiglerAPI::GetLastTappedNotification() {
	return iClient->GetLastTappedAppItem();
}

TInt PiglerAPI::SetRemoveNotificationOnTap(TInt uid, TBool remove)
{
	return iClient->SetRemoveItemOnTap(uid, remove);
}

void PiglerAPI::Close()
{
	iClient->Close();
}

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
