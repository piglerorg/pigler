#ifndef PIGLERREQUEST_H
#define PIGLERREQUEST_H
#include <e32base.h>
struct TPiglerNotification
{
    TInt uid;
    TBuf<128> text;
};
#endif
