#include <e32base.h>

class PiglerRequest
	{
public:
	TInt op;
	TInt id;
	TPtrC name;
	TPtrC icon;
	};

class PiglerPlugin;

class PiglerSession : public CSession2
	{
private:
	PiglerPlugin* plugin;
	
public:
	PiglerSession(PiglerPlugin* plugin);
	virtual void ServiceL(const RMessage2& message);
	virtual void ServiceError(const RMessage2& message, TInt error);
	};
