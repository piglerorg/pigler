/**
 * Copyright (c) 2023-2024 Arman Jussupgaliyev
 * Copyright (c) 2023 Mathew (curoviyxru)
 */

#include "PiglerAPI.h"

#include "PiglerTapServer.h"
#include "PiglerProtocol.h"
#include <random.h>
#include "PiglerUids.hrh"
#include "AknSmallIndicator.h"

PiglerAPI::PiglerAPI() :
	RSessionBase(), iServer(new CPiglerTapServer), iConnected(EFalse), iAppId(0), iRandom(EFalse)
{
}

PiglerAPI::~PiglerAPI()
{
	Close();
	delete iServer;
}

void PiglerAPI::SetTapHandler(IPiglerTapHandler *handler)
{
	iServer->SetHandler(handler);
}

void PiglerAPI::SetAppId(TInt aAppId)
{
	iAppId = aAppId;
}

TInt PiglerAPI::Init(TBuf<64> aAppName)
{
	TInt err = Connect();
	if (err == KErrNone) {
		iAppName = aAppName;
		TPiglerMessage message;
		message.appName = aAppName;
		message.argument = iAppId;
		
		TBuf<128> serverName(_L("PiglerHandler_"));
		serverName.Append(aAppName);
		iServer->StartL(serverName);
		
		return SendMessage(EInitApp, message);
	}
	return err;
}

TInt PiglerAPI::Init()
{
	iRandom = ETrue;
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

TInt PiglerAPI::SetRemoveOnTap(TInt aUid, TBool aRemove)
{
	if (!iConnected) return KErrNotReady;
	TPiglerMessage message;
	message.appName = iAppName;
	message.uid = aUid;
	message.argument = aRemove;
	return SendMessage(ESetRemoveItemOnTap, message);
}

TInt PiglerAPI::SetLaunchAppOnTap(TInt aUid, TBool aLaunchOnTap)
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

TInt PiglerAPI::GetNotificationsCount()
{
	if (!iConnected) return KErrNotReady;
	TPiglerMessage message;
	message.appName = iAppName;
	return SendMessage(EGetNotificationsCount, message);
}

TInt PiglerAPI::GetMaxNotificationsCount()
{
	if (!iConnected) return KErrNotReady;
	return SendReceive(EGetAPIVersion);
}

TInt PiglerAPI::GetGlobalNotificationsCount()
{
	if (!iConnected) return KErrNotReady;
	return SendReceive(EGetGlobalNotificationsCount);
}

TInt PiglerAPI::Remove()
{
	if (!iConnected) return KErrNotReady;
	TPiglerMessage message;
	message.appName = iAppName;
	return SendMessage(ERemoveApp, message);
}

TInt PiglerAPI::GetBitmapSize()
{
	if (!iConnected) return KErrNotReady;
	return SendReceive(EGetBitmapSize);
}

TInt PiglerAPI::StartAnnaServer()
{
	TRAPD(err, 
		CAknSmallIndicator* smallIndicator = CAknSmallIndicator::NewLC(TUid::Uid(KPiglerStartIndicatorUID));
		smallIndicator->SetIndicatorStateL(1);
		smallIndicator->SetIndicatorStateL(0);
		CleanupStack::PopAndDestroy(smallIndicator);
	);
	return err;
}

void PiglerAPI::Close()
{
	if (iConnected) {
		if (iRandom) {
			Remove();
		}
		RSessionBase::Close();
	}
	iServer->Cancel();
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
