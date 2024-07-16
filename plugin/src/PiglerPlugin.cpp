#include "PiglerPlugin.h"
#include <mw/gulicon.h>
#include <eikenv.h>
#include <apaid.h>
#include <apgcli.h>

#if defined(PIGLER_ANNA) || defined(PIGLER_N97)
#include <avkon.hrh>
#include "AknSmallIndicator.h"
#include "aknstatuspanedatapublisher.h"
#include "PiglerUids.hrh"
#include "eikspane.h"
#include "eiksrvsp.h"
#endif

static PiglerPlugin *mainPlugin = NULL;

PiglerPlugin::PiglerPlugin()
{
}

PiglerPlugin::~PiglerPlugin()
{
	iServer->Cancel();
	delete iServer;
	iServer = NULL;
	delete iApps;
	iApps = NULL;
	delete iItems;
	iItems = NULL;
	mainPlugin = NULL;
}

PiglerPlugin* PiglerPlugin::NewL()
{
	PiglerPlugin* self = new (ELeave) PiglerPlugin;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	mainPlugin = self;
	return self;
}

void PiglerPlugin::ConstructL()
{
	iItems = new (ELeave) CArrayFixFlat<TNotificationItem>(5);
	iApps = new (ELeave) CArrayFixFlat<TNotificationApp>(5);
	iServer = new (ELeave) CPiglerServer(this);
	iServer->StartL(_L("PiglerServer"));
}

TInt PiglerPlugin::InitApp(TPiglerMessage aMessage, TInt aSecureId)
{
	for (TInt i = 0; i < iApps->Count(); i++) {
		TNotificationApp& app = iApps->At(i);
		if (app.appName.Compare(aMessage.appName) == 0) {
			app.secureId = aSecureId;
			TInt lastMissedItem = app.lastMissedItem;
			app.lastMissedItem = 0;
			return lastMissedItem; // прога уже есть в списке
		}
	}
	
	TNotificationApp app;
	app.secureId = aSecureId;
	app.appName = aMessage.appName;
	app.appId = aMessage.argument;
	app.lastTappedItem = 0;
	app.lastMissedItem = 0;
	app.itemsCount = 0;
	iApps->AppendL(app);
	
	return 0;
}

#if defined(PIGLER_ANNA) || defined(PIGLER_N97)
LOCAL_C void SetIndicatorState(TInt aUid, TInt aState) {
	CEikServStatusPane* ssp = static_cast<CEikServStatusPane*>(CEikStatusPaneBase::Current());
	CAknStatusPaneDataPublisher* publisher = ssp->iDataPublisher;
	publisher->SetIndicatorState(TUid::Uid(aUid), aState);
	publisher->PublishDataL();
	// TODO hide indicator icon
}
#endif

TInt PiglerPlugin::SetItem(TPiglerMessage aMessage)
{
	if (aMessage.uid != 0) {
		TInt idx = getItemIdx(aMessage.uid);
		if (idx != KErrNotFound) {
			TNotificationItem& item = iItems->At(idx);
			// проверка на изменение уведомления из другой проги
			if (item.appName.Compare(aMessage.appName) != 0) {
				return KErrAccessDenied;
			}
			item.text = aMessage.text;
			TRAPD(error, UpdateL(item.uid));
			return error;
		}
		return KErrNotFound;
	}
	
	TInt appIdx = getAppIdx(aMessage.appName);
	if (appIdx == KErrNotFound) {
		return KErrArgument;
	}
	
	TNotificationApp& app = iApps->At(appIdx);
	if (app.itemsCount >= KMaxNotificationsPerAppCount) {
		return KErrAlreadyExists;
	}
	
	TNotificationItem item;
	item.appName = aMessage.appName;
	item.text = aMessage.text;
	item.icon = NULL;
	// дефолтные настройки
	item.removeOnTap = ETrue;
	item.launchApp = ETrue;
	
	// сначала добавляем айтем в статус панельку чтобы получить уид, а потом изменяем его
	TInt uid = 0;
#if defined(PIGLER_ANNA) || defined(PIGLER_N97)
	if (getItemIdx(KPiglerIndicator1UID) == KErrNotFound) {
		uid = KPiglerIndicator1UID;
#ifdef KPiglerIndicator2UID
	} else if (getItemIdx(KPiglerIndicator2UID) == KErrNotFound) {
		uid = KPiglerIndicator2UID;
#endif
#ifdef KPiglerIndicator3UID
	} else if (getItemIdx(KPiglerIndicator3UID) == KErrNotFound) {
		uid = KPiglerIndicator3UID;
#endif
#ifdef KPiglerIndicator4UID
	} else if (getItemIdx(KPiglerIndicator4UID) == KErrNotFound) {
		uid = KPiglerIndicator4UID;
#endif
#ifdef KPiglerIndicator5UID
	} else if (getItemIdx(KPiglerIndicator5UID) == KErrNotFound) {
		uid = KPiglerIndicator5UID;
#endif
	} else {
		return KErrOverflow;
	}
	SetIndicatorState(uid, EAknIndicatorStateOn);
#else
		TRAPD(error, uid = AddStatusPanelItemL());
		if (error != KErrNone) {
			return error;
		}
#endif
	
	item.uid = uid;
	++app.itemsCount;
	
	TRAP_IGNORE(iItems->AppendL(item));
	TRAP_IGNORE(UpdateL(uid));
	return uid;
}

