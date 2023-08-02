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
	EHandleTap, // client side
	EGetAPIVersion,
	ESetLaunchOnTap,
	EGetItem
};

struct TPiglerMessage
{
	TInt uid;
	TBuf<64> appName;
	TBuf<256> text;
	TInt argument; // removeOnTap or appId
};

#endif
