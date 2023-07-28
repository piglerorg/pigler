#ifndef PIGLERPROTOCOL
#define PIGLERPROTOCOL
#include <e32base.h>

struct TPiglerMessage
{
	TInt uid;
	TBuf<128> text;
	TBuf<64> appName;
};
#endif
