#include <e32base.h>

#include "PiglerProtocol.h"

class PiglerClient : public RSessionBase
{
public:
    TInt Connect();
    TInt Init(TDesC aAppName);
    TInt SetNotification(TInt aUid, TDesC aText);
    TInt RemoveNotification(TInt aUid);
    TInt RemoveAllNotifications();
private:
    TDesC *iAppName;
    TInt SendMessage(TInt function, const TPiglerMessage aMessage);
};
