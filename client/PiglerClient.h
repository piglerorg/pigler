#ifndef PIGLERCLIENT_H
#define PIGLERCLIENT_H
#include <e32base.h>
#include "PiglerRequest.h"

class PiglerClient : public RSessionBase
{
public:
    TInt Connect();
    TInt Post(TPiglerNotification notification);
    TInt Remove(TPiglerNotification notification);
    TInt Update(TPiglerNotification notification);
private:
    TInt SendStruct(TInt function, const TPiglerNotification notification);
};
#endif
