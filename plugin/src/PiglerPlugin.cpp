#include <mw/gulicon.h>
#include <eikenv.h>
#include "PiglerPlugin.h"
#include <apaid.h>
#include <apgcli.h>

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
}

PiglerPlugin* PiglerPlugin::NewL()
{
	PiglerPlugin* self = new (ELeave) PiglerPlugin;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
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
	iApps->AppendL(app);
	
	return 0;
}

TInt PiglerPlugin::SetItem(TPiglerMessage aMessage)
{
	if (aMessage.uid != 0) {
		TInt idx = getItemIdx(aMessage.uid);
		if (idx != -1) {
			TNotificationItem& item = iItems->At(idx);
			// проверка на изменение уведомления из другой проги
			if (item.appName.Compare(aMessage.appName) != 0) {
				return KErrAccessDenied;
			}
			item.text = aMessage.text;
			UpdateL(item.uid);
			return KErrNone;
		}
		return KErrNotFound;
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
	TRAPD(error, uid = AddStatusPanelItemL());
	if (error != KErrNone) {
		return error;
	}
	
	item.uid = uid;
	
	TRAP_IGNORE(iItems->AppendL(item));
	TRAP_IGNORE(UpdateL(uid));
	return uid;
}

TInt PiglerPlugin::RemoveItem(TPiglerMessage aMessage)
{
	TInt idx = getItemIdx(aMessage.uid);
	if (idx != -1) {
		// проверка на изменение уведомления из другой проги
		TNotificationItem item = iItems->At(idx);
		if (item.appName.Compare(aMessage.appName) != 0) {
			return KErrAccessDenied;
		}
		RemoveStatusPanelItem(aMessage.uid);
		delete item.icon;
		iItems->Delete(idx);
		iItems->Compress();
		return KErrNone;
	}
	return KErrNotFound;
}

TInt PiglerPlugin::RemoveItems(TPiglerMessage aMessage)
{
	TInt removed = 0;
	for (TInt i = iItems->Count()-1; i >= 0; i--) {
		TNotificationItem item = iItems->At(i);
		if (item.appName.Compare(aMessage.appName) == 0) {
			RemoveStatusPanelItem(item.uid);
			delete item.icon;
			iItems->Delete(i);
			removed++;
		}
	}
	iItems->Compress();
	// возвращать колво удаленных уведов
	return removed;
}

TInt PiglerPlugin::GetLastTappedAppItem(TPiglerMessage aMessage)
{
	for (TInt i = 0; i < iApps->Count(); i++) {
		TNotificationApp app = iApps->At(i);
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
	if (idx == -1) {
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
	if (idx == -1) {
		return KErrNotFound;
	}
	TNotificationItem item = iItems->At(idx);
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
	if (idx == -1) {
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
		return;
	}
	
	CApaCommandLine* cli = CApaCommandLine::NewL();
	cli->SetExecutableNameL(appInfo.iFullName);
	session.StartApp(*cli);
	delete cli;
}

TBool NotifyApp(TNotificationItem item)
{
	CPiglerTapSession session;
	
	if (session.Connect(item.appName) != KErrNone) {
		return EFalse;
	}
	
	if (session.SendMessage(item.uid) != KErrNone) {
		return EFalse;
	}
	
	session.Close();
	return ETrue;
}

void PiglerPlugin::HandleIndicatorTapL(const TInt aUid)
{
	TInt idx = getItemIdx(aUid);
	if (idx != -1) {
		TNotificationItem item = iItems->At(idx);
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
			RemoveStatusPanelItem(aUid);
			delete item.icon;
			iItems->Delete(idx);
			iItems->Compress();
		}
	} else {
		// айтема нет в списке поэтому просто удаляем
		RemoveStatusPanelItem(aUid);
	}
	
	//TODO: show Java or native app if it is already opened
	//TODO: if not launch it and show it then
}

TInt PiglerPlugin::getItemIdx(const TInt aUid)
{
	if (aUid == 0) {
		return -1;
	}
	
	for (TInt i = 0; i < iItems->Count(); i++) {
		TNotificationItem item = iItems->At(i);
		if (item.uid == aUid) {
			return i;
		}
	}
	return -1;
}

HBufC* PiglerPlugin::TextL(const TInt aUid, TInt& aTextType)
{
	TInt idx = getItemIdx(aUid);
	if (idx != -1) {
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
	error = icon->Create(TSize(68, 68), EColor16MA);
	
	if (error != KErrNone) {
		return error;
	}
	if (icon == NULL) {
		return KErrGeneral;
	}

	TInt amount = aIcon->Length();
	if (amount < icon->DataSize()) {
		return KErrUnderflow;
	}
	
	if (amount > icon->DataSize()) {
		return KErrOverflow;
	}
	
	icon->BeginDataAccess();
	
	TUint8* data = (TUint8*) icon->DataAddress();
	const TUint8* from = aIcon->Ptr();
	
	memcpy(data, from, amount);

	icon->EndDataAccess();
	icon->Compress();
	
	CGulIcon* gulIcon = NULL;
	TRAP(error, gulIcon = CGulIcon::NewL(icon));
	if (error != KErrNone) {
		return error;
	}
	
	delete aIcon;
	delete item.icon;
	
	item.icon = gulIcon;
	TRAP_IGNORE(UpdateL(item.uid));
	return KErrNone;
}

const CGulIcon* PiglerPlugin::IconL(const TInt aUid)
{
	TInt idx = getItemIdx(aUid);
	if (idx != -1) {
		return iItems->At(idx).icon;
	}
	return NULL;
}
