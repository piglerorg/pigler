#include "PiglerServer.h"
#include "PiglerSession.h"

PiglerServer::PiglerServer(PiglerPlugin* plugin) : CServer2(EPriorityStandard), plugin(plugin)
	{
	
	}

CSession2* PiglerServer::NewSessionL(const TVersion& version, const RMessage2& message) const
	{
	return new (ELeave) PiglerSession(plugin);
	}
