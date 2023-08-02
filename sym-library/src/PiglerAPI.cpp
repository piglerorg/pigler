#include "PiglerAPI.h"

#include "PiglerTapServer.h"
#include "PiglerProtocol.h"
#include <random.h>

PiglerAPI::PiglerAPI() :
	RSessionBase(), server(new CPiglerTapServer)
{
	
}

PiglerAPI::~PiglerAPI()
{
	Close();
	delete server;
}

void PiglerAPI::SetTapHandler(IPiglerTapHandler *handler)
{
	server->SetHandler(handler);
}

TInt PiglerAPI::Init(TBuf<64> aAppName)
{
	TInt err = Connect();
	if (err == KErrNone) {
		iAppName = aAppName;
		TPiglerMessage message;
		message.appName = aAppName;
		
		TBuf<128> serverName(_L("PiglerHandler_"));
		serverName.Append(aAppName);
		server->StartL(serverName);
		
		return SendMessage(EInitApp, message);
	}
	return err;
}

TInt PiglerAPI::Init()
{
	TBuf8<64> random;
	random.SetLength(64);
	TRandom::RandomL(random);
	
	TUint8* ptr = (TUint8*) random.Ptr();
	for (TInt i = 0; i < random.Length(); ++i) {
		ptr[i] = (ptr[i] % 26) + 0x41;
	}
	
	TBuf<64> appName;
	appName.Copy(random);
	
	return Init(appName);
}

TInt PiglerAPI::GetAPIVersion()
{
	if (!iConnected) return KErrNotReady;
	return SendReceive(EGetAPIVersion);
}

TBuf<64> PiglerAPI::GetAppName()
{
	return iAppName;
}

TInt PiglerAPI::SetNotification(TInt aUid, TBuf<256> aText)
{
	if (!iConnected) return KErrNotReady;
	TPiglerMessage message;
	message.uid = aUid;
	message.appName = iAppName;
	message.text = aText;
	return SendMessage(ESetItem, message);
}

TInt PiglerAPI::RemoveNotification(TInt aUid)
{
	if (!iConnected) return KErrNotReady;
	TPiglerMessage message;
	message.uid = aUid;
	message.appName = iAppName;
	return SendMessage(ERemoveItem, message);
}

TInt PiglerAPI::RemoveAllNotifications()
{
	if (!iConnected) return KErrNotReady;
	TPiglerMessage message;
	message.appName = iAppName;
	return SendMessage(ERemoveAppItems, message);
}

TInt PiglerAPI::GetLastTappedNotification()
{
	if (!iConnected) return KErrNotReady;
	TPiglerMessage message;
	message.appName = iAppName;
	return SendMessage(EGetLastTappedAppItem, message);
}

TInt PiglerAPI::SetRemoveNotificationOnTap(TInt aUid, TBool aRemove)
{
	if (!iConnected) return KErrNotReady;
	TPiglerMessage message;
	message.appName = iAppName;
	message.uid = aUid;
	message.argument = aRemove;
	return SendMessage(ESetRemoveItemOnTap, message);
}

TInt PiglerAPI::SetLaunchAppOnNotificationTap(TInt aUid, TBool aLaunchOnTap)
{
	if (!iConnected) return KErrNotReady;
	TPiglerMessage message;
	message.appName = iAppName;
	message.uid = aUid;
	message.argument = aLaunchOnTap;
	return SendMessage(ESetRemoveItemOnTap, message);
}

TInt PiglerAPI::SetNotificationIcon(TInt aUid, TPtrC8& aIconBitmap)
{
	if (!iConnected) return KErrNotReady;
	TPiglerMessage message;
	message.appName = iAppName;
	message.uid = aUid;
	TPckg<TPiglerMessage> data(message);
	TIpcArgs args(&data, &aIconBitmap);
	return SendReceive(ESetItemIcon, args);
}

void PiglerAPI::Close()
{
	if (iConnected) {
		RSessionBase::Close();
	}
	server->Cancel();
}

TInt PiglerAPI::Connect()
{
	TInt res = CreateSession(_L("PiglerServer"), TVersion(1, 0, 0));
	if(res == KErrNone) {
		iConnected = ETrue;
	}
	return res;
}

TInt PiglerAPI::SendMessage(TInt function, const TPiglerMessage aMessage)
{
	TPckg<TPiglerMessage> data(aMessage);
	TIpcArgs args(&data);
	return SendReceive(function, args);
}
