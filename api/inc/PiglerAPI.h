#include <e32base.h>

class PiglerClient;

/**
 * Pigler Notifications API
 */
class PiglerAPI {
public:
	/**
	 * Initializes API connection
	 * 
	 * @param appName Application name
	 */
	TInt Init(TBuf<64> appName);
	
	/**
	 * Adds or updates notification
	 * 
	 * Returns UID or error code
	 * 
	 * KErrNotFound if there is no such item with that uid
	 * KErrAccessDenied if item was created by another app
	 * 
	 * @param uid Notification UID, 0 - to create
	 * @param text Notification text
	 */
	TInt SetNotification(TInt uid, TBuf<128> text);
	
	/**
	 * Removes notification by its UID
	 * 
	 * Returns error code
	 * 
	 * KErrNotFound if there is no such item with that uid
	 * KErrAccessDenied if item was created by another app
	 * 
	 * @param uid Notification UID
	 */
	TInt RemoveNotification(TInt uid);
	
	/**
	 * Removes all notifications created by this app
	 */
	TInt RemoveAllNotifications();
	
	/**
	 * Gets last tapped notification
	 * 
	 * Returns UID or error code
	 * 
	 * KErrNotFound if app is not initialized
	 * KErrNone if no notification has been tapped yet
	 */
	TInt GetLastTappedNotification();
	
	/**
	 * Marks that notification needs to be removed on tap
	 * 
	 * Returns error code
	 * 
	 * KErrNotFound if there is no such item with that uid
	 * KErrAccessDenied if item was created by another app
	 */
	TInt SetRemoveNotificationOnTap(TInt uid, TBool remove);
	
	/**
	 * Closes API connection
	 */
	void Close();
private:
	PiglerClient *iClient;
};
