#ifndef PIGLERSERVER
#define PIGLERSERVER
#include <e32base.h>
#include "PiglerProtocol.h"

class PiglerPlugin;

NONSHARABLE_CLASS(CPiglerServer): public CServer2
{
public:
	CPiglerServer(PiglerPlugin* plugin);

private:
	PiglerPlugin* iPlugin;
	CSession2* NewSessionL(const TVersion& version, const RMessage2& message) const;
};

NONSHARABLE_CLASS(CPiglerSession): public CSession2
{
private:
	PiglerPlugin* iPlugin;

public:
	CPiglerSession(PiglerPlugin* plugin);
	void ServiceL(const RMessage2& message);
	void ServiceError(const RMessage2& message, TInt error);
private:
	TPiglerMessage ReadMessage(const RMessage2& message);
	TPiglerMessage ReadIconMessage(const RMessage2& message);
};
#endif
