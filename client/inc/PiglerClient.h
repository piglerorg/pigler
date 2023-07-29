#include <e32base.h>
#include "PiglerProtocol.h"

class PiglerClient : public RSessionBase
{
public:
    TInt Connect();
    TInt InitApp(TBuf<64> aAppName);
    TInt SetItem(const TInt aUid, TBuf<128> aText);
    TInt RemoveItem(const TInt aUid);
    TInt RemoveAppItems();
    TInt GetLastTappedAppItem();
    TInt SetRemoveItemOnTap(const TInt aUid, const TBool aRemove);
private:
    TBuf<64> iAppName;
    TInt SendMessage(TInt function, const TPiglerMessage aMessage);
};
