#include <AknIndicatorPlugin.h>
#include "PiglerServer.h"

struct TNotificationItem
{
	TInt uid;
	TBuf<64> appName;
	TBuf<128> text;
	TBool removeOnTap;
};


struct TNotificationApp
{
	TInt secureId;
	TBuf<64> appName;
	TInt lastTappedItem;
};

class TUidNotificationMap;

NONSHARABLE_CLASS(PiglerPlugin) : public CAknIndicatorPlugin
{
public:
	~PiglerPlugin();
	static PiglerPlugin* NewL();

	void HandleIndicatorTapL(const TInt aUid);
	HBufC* TextL(const TInt aUid, TInt& aTextType);
	const CGulIcon* IconL(const TInt aUid);

	void InitApp(TPiglerMessage aMessage, const TInt secureId);
	TInt SetItem(TPiglerMessage aMessage);
	TInt RemoveItem(TPiglerMessage aMessage);
	TInt RemoveItems(TPiglerMessage aMessage);
	TInt GetLastTappedAppItem(TPiglerMessage aMessage);
	TInt SetRemoveItemOnTap(TPiglerMessage aMessage);

private:
	TNotificationItem iNextItem;
	TInt getItemIdx(const TInt aUid);
	CArrayFixFlat<TUidNotificationMap> *iItemsMap;
	CArrayFixFlat<TNotificationApp> *iApps;
	CPiglerServer *iServer;
	PiglerPlugin();
	void ConstructL();
};