TInt PiglerPlugin::RemoveItem(TPiglerMessage aMessage)
{
	TInt idx = getItemIdx(aMessage.uid);
	if (idx != KErrNotFound) {
		// проверка на изменение уведомления из другой проги
		TNotificationItem& item = iItems->At(idx);
		if (item.appName.Compare(aMessage.appName) != 0) {
			return KErrAccessDenied;
		}
#if defined(PIGLER_ANNA) || defined(PIGLER_N97)
		SetIndicatorState(aMessage.uid, EAknIndicatorStateOff);
#else
		RemoveStatusPanelItem(aMessage.uid);
#endif
		delete item.icon;
		iItems->Delete(idx);
		iItems->Compress();
		
		TInt appIdx = getAppIdx(aMessage.appName);
		if (appIdx != KErrNotFound) {
			iApps->At(appIdx).itemsCount--;
		}
		
		return KErrNone;
	}
	return KErrNotFound;
}

TInt PiglerPlugin::RemoveItems(TPiglerMessage aMessage)
{
	TInt removed = 0;
	for (TInt i = iItems->Count()-1; i >= 0; i--) {
		TNotificationItem& item = iItems->At(i);
		if (item.appName.Compare(aMessage.appName) == 0) {
#if defined(PIGLER_ANNA) || defined(PIGLER_N97)
			SetIndicatorState(item.uid, EAknIndicatorStateOff);
#else
			RemoveStatusPanelItem(item.uid);
#endif
			delete item.icon;
			iItems->Delete(i);
			removed++;
		}
	}
	iItems->Compress();
	
	TInt appIdx = getAppIdx(aMessage.appName);
	if (appIdx != KErrNotFound) {
		iApps->At(appIdx).itemsCount = 0;
	}
	
	// возвращать колво удаленных уведов
	return removed;
}

TInt PiglerPlugin::GetLastTappedAppItem(TPiglerMessage aMessage)
{
	for (TInt i = 0; i < iApps->Count(); i++) {
		TNotificationApp& app = iApps->At(i);
		if (app.appName.Compare(aMessage.appName) == 0) {
			return app.lastTappedItem;
		}
	}
	// проги нет в списке
	return KErrNotFound;
}

TInt PiglerPlugin::SetRemoveItemOnTap(TPiglerMessage aMessage)
{
	TInt idx = getItemIdx(aMessage.uid);
	if (idx == KErrNotFound) {
		return KErrNotFound;
	}
	
	TNotificationItem& item = iItems->At(idx);
	// проверка на изменение уведомления из другой проги
	if (item.appName.Compare(aMessage.appName) != 0) {
		return KErrAccessDenied;
	}
	item.removeOnTap = aMessage.argument;
	return KErrNone;
}

TInt PiglerPlugin::GetItem(TPiglerMessage& aMessage)
{
	TInt idx = getItemIdx(aMessage.uid);
	if (idx == KErrNotFound) {
		return KErrNotFound;
	}
	TNotificationItem& item = iItems->At(idx);
	// проверка на изменение уведомления из другой проги
	if (item.appName.Compare(aMessage.appName) != 0) {
		return KErrAccessDenied;
	}
	aMessage.text = item.text;
	aMessage.argument = item.removeOnTap;
	return idx;
}

