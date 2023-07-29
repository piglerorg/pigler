#include <mw/gulicon.h>
#include <eikenv.h>
#include "PiglerPlugin.h"
#include <PiglerPlugin.mbg>

//TODO: i will revork this today kek

//TODO: remove it, what is this
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

void PiglerPlugin::ConstructL()
{
	iItemsMap = new (ELeave) CArrayFixFlat<TUidNotificationMap>(5);
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
			TNotificationItem item = iItemsMap->At(idx).iItem;
			// проверка на изменение уведомления из другой проги
			if (item.appName.Compare(aMessage.appName) != 0) {
				return KErrAccessDenied;
			}
			iItemsMap->At(idx).iItem.text = aMessage.text;
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
    TInt error;
	TInt uid;
	TRAP(error, uid = AddStatusPanelItemL());
	if (error != KErrNone) {
		return error;
	}
	
	item.uid = uid;
	//TODO: remove it, ???
    iNextItem = item;
	
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
		RemoveStatusPanelItem(aMessage.uid);
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
	iItemsMap->At(idx).iItem.removeOnTap = aMessage.remove;
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
    if (aUid == 0) {
        return -1;
    }
    
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
	    //TODO: could it be a memory leak?
		return iItemsMap->At(idx).iItem.text.AllocL();
	}
	return NULL;
}

TInt PiglerPlugin::SetIcon(TPiglerIconMessage aMessage) 
{
    //TODO: memory leaks? 
    //TODO: do not allocate CFbsBitmap if already exists
    TInt idx = getItemIdx(aMessage.uid);
    if (idx == KErrNotFound) {
        return KErrNotFound;
    }
    TNotificationItem item = iItemsMap->At(idx).iItem;
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
    
    if (aMessage.icon.Length() < icon->DataSize()) {
        return KErrUnderflow;
    }
    
    if (aMessage.icon.Length() > icon->DataSize()) {
        return KErrOverflow;
    }
    
    icon->BeginDataAccess();
    
    TUint8* data = (TUint8*) icon->DataAddress();
    const TUint8* from = aMessage.icon.Ptr();
    TInt amount = aMessage.icon.Length();
    
    memcpy(data, from, amount);

    icon->EndDataAccess();
    icon->Compress();
    
    //TODO: remove masks
    
//    CFbsBitmap *mask = NULL;
//    TRAP(error, mask = new (ELeave) CFbsBitmap);
//    error = mask->Create(TSize(68, 68), EGray256);
//    
//    if (error != KErrNone) {
//        return error;
//    }
//    if (mask == NULL) {
//        return KErrGeneral;
//    }
//    
//    if (aMessage.mask.Length() < mask->DataSize()) {
//        return KErrUnderflow;
//    }
//    
//    if (aMessage.mask.Length() > mask->DataSize()) {
//        return KErrOverflow;
//    }
//    
//    mask->BeginDataAccess();
//    
//    data = (TUint8*) mask->DataAddress();
//    from = aMessage.mask.Ptr();
//    amount = aMessage.mask.Length();
//    
//    memcpy(data, from, amount);
//
//    mask->EndDataAccess();
//    mask->Compress();
    
    //TODO: free aMessage.icon and aMessage.mask
    
    CGulIcon* gulIcon = NULL;
    TRAP(error, gulIcon = CGulIcon::NewL(icon)); //, mask
    if (error != KErrNone) {
        gulIcon = NULL;
        return error;
    }
    
    iItemsMap->At(idx).iItem.icon = gulIcon;
    TRAP_IGNORE(UpdateL(item.uid));
    return KErrNone;
}

const CGulIcon* PiglerPlugin::IconL(const TInt aUid)
{
	// TODO: icon changing
	// размер иконки разный на Belle Refresh и Belle FP1: ~54 и 68 (откуда инфа?)
	// TODO: сделать скейлинг или делать иконки меньшим размером (на клиенте)
	TInt idx = getItemIdx(aUid);
	if (idx != -1) {
	    return iItemsMap->At(idx).iItem.icon;
	}
	return NULL;
}
