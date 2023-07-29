#include "PiglerAPI.h"
#include "PiglerClient.h"

TInt PiglerAPI::Init(TBuf<64> name)
{
	iClient = new PiglerClient;
	TInt err = iClient->Connect();
	if (err == KErrNone) {
		return iClient->InitApp(name);
	}
	return err;
}

TInt PiglerAPI::SetNotification(TInt uid, TBuf<128> text)
{
	return iClient->SetItem(uid, text);
}

TInt PiglerAPI::RemoveNotification(TInt uid)
{
	return iClient->RemoveItem(uid);
}

TInt PiglerAPI::RemoveAllNotifications()
{
	return iClient->RemoveAppItems();
}

TInt PiglerAPI::GetLastTappedNotification() {
	return iClient->GetLastTappedAppItem();
}

TInt PiglerAPI::SetRemoveNotificationOnTap(TInt uid, TBool remove)
{
	return iClient->SetRemoveItemOnTap(uid, remove);
}

void PiglerAPI::Close()
{
	iClient->Close();
}
