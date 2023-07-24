#include <e32base.h>
#include "PiglerRequest.h"

class PiglerClient : public RSessionBase
{
public:
    TInt Connect();
    TInt Post(HBufC* text);
    TInt Remove(const TInt uid);
    TInt Update(const TInt uid);
private:
    TInt Send();
    TInt SendStruct(const TInt function, const PiglerRequest request);
};
