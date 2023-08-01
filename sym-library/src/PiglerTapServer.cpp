#include "PiglerTapServer.h"

#include "PiglerAPI.h"

CPiglerTapServer::CPiglerTapServer() :
	CServer2(EPriorityStandard), handler(NULL)
{
}

CSession2* CPiglerTapServer::NewSessionL(const TVersion& version, const RMessage2& message) const
{
	return new (ELeave) CPiglerTapSession(handler);
}

void CPiglerTapServer::SetHandler(IPiglerTapHandler *handler)
{
	this->handler = handler;
}

CPiglerTapSession::CPiglerTapSession(IPiglerTapHandler *handler) :
	CSession2(), handler(handler)
{
}

void CPiglerTapSession::ServiceL(const RMessage2& aMessage)
{
	switch (aMessage.Function()) {
	case EHandleTap:
	{
		if (!handler) {
			aMessage.Complete(KErrNotFound);
			return;
		}

		TPiglerMessage message = ReadMessage(aMessage);
		handler->handleTap(message.uid, message.appName, message.text, message.remove);
		aMessage.Complete(KErrNone);
		break;
	}
	default:
	{
		aMessage.Complete(KErrNotFound);
	}
	}
}

TPiglerMessage CPiglerTapSession::ReadMessage(const RMessage2& aMessage) {
	TPiglerMessage message;
	TPckg<TPiglerMessage> data(message);
	aMessage.ReadL(0, data);
	return message;
}

void CPiglerTapSession::ServiceError(const RMessage2& aMessage, TInt aError)
{
	aMessage.Complete(aError);
}
