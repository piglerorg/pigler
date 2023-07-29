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
	ESetRemoveItemOnTap
};

struct TPiglerMessage
{
	TInt uid;
	TBuf<128> text;
	TBuf<64> appName;
	TBool remove;
};
#endif
