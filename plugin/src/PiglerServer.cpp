#include "PiglerServer.h"
#include "PiglerPlugin.h"

CPiglerServer::CPiglerServer(PiglerPlugin* plugin) :
	CServer2(EPriorityStandard), iPlugin(plugin)
{
}

CSession2* CPiglerServer::NewSessionL(const TVersion& version, const RMessage2& message) const
{
	return new (ELeave) CPiglerSession(iPlugin);
}

CPiglerSession::CPiglerSession(PiglerPlugin* aPlugin) :
	CSession2(), iPlugin(aPlugin)
{
}

void CPiglerSession::ServiceL(const RMessage2& aMessage)
{
	switch (aMessage.Function()) {
	case EInitApp:
	{
		iPlugin->InitApp(ReadMessage(aMessage), aMessage.SecureId().iId);
		aMessage.Complete(KErrNone);
	}
	break;
	case ESetItem:
	{
		aMessage.Complete(iPlugin->SetItem(ReadMessage(aMessage)));
	}
	break;
	case ERemoveItem:
	{
		aMessage.Complete(iPlugin->RemoveItem(ReadMessage(aMessage)));
	}
	break;
	case ERemoveAppItems:
	{
		aMessage.Complete(iPlugin->RemoveItems(ReadMessage(aMessage)));
	}
	break;
	case EGetLastTappedAppItem:
	{
		aMessage.Complete(iPlugin->GetLastTappedAppItem(ReadMessage(aMessage)));
	}
	break;
	case ESetRemoveItemOnTap:
	{
		aMessage.Complete(iPlugin->SetRemoveItemOnTap(ReadMessage(aMessage)));
	}
	break;
	default:
	{
		aMessage.Complete(KErrNotFound);
	}
	}
}

TPiglerMessage CPiglerSession::ReadMessage(const RMessage2& aMessage) {
	TPiglerMessage message;
	TPckg<TPiglerMessage> data(message);
	aMessage.ReadL(0, data);
	return message;
}

void CPiglerSession::ServiceError(const RMessage2& aMessage, TInt aError)
{
	aMessage.Complete(aError);
}
