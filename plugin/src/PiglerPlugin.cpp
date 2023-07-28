#include <mw/gulicon.h>
#include <eikenv.h>
#include "PiglerPlugin.h"
#include <PiglerPlugin.mbg>

class TUidNotificationMap
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
	delete iItemsMap;
	iItemsMap = NULL;
	this->~CAknIndicatorPlugin();
}

PiglerPlugin* PiglerPlugin::NewL()
{
	PiglerPlugin* self = new (ELeave) PiglerPlugin;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

_LIT(KRequestsServerName, "PiglerRequestsServer");
_LIT(KResponseQueueName, "PiglerResponseQueue");
void PiglerPlugin::ConstructL()
{
	iItemsMap = new (ELeave) CArrayFixFlat<TUidNotificationMap>(5);
	iServer = new (ELeave) CPiglerServer(this);
	iServer->StartL(KRequestsServerName);
}

void PiglerPlugin::NewApp(TDesC aAppName, TInt aId)
{
}


TInt PiglerPlugin::NewItem(TDesC aAppName, TDesC aText)
{
	TNotificationItem item;
	item.appName = aAppName;
	item.text = aText;
	iAdded++;
	iNextItem = item;
	return AddStatusPanelItemL();
}

TInt PiglerPlugin::UpdateItem(TDesC aAppName, TInt aUid, TDesC aText)
{
	TInt idx = getItemIdx(aUid);
	if (idx != -1) {
		TNotificationItem item = iItemsMap->At(idx).iItem;
		if(item.appName.Compare(aAppName) != 0) {
			return KErrAccessDenied;
		}
		item.text = aText;
		UpdateL(idx);
	    return KErrNone;
	}
	return KErrNotFound;
}

TInt PiglerPlugin::RemoveItem(TDesC aAppName, TInt aUid)
{
	TInt idx = getItemIdx(aUid);
	if (idx != -1) {
		if (iItemsMap->At(idx).iItem.appName.Compare(aAppName) != 0) {
			return KErrAccessDenied;
		}
		RemoveStatusPanelItem(idx);
		iItemsMap->Delete(idx);
	    iItemsMap->Compress();
	    return KErrNone;
	}
	return KErrNotFound;
}

TInt PiglerPlugin::RemoveItems(TDesC aAppName)
{
	for (TInt i = iItemsMap->Count()-1; i > 0; i--) {
		TUidNotificationMap map = iItemsMap->At(i);
		if (map.iItem.appName.Compare(aAppName) == 0) {
			iItemsMap->Delete(i);
		}
	}
    return KErrNone;
}

void PiglerPlugin::HandleIndicatorTapL(const TInt aUid)
{
	// TODO: add tap handling
	this->RemoveStatusPanelItem(aUid);
}

TInt PiglerPlugin::getItemIdx(TInt uid)
{
	for (TInt i = 0; i < iItemsMap->Count(); i++) {
		TUidNotificationMap map = iItemsMap->At(i);
		if (map.iUid == uid) {
			return i;
		}
	}
	return -1;
}

HBufC* PiglerPlugin::TextL(const TInt aUid, TInt& aTextType)
{
	if (iItemsMap == NULL) {
		iItemsMap = new CArrayFixFlat<TUidNotificationMap> (5);
	}
	TInt idx = getItemIdx(aUid);
	if (idx != -1) {
	    aTextType = EAknIndicatorPluginLinkText;
		return iItemsMap->At(idx).iItem.text.AllocL();
	} else if (iAdded > 0) {
	    aTextType = EAknIndicatorPluginLinkText;
		iAdded--;
		TRAP_IGNORE(iItemsMap->AppendL(TUidNotificationMap(aUid, iNextItem)));
		return iNextItem.text.AllocL();
	}
	return NULL;
}

const CGulIcon* PiglerPlugin::IconL(const TInt aUid)
{
	// TODO: icon
	//_LIT(KMBMFile, "c:\\resource\\apps\\piglerplugin.mbm");
	//return CEikonEnv::Static()->CreateIconL(KMBMFile, EMbmPiglerpluginIcon, EMbmPiglerpluginIcon_mask);
    return NULL;
}
