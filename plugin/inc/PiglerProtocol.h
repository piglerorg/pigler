#ifndef PIGLERPROTOCOL
#define PIGLERPROTOCOL
#include <e32base.h>

enum TPiglerCommands {
	EInitApp = 1,
	ESetItem,
	ERemoveItem,
	ERemoveAppItems,
	EGetAppItems,
	EGetLastTappedAppItem,
	ESetRemoveItemOnTap,
	ESetItemIcon,
	EHandleTap,
	EGetApiVersion,
	EGetItem,
	ESetItemSettings
};

struct TPiglerMessage
{
	TInt uid;
	TBuf<64> appName;
	TBuf<256> text;
	TBool remove;
};

#endif
