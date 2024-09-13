/**
 * Copyright (c) 2023-2024 Arman Jussupgaliyev
 * Copyright (c) 2023 Mathew (curoviyxru)
 */

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
		if (handler) {
			handler->HandleTap(ReadMessage(aMessage));
		}
		
		aMessage.Complete(KErrNone);
		break;
	}
	default:
	{
		aMessage.Complete(KErrNotFound);
	}
	}
}

TInt CPiglerTapSession::ReadMessage(const RMessage2& aMessage) {
	TInt message;
	TPckg<TInt> data(message);
	aMessage.ReadL(0, data);
	return message;
}

void CPiglerTapSession::ServiceError(const RMessage2& aMessage, TInt aError)
{
	aMessage.Complete(aError);
}
