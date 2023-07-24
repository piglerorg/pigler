#include <AknIndicatorPlugin.h>

//NONSHARABLE_CLASS(PiglerPlugin) : public CAknIndicatorPlugin
class PiglerPlugin : public CAknIndicatorPlugin
	{
public:
	~PiglerPlugin();
	static PiglerPlugin* NewL();
	
	void HandleIndicatorTapL( const TInt aUid );
	HBufC* TextL( const TInt aUid, TInt& aTextType );
	const CGulIcon* IconL( const TInt aUid );

private:
	PiglerPlugin();
	void ConstructL();
	};
