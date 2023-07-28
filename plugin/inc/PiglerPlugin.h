#include <AknIndicatorPlugin.h>
#include "PiglerServer.h"

struct TNotificationItem
{
	TInt uid;
	TBuf<64> appName;
	TBuf<128> text;
};

class TUidNotificationMap;

class PiglerPlugin : public CAknIndicatorPlugin
{
public:
	~PiglerPlugin();
	static PiglerPlugin* NewL();

	void HandleIndicatorTapL(const TInt aUid);
	HBufC* TextL(const TInt aUid, TInt& aTextType);
	const CGulIcon* IconL(const TInt aUid);

	void InitApp(TPiglerMessage aMessage);
	TInt SetItem(TPiglerMessage aMessage);
	TInt RemoveItem(TPiglerMessage aMessage);
	TInt RemoveItems(TPiglerMessage aMessage);

private:
	TNotificationItem iNextItem;
	TInt getItemIdx(TInt aUid);
	CArrayFixFlat<TUidNotificationMap> *iItemsMap;
	TInt iAdded;
	CPiglerServer *iServer;
	PiglerPlugin();
	void ConstructL();
};
