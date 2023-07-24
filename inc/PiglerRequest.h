#include <e32base.h>
class PiglerRequest
{
public:
    TInt uid;
    HBufC* text;
};
enum TPiglerRequestFunction
{
    EPostItem,
    ERemoveItem,
    EUpdate
};
