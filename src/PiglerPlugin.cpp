#include "PiglerPlugin.h"
#include <mw/gulicon.h>
#include "PiglerServer.h"
#include <PiglerPlugin.mbg>
#include <eikenv.h>

class TUidNotificationMap
{
public:
	TUidNotificationMap(const TInt uid, TPiglerNotification item) :
		iUid(uid), iItem(item)
	{
	}
	const TInt iUid;
	TPiglerNotification iItem;
};

PiglerPlugin::PiglerPlugin()
{
}

PiglerPlugin::~PiglerPlugin()
{
	//delete iItemsMap;
	//iItemsMap = NULL;
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
	iItemsMap = new (ELeave) CArrayFixFlat<TUidNotificationMap> (5);
	PiglerServer* server = new (ELeave) PiglerServer(this);
	CleanupStack::PushL(server);
	_LIT(KServerName, "PiglerServer");
	server->StartL(KServerName);
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(server);
}

void PiglerPlugin::NewItem(TPiglerNotification request)
{
	iAdded++;
	iNextItem = request;
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
	aTextType = EAknIndicatorPluginLinkText;
	if (iItemsMap == NULL) {
		iItemsMap = new CArrayFixFlat<TUidNotificationMap> (5);
	}
	TInt idx = getItemIdx(aUid);
	if (idx != -1) {
		return iItemsMap->At(idx).iItem.text.AllocL();
	} else if (iAdded > 0) {
		iAdded--;
		TRAP_IGNORE(iItemsMap->AppendL(TUidNotificationMap(aUid, iNextItem)));
		return iNextItem.text.AllocL();
	}
	_LIT(KSomeText, "Error");
	return KSomeText().AllocL();
}

const CGulIcon* PiglerPlugin::IconL(const TInt aUid)
{
	_LIT(KMBMFile, "c:\\resource\\apps\\piglerplugin.mbm");
	return CEikonEnv::Static()->CreateIconL(KMBMFile, EMbmPiglerpluginIcon, EMbmPiglerpluginIcon_mask);
}
