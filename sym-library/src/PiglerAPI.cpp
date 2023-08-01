#include "PiglerAPI.h"

#include "PiglerProtocol.h"
#include <random.h>

PiglerAPI::~PiglerAPI()
{
	Close();
}

TInt PiglerAPI::Init(TBuf<64> aAppName)
{
	TInt err = Connect();
	if (err == KErrNone) {
		iAppName = aAppName;
		TPiglerMessage message;
		message.appName = aAppName;
		return this->SendMessage(EInitApp, message);
	}
	return err;
}

TInt PiglerAPI::Init()
{
	TBuf8<64> random;
	random.SetLength(64);
	TRandom::RandomL(random);
	
	TBuf<64> appName;
	appName.Copy(random);
	
	return Init(appName);
}

TBuf<64> PiglerAPI::GetAppName()
{
	return iAppName;
}

TInt PiglerAPI::SetNotification(TInt aUid, TBuf<256> aText)
{
	TPiglerMessage message;
	message.uid = aUid;
	message.appName = iAppName;
	message.text = aText;
	return this->SendMessage(ESetItem, message);
}

TInt PiglerAPI::RemoveNotification(TInt aUid)
{
	TPiglerMessage message;
	message.uid = aUid;
	message.appName = iAppName;
	return this->SendMessage(ERemoveItem, message);
}

TInt PiglerAPI::RemoveAllNotifications()
{
	TPiglerMessage message;
	message.appName = iAppName;
	return this->SendMessage(ERemoveAppItems, message);
}

TInt PiglerAPI::GetLastTappedNotification()
{
	TPiglerMessage message;
	message.appName = iAppName;
	return this->SendMessage(EGetLastTappedAppItem, message);
}

TInt PiglerAPI::SetRemoveNotificationOnTap(TInt aUid, TBool aRemove)
{
	TPiglerMessage message;
	message.appName = iAppName;
	message.uid = aUid;
	message.remove = aRemove;
	return this->SendMessage(ESetRemoveItemOnTap, message);
}

TInt PiglerAPI::SetNotificationIcon(TInt aUid, TPtrC8& aIconBitmap)
{
	TPiglerMessage message;
	message.appName = iAppName;
	message.uid = aUid;
	TPckg<TPiglerMessage> data(message);
	TIpcArgs args(&data, &aIconBitmap);
	return SendReceive(ESetItemIcon, args);
}

void PiglerAPI::Close()
{
	RSessionBase::Close();
}

TInt PiglerAPI::Connect()
{
	return CreateSession(_L("PiglerServer"), TVersion(1, 0, 0));
}

TInt PiglerAPI::SendMessage(TInt function, const TPiglerMessage aMessage)
{
	TPckg<TPiglerMessage> data(aMessage);
	TIpcArgs args(&data);
	return SendReceive(function, args);
}