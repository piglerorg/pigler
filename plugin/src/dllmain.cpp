#include <ecom\implementationproxy.h>
#include "PiglerPlugin.h"
#include "PiglerUids.hrh"

const TImplementationProxy ImplementationTable[] =
{
	IMPLEMENTATION_PROXY_ENTRY(KPiglerImplementation1UID, PiglerPlugin::NewL),
#if defined(PIGLER_ANNA) || defined(PIGLER_N97)
#ifdef KPiglerImplementation2UID
	IMPLEMENTATION_PROXY_ENTRY(KPiglerImplementation2UID, PiglerPlugin2::NewL),
#endif
#ifdef KPiglerImplementation3UID
	IMPLEMENTATION_PROXY_ENTRY(KPiglerImplementation3UID, PiglerPlugin2::NewL),
#endif
#ifdef KPiglerImplementation4UID
	IMPLEMENTATION_PROXY_ENTRY(KPiglerImplementation4UID, PiglerPlugin2::NewL),
#endif
#endif
};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
}
