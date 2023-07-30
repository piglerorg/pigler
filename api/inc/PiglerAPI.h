#include <e32base.h>

class TPiglerMessage;

/**
 * Pigler Notifications API
 */
class PiglerAPI: private RSessionBase {
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
	 * Sets notification icon
	 * 
	 * Returns error code
	 * 
	 * KErrNotFound if there is no such item with that uid
	 * KErrAccessDenied if item was created by another app
	 * KErrUnderflow if icon or mask have the smaller size than 68x68
	 */
	TInt SetNotificationIcon(TInt uid, TPtrC8& icon, TPtrC8& mask);
	
	/**
	 * Closes API connection
	 */
	void Close();
private:
	TInt Connect();
	TInt SendMessage(TInt function, const TPiglerMessage aMessage);
    TBuf<64> iAppName;
};

/**
 * Pigler Notifications handler API
 */
class PiglerNotificationHandler {
public:
	virtual void HandleNotificationTap(TInt uid);
};
