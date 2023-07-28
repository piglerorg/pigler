#include <PiglerApi.h>

int main() 
{
	PiglerApi *api = new PiglerApi;
	_LIT(KAppName, "Example");
	_LIT(KText, "My test notification\nHello world!");
	api->Init(KAppName);
	TInt uid = api->SetNotification(0, KText, NULL);
//	api->RemoveNotification(uid);
//	api->RemoveAllNotifications();
	api->Close();
}
