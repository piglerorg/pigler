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

void PiglerPlugin::InitApp(TPiglerMessage aMessage)
{
}


TInt PiglerPlugin::SetItem(TPiglerMessage aMessage)
{
	if(aMessage.uid != 0) {
		TInt idx = getItemIdx(aMessage.uid);
		if (idx != -1) {
			TNotificationItem item = iItemsMap->At(idx).iItem;
			if(item.appName.Compare(aMessage.appName) != 0) {
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
	iAdded++;
	iNextItem = item;
	TInt uid;
	TRAP(uid, uid = AddStatusPanelItemL());
	if(uid < 0) {
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
	TInt idx = getItemIdx(aUid);
	if (idx != -1) {
	    aTextType = EAknIndicatorPluginLinkText;
		return iItemsMap->At(idx).iItem.text.AllocL();
	}
	return NULL;
}

const CGulIcon* PiglerPlugin::IconL(const TInt aUid)
{
	// TODO: icon
	_LIT(KMBMFile, "c:\\resource\\apps\\piglerplugin.mbm");
	return CEikonEnv::Static()->CreateIconL(KMBMFile, EMbmPiglerpluginIcon, EMbmPiglerpluginMask);
}