TInt PiglerPlugin::SetLaunchOnTap(TPiglerMessage aMessage)
{
	TInt idx = getItemIdx(aMessage.uid);
	if (idx == KErrNotFound) {
		return KErrNotFound;
	}
	TNotificationItem& item = iItems->At(idx);
	// проверка на изменение уведомления из другой проги
	if (item.appName.Compare(aMessage.appName) != 0) {
		return KErrAccessDenied;
	}
	item.launchApp = aMessage.argument;
	return KErrNone;
}

void LaunchApp(TInt aUid)
{
	TUid uid = {aUid};
	TApaAppInfo appInfo;
	RApaLsSession session;
	
	TInt result;
	
	result = session.Connect();
	
	if (result != KErrNone) {
		return;
	}
	
	result = session.GetAppInfo(appInfo, uid);
	
	if (result != KErrNone || appInfo.iUid != uid) {
		session.Close();
		return;
	}
	
	CApaCommandLine* cli = CApaCommandLine::NewL();
	cli->SetExecutableNameL(appInfo.iFullName);
	session.StartApp(*cli);
	delete cli;
	session.Close();
}

TBool NotifyApp(TNotificationItem item)
{
	CPiglerTapSession session;
	
	if (session.Connect(item.appName) != KErrNone) {
		return EFalse;
	}
	
	if (session.SendMessage(item.uid) != KErrNone) {
		session.Close();
		return EFalse;
	}
	
	session.Close();
	return ETrue;
}

void PiglerPlugin::HandleIndicatorTapL(const TInt aUid)
{
	TInt idx = getItemIdx(aUid);
	if (idx != KErrNotFound) {
		TNotificationItem& item = iItems->At(idx);
		for (TInt i = 0; i < iApps->Count(); i++) {
			TNotificationApp& app = iApps->At(i);
			if (app.appName.Compare(item.appName) == 0) {
				app.lastTappedItem = aUid;
				if (!NotifyApp(item)) {
					app.lastMissedItem = aUid;
				}
				if(item.launchApp) {
					LaunchApp(app.appId != 0 ? app.appId : app.secureId);
				}
				break;
			}
		}
		if (item.removeOnTap) {
#if defined(PIGLER_ANNA) || defined(PIGLER_N97)
			SetIndicatorState(aUid, EAknIndicatorStateOff);
#else
			RemoveStatusPanelItem(aUid);
#endif
			TInt appIdx = getAppIdx(item.appName);
			if (appIdx != KErrNotFound) {
				iApps->At(appIdx).itemsCount--;
			}

			delete item.icon;
			iItems->Delete(idx);
			iItems->Compress();
		}
	} else {
		// TODO override system indicator actions
#if defined(PIGLER_ANNA) || defined(PIGLER_N97)
		// айтема нет в списке поэтому просто удаляем
		SetIndicatorState(aUid, EAknIndicatorStateOff);
#else
		RemoveStatusPanelItem(aUid);
#endif
	}
}

TInt PiglerPlugin::getItemIdx(const TInt aUid)
{
	if (aUid == 0) {
		return KErrNotFound;
	}
	
	for (TInt i = 0; i < iItems->Count(); i++) {
		TNotificationItem item = iItems->At(i);
		if (item.uid == aUid) {
			return i;
		}
	}
	return KErrNotFound;
}

TInt PiglerPlugin::getAppIdx(TBuf<64> aAppName) {
	for (TInt i = 0; i < iApps->Count(); i++) {
		TNotificationApp& app = iApps->At(i);
		if (app.appName.Compare(aAppName) == 0) {
			return i;
		}
	}
	return KErrNotFound;
}

HBufC* PiglerPlugin::TextL(const TInt aUid, TInt& aTextType)
{
	TInt idx = getItemIdx(aUid);
	if (idx != KErrNotFound) {
		aTextType = EAknIndicatorPluginLinkText;
		return iItems->At(idx).text.AllocL();
	}
	return NULL;
}

