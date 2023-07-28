#include <PiglerApi.h>

int main() 
{
	PiglerApi *api = new PiglerApi;
	TBuf<64> appName(_L("Example"));
	TBuf<128> text(_L("My test notification\nHello world!"));
	if(api->Init(appName) == KErrNone) {
		TInt uid = api->SetNotification(0, text, NULL);
//		api->RemoveNotification(uid);
//		api->RemoveAllNotifications();
		api->Close();
	}
}
