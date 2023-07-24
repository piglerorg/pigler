#include <AknIndicatorPlugin.h>
#include "PiglerRequest.h"

class TUidNotificationMap;

//NONSHARABLE_CLASS(PiglerPlugin) : public CAknIndicatorPlugin
class PiglerPlugin : public CAknIndicatorPlugin
{
public:
	~PiglerPlugin();
	static PiglerPlugin* NewL();
	
	void HandleIndicatorTapL( const TInt aUid );
	HBufC* TextL( const TInt aUid, TInt& aTextType );
	const CGulIcon* IconL( const TInt aUid );

    void NewItem( TPiglerNotification request );
	void UpdateItem( TPiglerNotification request );
    void RemoveItem( TPiglerNotification request );
	
private:
    TPiglerNotification iNextItem;
    TInt getItemIdx( TInt uid );
    CArrayFixFlat<TUidNotificationMap> *iItemsMap;
	PiglerPlugin();
	void ConstructL();
};
