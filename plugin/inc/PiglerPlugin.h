#include <AknIndicatorPlugin.h>
#include "PiglerServer.h"

struct TNotificationItem
{
	TInt uid;
	TBuf<64> appName;
	TBuf<256> text;
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

	void NewApp(TDesC aAppName, TInt aId);
	TInt NewItem(TDesC aAppName, TDesC aText);
	TInt UpdateItem(TDesC aAppName, TInt aUid, TDesC aText);
	TInt RemoveItem(TDesC aAppName, TInt aUid);
	TInt RemoveItems(TDesC aAppName);

private:
	TNotificationItem iNextItem;
	TInt getItemIdx(TInt aUid);
	CArrayFixFlat<TUidNotificationMap> *iItemsMap;
	TInt iAdded;
	CPiglerServer* iServer;
	PiglerPlugin();
	void ConstructL();
};
