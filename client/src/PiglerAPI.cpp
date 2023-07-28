#include "PiglerAPI.h"
#include "PiglerClient.h"

TInt PiglerAPI::Init(TBuf<64> name)
{
	iClient = new PiglerClient;
	TInt err = iClient->Connect();
	if(err < 0) {
		return err;
	}
	return iClient->Init(name);
}

TInt PiglerAPI::SetNotification(TInt uid, TBuf<128> text, TInt iconBitmap)
{
	return iClient->SetNotification(uid, text);
}

TInt PiglerAPI::RemoveNotification(TInt uid)
{
	return iClient->RemoveNotification(uid);
}

TInt PiglerAPI::RemoveAllNotifications()
{
	return iClient->RemoveAllNotifications();
}

void PiglerAPI::Close()
{
	iClient->Close();
}
