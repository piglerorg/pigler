#include "PiglerSession.h"
#include "PiglerPlugin.h"

PiglerSession::PiglerSession(PiglerPlugin* plugin) : CSession2(), plugin(plugin)
{
}

void PiglerSession::ServiceL(const RMessage2& message)
{
	PiglerRequest request;
	TPckg<PiglerRequest> data(request);
	message.ReadL(0, data);
	message.Complete(plugin->AddStatusPanelItemL());
}

void PiglerSession::ServiceError(const RMessage2& message, TInt error)
{
	message.Complete(error);
}
