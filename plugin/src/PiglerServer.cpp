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
	case 1: // Init
	{
		TPiglerMessage message;
		TPckg<TPiglerMessage> data(message);
		aMessage.ReadL(0, data);
		iPlugin->NewApp(message.appName, aMessage.SecureId().iId);
		aMessage.Complete(KErrNone);
	}
	break;
	case 2: // SetNotification
	{
		TPiglerMessage message;
		TPckg<TPiglerMessage> data(message);
		aMessage.ReadL(0, data);
		TInt uid = message.uid;
		if(message.uid == 0) {
			aMessage.Complete(iPlugin->NewItem(message.appName, message.text));
		} else {
			aMessage.Complete(iPlugin->UpdateItem(message.appName, uid, message.text));
		}
	}
	break;
	case 3: // RemoveNotification
	{
		TPiglerMessage message;
		TPckg<TPiglerMessage> data(message);
		aMessage.ReadL(0, data);
		aMessage.Complete(iPlugin->RemoveItem(message.appName, message.uid));
	}
	break;
	case 4: // RemoveNotifications
	{
		TPiglerMessage message;
		TPckg<TPiglerMessage> data(message);
		aMessage.ReadL(0, data);
		aMessage.Complete(iPlugin->RemoveItems(message.appName));
	}
	break;
	default:
	{
		aMessage.Complete(KErrNotFound);
	}
	}
}

void CPiglerSession::ServiceError(const RMessage2& aMessage, TInt aError)
{
	aMessage.Complete(aError);
}
