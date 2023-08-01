#include <ecom\implementationproxy.h>
#include "PiglerPlugin.h"
#include "PiglerUids.hrh"

const TImplementationProxy ImplementationTable[] =
{
	IMPLEMENTATION_PROXY_ENTRY(KPiglerPluginUID, PiglerPlugin::NewL)
};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
}
