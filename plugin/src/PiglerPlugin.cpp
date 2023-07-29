#include <mw/gulicon.h>
#include <eikenv.h>
#include "PiglerPlugin.h"
#include <PiglerPlugin.mbg>

NONSHARABLE_CLASS(TUidNotificationMap)
{
public:
	TUidNotificationMap(const TInt uid, TNotificationItem item) :
		iUid(uid), iItem(item)
	{
	}
	const TInt iUid;
	TNotificationItem iItem;
};

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
	delete iItemsMap;
	iItemsMap = NULL;
}

PiglerPlugin* PiglerPlugin::NewL()
{
	PiglerPlugin* self = new (ELeave) PiglerPlugin;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

_LIT(KRequestsServerName, "PiglerServer");
//_LIT(KResponseQueueName, "PiglerQueue");
void PiglerPlugin::ConstructL()
{
	iItemsMap = new (ELeave) CArrayFixFlat<TUidNotificationMap>(5);
	iApps = new (ELeave) CArrayFixFlat<TNotificationApp>(5);
	iServer = new (ELeave) CPiglerServer(this);
	iServer->StartL(KRequestsServerName);
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
			TNotificationItem item = iItemsMap->At(idx).iItem;
			// проверка на изменение уведомления из другой проги
			if (item.appName.Compare(aMessage.appName) != 0) {
				return KErrAccessDenied;
			}
			item.text = aMessage.text;
			UpdateL(idx);
			return KErrNone;
		}
		return KErrNotFound;
	}
	TNotificationItem item;
	item.appName = aMessage.appName;
	item.text = aMessage.text;
	iNextItem = item;
	// сначала добавляем айтем в статус панельку чтобы получить уид, а потом изменяем его
	TInt uid;
	TRAP(uid, uid = AddStatusPanelItemL());
	if (uid < 0) {
		return uid;
	}
	TRAP_IGNORE(iItemsMap->AppendL(TUidNotificationMap(uid, item)));
	TRAP_IGNORE(UpdateL(uid));
	return uid;
}

TInt PiglerPlugin::RemoveItem(TPiglerMessage aMessage)
{
	TInt idx = getItemIdx(aMessage.uid);
	if (idx != -1) {
		// проверка на изменение уведомления из другой проги
		if (iItemsMap->At(idx).iItem.appName.Compare(aMessage.appName) != 0) {
			return KErrAccessDenied;
		}
		RemoveStatusPanelItem(idx);
		iItemsMap->Delete(idx);
	    iItemsMap->Compress();
	    return KErrNone;
	}
	return KErrNotFound;
}

TInt PiglerPlugin::RemoveItems(TPiglerMessage aMessage)
{
	for (TInt i = iItemsMap->Count()-1; i > 0; i--) {
		TUidNotificationMap map = iItemsMap->At(i);
		if (map.iItem.appName.Compare(aMessage.appName) == 0) {
			iItemsMap->Delete(i);
		}
	}
    iItemsMap->Compress();
    return KErrNone;
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
	TNotificationItem item = iItemsMap->At(idx).iItem;
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
		TNotificationItem item = iItemsMap->At(idx).iItem;
		for (TInt i = 0; i < iApps->Count(); i++) {
			TNotificationApp app = iApps->At(i);
			if (app.appName.Compare(item.appName) == 0) {
				app.lastTappedItem = aUid;
				break;
			}
		}
		if (item.removeOnTap) {
			RemoveStatusPanelItem(aUid);
			iItemsMap->Delete(idx);
		    iItemsMap->Compress();
		}
	} else {
		// айтема нет в списке поэтому просто удаляем
		RemoveStatusPanelItem(aUid);
	}
}

TInt PiglerPlugin::getItemIdx(const TInt aUid)
{
	for (TInt i = 0; i < iItemsMap->Count(); i++) {
		TUidNotificationMap map = iItemsMap->At(i);
		if (map.iUid == aUid) {
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
		return iItemsMap->At(idx).iItem.text.AllocL();
	}
	return NULL;
}

const CGulIcon* PiglerPlugin::IconL(const TInt aUid)
{
	// TODO: icon changing
	// размер иконки разный на Belle Refresh и Belle FP1: ~54 и 68
	// TODO: сделать скейлинг или делать иконки меньшим размером
	_LIT(KMBMFile, "c:\\resource\\apps\\piglerplugin.mbm");
	return CEikonEnv::Static()->CreateIconL(KMBMFile, EMbmPiglerpluginIcon, EMbmPiglerpluginMask);
}
