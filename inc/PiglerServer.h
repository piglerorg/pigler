#include <e32base.h>

class PiglerPlugin;

class PiglerServer : public CServer2
	{
public:
	PiglerServer(PiglerPlugin* plugin);
	
private:
	PiglerPlugin* plugin;
	virtual CSession2* NewSessionL(const TVersion& version, const RMessage2& message) const;
	};
