#include <mw/gulicon.h>
#include <eikenv.h>
#include "PiglerPlugin.h"
#include <PiglerPlugin.mbg>

//TODO: i will revork this today kek

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

void PiglerPlugin::InitApp(TPiglerMessage aMessage, TInt aSecureId)
{
	for (TInt i = 0; i < iApps->Count(); i++) {
		TNotificationApp app = iApps->At(i);
		if (app.appName.Compare(aMessage.appName) == 0) {
			return; // прога уже есть в списке
		}
	}
	TNotificationApp app;
	app.secureId = aSecureId;
	app.appName = aMessage.appName;
	iApps->AppendL(app);
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
	item.removeOnTap = aMessage.remove;
	
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
		if (iItems->At(idx).appName.Compare(aMessage.appName) != 0) {
			return KErrAccessDenied;
		}
		RemoveStatusPanelItem(aMessage.uid);
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
	if (idx == KErrNotFound) {
		return KErrNotFound;
	}
	TNotificationItem& item = iItems->At(idx);
	// проверка на изменение уведомления из другой проги
	if (item.appName.Compare(aMessage.appName) != 0) {
		return KErrAccessDenied;
	}
	item.removeOnTap = aMessage.remove;
	return KErrNone;
}

void PiglerPlugin::HandleIndicatorTapL(const TInt aUid)
{
	// TODO: add tap handling
	TInt idx = getItemIdx(aUid);
	if (idx != -1) {
		TNotificationItem item = iItems->At(idx);
		for (TInt i = 0; i < iApps->Count(); i++) {
			TNotificationApp& app = iApps->At(i);
			if (app.appName.Compare(item.appName) == 0) {
				app.lastTappedItem = aUid;
				break;
			}
		}
		if (item.removeOnTap) {
			RemoveStatusPanelItem(aUid);
			iItems->Delete(idx);
			iItems->Compress();
		}
	} else {
		// айтема нет в списке поэтому просто удаляем
		RemoveStatusPanelItem(aUid);
	}
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
		//TODO: could it be a memory leak?
		return iItems->At(idx).text.AllocL();
	}
	return NULL;
}

TInt PiglerPlugin::SetItemIcon(TPiglerMessage aMessage, TPtrC8 aIconPtr) 
{
	//TODO: memory leaks? 
	//TODO: do not allocate CFbsBitmap if already exists
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
	//EColor16M = BRG???
	error = icon->Create(TSize(68, 68), EColor16MA);
	
	if (error != KErrNone) {
		return error;
	}
	if (icon == NULL) {
		return KErrGeneral;
	}
	
	if (aIconPtr.Length() < icon->DataSize()) {
		return KErrUnderflow;
	}
	
	if (aIconPtr.Length() > icon->DataSize()) {
		return KErrOverflow;
	}
	
	icon->BeginDataAccess();
	
	TUint8* data = (TUint8*) icon->DataAddress();
	const TUint8* from = aIconPtr.Ptr();
	TInt amount = aIconPtr.Length();
	
	memcpy(data, from, amount);

	icon->EndDataAccess();
	icon->Compress();
	
	//TODO: free aMessage.icon and aMessage.mask
	
	CGulIcon* gulIcon = NULL;
	TRAP(error, gulIcon = CGulIcon::NewL(icon)); //, mask
	if (error != KErrNone) {
		return error;
	}
	
	item.icon = gulIcon;
	TRAP_IGNORE(UpdateL(item.uid));
	return KErrNone;
}

const CGulIcon* PiglerPlugin::IconL(const TInt aUid)
{
	// размер иконки разный на Belle Refresh и Belle FP1: ~54 и 68 (откуда инфа?)
	// >>откуда инфа, сравнил
	// TODO: сделать скейлинг или делать иконки меньшим размером (на клиенте)
	TInt idx = getItemIdx(aUid);
	if (idx != -1) {
		return iItems->At(idx).icon;
	}
	return NULL;
}
