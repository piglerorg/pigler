/**
 * Copyright (c) 2023-2024 Arman Jussupgaliyev
 * Copyright (c) 2023 Mathew (curoviyxru)
 */

#include "AknIndicatorPlugin.h"
#include "PiglerServer.h"

const TInt KPiglerAPIVersion = 4;
#ifdef PIGLER_N97
const TInt KMaxNotificationsCount = 2;
const TInt KMaxNotificationsPerAppCount = 1;
const TInt KBitmapDimension = 28;
#elif defined(PIGLER_ANNA)
const TInt KMaxNotificationsCount = 3;
const TInt KMaxNotificationsPerAppCount = 2;
const TInt KBitmapDimension = 28;
#else
const TInt KMaxNotificationsCount = 100;
const TInt KMaxNotificationsPerAppCount = 10;
const TInt KBitmapDimension = 68;
#endif

struct TNotificationItem
{
	TInt uid;
	TBuf<64> appName;
	TBuf<256> text;
	CGulIcon *icon;
	TBool removeOnTap;
	TBool launchApp;
};


struct TNotificationApp
{
	TInt secureId;
	TInt appId;
	TBuf<64> appName;
	TInt lastTappedItem;
	TInt lastMissedItem;
	TInt itemsCount;
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
	TInt GetItem(TPiglerMessage& aMessage);
	TInt SetLaunchOnTap(TPiglerMessage aMessage);
	TInt GetNotificationsCount(TPiglerMessage aMessage);
	TInt GetGlobalNotificationsCount();
	TInt RemoveApp(TPiglerMessage aMessage);

private:
	TInt getItemIdx(const TInt aUid);
	TInt getAppIdx(TBuf<64> aAppName);
	CArrayFixFlat<TNotificationItem> *iItems;
	CArrayFixFlat<TNotificationApp> *iApps;
	CPiglerServer *iServer;
	PiglerPlugin();
	void ConstructL();
};

#if defined(PIGLER_ANNA) || defined(PIGLER_N97)
NONSHARABLE_CLASS(PiglerPlugin2) : public CAknIndicatorPlugin
{
public:
	~PiglerPlugin2();
	static PiglerPlugin2* NewL();

	virtual void HandleIndicatorTapL(const TInt aUid);
	virtual HBufC* TextL(const TInt aUid, TInt& aTextType);
	virtual const CGulIcon* IconL(const TInt aUid);

private:
	PiglerPlugin2();
	void ConstructL();
};
#endif
