#include "PiglerPlugin.h"
#include <mw/gulicon.h>
#include "PiglerServer.h"
#include <PiglerPlugin.mbg>
#include <eikenv.h>

class TUidNotificationMap {
public:
	TUidNotificationMap(const TInt uid, const TPiglerNotification notification) : iUid(uid), iItem(notification) {}
    const TInt iUid;
    const TPiglerNotification iItem;
};

PiglerPlugin::PiglerPlugin()
{
	iItemsMap = new (ELeave) CArrayFixFlat<TUidNotificationMap>(5);
}

PiglerPlugin::~PiglerPlugin()
{
	delete iItemsMap;
	iItemsMap = NULL;
}

PiglerPlugin* PiglerPlugin::NewL()
{
	PiglerPlugin* self = new (ELeave) PiglerPlugin();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void PiglerPlugin::ConstructL()
{
	PiglerServer* server = new (ELeave) PiglerServer(this);
	CleanupStack::PushL(server);
	_LIT(KServerName, "PiglerServer");
	server->StartL(KServerName);
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(server);
}

void PiglerPlugin::NewItem(TPiglerNotification request)
{
	*iNextItem = request;
}

void PiglerPlugin::UpdateItem(TPiglerNotification request)
{

}

void PiglerPlugin::RemoveItem(TPiglerNotification request)
{
    TInt idx = getItemIdx(request.uid); 
    if (idx != -1) {
    	iItemsMap->Delete(idx);
    }
    iItemsMap->Compress();
}

void PiglerPlugin::HandleIndicatorTapL(const TInt aUid)
{

}

TInt PiglerPlugin::getItemIdx(const TInt uid) {
	for(TInt i = 0; i < iItemsMap->Count(); i++) {
		TUidNotificationMap map = iItemsMap->At(i);
		if(map.iUid == uid) {
			return i;
		}
	}
	return -1;
}

HBufC* PiglerPlugin::TextL(const TInt aUid, TInt& aTextType)
{
    aTextType = EAknIndicatorPluginLinkText;
	TInt idx = getItemIdx(aUid); 
	if (idx != -1) {
		return iItemsMap->At(idx).iItem.text.AllocL();
	} else if(iNextItem != 0) {
		TPiglerNotification *item = iNextItem;
		TRAP_IGNORE(iItemsMap->AppendL(TUidNotificationMap(aUid, *item)));
		iNextItem = NULL;
		return item->text.AllocL();
	}
	_LIT(KSomeText, "Error");
	return KSomeText().AllocL();
}

const CGulIcon* PiglerPlugin::IconL(const TInt aUid)
{
	_LIT(KMBMFile, "c:\\resource\\apps\\piglerplugin.mbm");
	return CEikonEnv::Static()->CreateIconL(KMBMFile, EMbmPiglerpluginIcon, EMbmPiglerpluginIcon_mask);
}
