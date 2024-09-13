/**
 * Copyright (c) 2023-2024 Arman Jussupgaliyev
 * Copyright (c) 2023 Mathew (curoviyxru)
 */

#ifndef PIGLERSERVER
#define PIGLERSERVER
#include <e32base.h>
#include "PiglerProtocol.h"

NONSHARABLE_CLASS(CPiglerTapSession): private RSessionBase {
public:
	TInt Connect(TBuf<64> appName);
	TInt SendMessage(TInt uid);
	void Close();
};

class PiglerPlugin;

NONSHARABLE_CLASS(CPiglerServer): public CServer2
{
private:
	PiglerPlugin* iPlugin;
	CSession2* NewSessionL(const TVersion& version, const RMessage2& message) const;
	
public:
	CPiglerServer(PiglerPlugin* plugin);
};

NONSHARABLE_CLASS(CPiglerSession): public CSession2
{
private:
	PiglerPlugin* iPlugin;
	TPiglerMessage ReadMessage(const RMessage2& message);

public:
	CPiglerSession(PiglerPlugin* plugin);
	void ServiceL(const RMessage2& message);
	void ServiceError(const RMessage2& message, TInt error);
};
#endif
