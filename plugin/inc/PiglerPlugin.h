#include "AknIndicatorPlugin.h"
#include "PiglerServer.h"

struct TNotificationItem
{
	TInt uid;
	TBuf<64> appName;
	TBuf<256> text;
	CGulIcon *icon;
	TBool removeOnTap;
};


struct TNotificationApp
{
	TInt secureId;
	TBuf<64> appName;
	TInt lastTappedItem;
	TInt lastMissedItem;
};

class TUidNotificationMap;

NONSHARABLE_CLASS(PiglerPlugin) : public CAknIndicatorPlugin
{
public:
	~PiglerPlugin();
	static PiglerPlugin* NewL();

	virtual void HandleIndicatorTapL(const TInt aUid);
	virtual HBufC* TextL(const TInt aUid, TInt& aTextType);
	virtual const CGulIcon* IconL(const TInt aUid);

	TInt InitApp(TPiglerMessage aMessage, const TInt secureId);
	TInt SetItem(TPiglerMessage aMessage);
	TInt RemoveItem(TPiglerMessage aMessage);
	TInt RemoveItems(TPiglerMessage aMessage);
	TInt GetLastTappedAppItem(TPiglerMessage aMessage);
	TInt SetRemoveItemOnTap(TPiglerMessage aMessage);
	TInt SetItemIcon(TPiglerMessage aMessage, HBufC8* aIcon);

private:
	TInt getItemIdx(const TInt aUid);
	CArrayFixFlat<TNotificationItem> *iItems;
	CArrayFixFlat<TNotificationApp> *iApps;
	CPiglerServer *iServer;
	PiglerPlugin();
	void ConstructL();
};
