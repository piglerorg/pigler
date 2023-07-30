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
	ESetItemIcon
};

struct TPiglerMessage
{
	TInt uid;
	TBuf<64> appName;
	TBuf<128> text;
	TBool remove;
};

struct TPiglerIconMessage
{
	TInt uid;
	TBuf<64> appName;
	TPtrC8 icon;
	TPtrC8 mask;
};

#endif
