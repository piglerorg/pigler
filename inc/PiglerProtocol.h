#ifndef PIGLERPROTOCOL
#define PIGLERPROTOCOL
#include <e32base.h>

struct TPiglerMessage
{
	TInt uid;
	TBuf<64> appName;
	TBuf<256> text;
};
#endif
