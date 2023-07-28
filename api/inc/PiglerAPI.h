#include <e32base.h>

class PiglerClient;

/**
 * Pigler Notifications API
 */
class PiglerAPI {
public:
	/**
	 * Initializes API connection
	 * Returns error code
	 * @param appName Application name
	 */
	TInt Init(TBuf<64> appName);
	
	/**
	 * Adds or updates notification
	 * 
	 * Returns UID or error code
	 * @param uid Notification UID, 0 - to create
	 * @param text Notification text
	 * @param iconBitmap Notification icon bitmap
	 */
	TInt SetNotification(TInt uid, TBuf<128> text, TInt iconBitmap); // здесь будет инт пока я не придумаю как отправлять иконки
	
	/**
	 * Removes notification by its UID
	 * 
	 * Returns error code
	 * @param uid Notification UID
	 */
	TInt RemoveNotification(TInt uid);
	
	/**
	 * Removes all created notifications
	 * 
	 * Returns error code
	 */
	TInt RemoveAllNotifications();
	
	/**
	 * Closes API connection
	 */
	void Close();
private:
	PiglerClient *iClient;
};
