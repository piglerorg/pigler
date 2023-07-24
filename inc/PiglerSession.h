#include <e32base.h>
#include "PiglerRequest.h"

class PiglerPlugin;

class PiglerSession: public CSession2
{
private:
	PiglerPlugin* plugin;

public:
	PiglerSession(PiglerPlugin* plugin);
	virtual void ServiceL(const RMessage2& message);
	virtual void ServiceError(const RMessage2& message, TInt error);
};
