#include "PiglerPlugin.h"
#include <mw/gulicon.h>
#include "PiglerServer.h"

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

void PiglerPlugin::HandleIndicatorTapL( const TInt aUid )
	{
	
	}

_LIT(KTest, "Test Notification");

HBufC* PiglerPlugin::TextL( const TInt aUid, TInt& aTextType )
	{
	return KTest().AllocL();
	}

const CGulIcon* PiglerPlugin::IconL( const TInt aUid )
	{
	//return CGulIcon::NewL();
	return 0;
	}
