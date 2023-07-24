#include "PiglerPlugin.h"
#include <mw/gulicon.h>
#include "PiglerServer.h"
#include <PiglerPlugin.mbg>
#include <eikenv.h>

PiglerPlugin::PiglerPlugin()
{

}

PiglerPlugin::~PiglerPlugin()
{

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

}

void PiglerPlugin::UpdateItem(TPiglerNotification request)
{

}

void PiglerPlugin::RemoveItem(TPiglerNotification request)
{

}

void PiglerPlugin::HandleIndicatorTapL(const TInt aUid)
{
}

HBufC* PiglerPlugin::TextL(const TInt aUid, TInt& aTextType)
{
	aTextType = EAknIndicatorPluginLinkText;
	_LIT(KSomeText, "Some text\ndsa");
	return KSomeText().AllocL();
}

const CGulIcon* PiglerPlugin::IconL(const TInt aUid)
{
	_LIT(KMBMFile, "c:\\resource\\apps\\piglerplugin.mbm");
	return CEikonEnv::Static()->CreateIconL(KMBMFile, EMbmPiglerpluginIcon,
		EMbmPiglerpluginIcon_mask);
}
