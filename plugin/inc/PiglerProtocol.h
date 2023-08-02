#ifndef PIGLERPROTOCOL
#define PIGLERPROTOCOL
#include <e32base.h>

enum TPiglerCommands {
	EInitApp = 1,
	ESetItem,
	ERemoveItem,
	ERemoveAppItems, // deprecated, use SetItemSettings
	EGetAppItems,
	EGetLastTappedAppItem,
	ESetRemoveItemOnTap,
	ESetItemIcon,
	EHandleTap, // client side
	EGetAPIVersion,
	EGetItem,
	ESetLaunchOnTap
};

struct TPiglerMessage
{
	TInt uid;
	TBuf<64> appName;
	TBuf<256> text;
	TInt argument; // removeOnTap or appId
};

#endif
