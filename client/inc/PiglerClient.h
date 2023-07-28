#include <e32base.h>
#include "PiglerProtocol.h"

class PiglerClient : public RSessionBase
{
public:
    TInt Connect();
    TInt Init(TBuf<64> aAppName);
    TInt SetNotification(const TInt aUid, TBuf<128> aText);
    TInt RemoveNotification(const TInt aUid);
    TInt RemoveAllNotifications();
private:
    TBuf<64> iAppName;
    TInt SendMessage(TInt function, const TPiglerMessage aMessage);
};