TInt PiglerPlugin::SetItemIcon(TPiglerMessage aMessage, HBufC8* aIcon) 
{
	TInt idx = getItemIdx(aMessage.uid);
	if (idx == KErrNotFound) {
		return KErrNotFound;
	}
	TNotificationItem& item = iItems->At(idx);
	// проверка на изменение уведомления из другой проги
	if (item.appName.Compare(aMessage.appName) != 0) {
		return KErrAccessDenied;
	}
	
	TInt error;
	
	CFbsBitmap *icon = NULL;
	TRAP(error, icon = new (ELeave) CFbsBitmap);
	error = icon->Create(TSize(KBitmapDimension, KBitmapDimension), EColor16MA);
	
	if (error != KErrNone) {
		return error;
	}
	if (icon == NULL) {
		return KErrGeneral;
	}
	
	CFbsBitmap *mask = NULL;

	TInt amount = aIcon->Length();
	if (amount < icon->DataSize()) {
		delete icon;
		return KErrUnderflow;
	}
	
	if (amount > icon->DataSize()) {
		delete icon;
		return KErrOverflow;
	}
	
	icon->BeginDataAccess();
	
	TUint8* data = (TUint8*) icon->DataAddress();
	const TUint8* from = aIcon->Ptr();
	
	memcpy(data, from, amount);

	icon->EndDataAccess();
#if defined(PIGLER_ANNA) || defined(PIGLER_N97)
	TRAP_IGNORE(
		mask = new (ELeave) CFbsBitmap;
		mask->Create(TSize(KBitmapDimension, KBitmapDimension), EGray256);
		icon->BeginDataAccess();
		mask->BeginDataAccess();
		TUint8* src = (TUint8*)icon->DataAddress();
		TUint8* dest = (TUint8*)mask->DataAddress();
		
		for (TInt i = 0; i < KBitmapDimension*KBitmapDimension; i++) {
			dest[i] = src[i*4+3];
		}
		
		icon->EndDataAccess(ETrue);
		mask->EndDataAccess();
	);
#endif
	icon->Compress();
	
	CGulIcon* gulIcon = NULL;
	TRAP(error, gulIcon = CGulIcon::NewL(icon, mask));
	if (error != KErrNone) {
		delete icon;
		delete mask;
		return error;
	}
	
	delete item.icon;
	
	item.icon = gulIcon;
	TRAP_IGNORE(UpdateL(item.uid));
	return KErrNone;
}

const CGulIcon* PiglerPlugin::IconL(const TInt aUid)
{
	TInt idx = getItemIdx(aUid);
	if (idx != KErrNotFound) {
		return iItems->At(idx).icon;
	}
	return NULL;
}

// v3
TInt PiglerPlugin::GetNotificationsCount(TPiglerMessage aMessage)
{
	TInt count(0);
	for (TInt i = 0; i < iItems->Count(); i++) {
		TNotificationItem& item = iItems->At(i);
		if(item.appName.Compare(aMessage.appName) == 0) {
			count++;
		}
	}
	return count;
}

// v4
TInt PiglerPlugin::GetGlobalNotificationsCount()
{
	return iItems->Count();
}

TInt PiglerPlugin::RemoveApp(TPiglerMessage aMessage)
{
	for (TInt i = 0; i < iApps->Count(); i++) {
		TNotificationApp& app = iApps->At(i);
		if (app.appName.Compare(aMessage.appName) == 0) {
			iApps->Delete(i);
			iApps->Compress();
			RemoveItems(aMessage);
			return KErrNone;
		}
	}
	return KErrNone;
}


#if defined(PIGLER_ANNA) || defined(PIGLER_N97)
PiglerPlugin2::PiglerPlugin2()
{
}

PiglerPlugin2::~PiglerPlugin2()
{
}

PiglerPlugin2* PiglerPlugin2::NewL()
{
	PiglerPlugin2* self = new (ELeave) PiglerPlugin2;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void PiglerPlugin2::ConstructL()
{
}

void PiglerPlugin2::HandleIndicatorTapL(const TInt aUid)
{
	if (mainPlugin) {
		return mainPlugin->HandleIndicatorTapL(aUid);
	}
}

HBufC* PiglerPlugin2::TextL(const TInt aUid, TInt& aTextType)
{
	if (mainPlugin) {
		return mainPlugin->TextL(aUid, aTextType);
	}
	return NULL;
}

const CGulIcon* PiglerPlugin2::IconL(const TInt aUid)
{
	if (mainPlugin) {
		return mainPlugin->IconL(aUid);
	}
	return NULL;
}
#endif
