#include <e32base.h>

class TPiglerMessage;
class CPiglerTapServer;

class IPiglerTapHandler {
public:
	virtual void handleTap(TInt uid, TBuf<64> appName, TBuf<256> text, TBool remove) = 0;
};

/**
 * Pigler Notifications API
 */
class PiglerAPI: private RSessionBase {
public:
	PiglerAPI();
	~PiglerAPI();
	
	/**
	 * Initializes API connection with specified app name
	 * 
	 * @param appName Application name
	 */
	TInt Init(TBuf<64> appName);
	
	/**
	 * Initializes API connection with random app name
	 * 
	 */
	TInt Init();
	
	/**
	 * Returns current app name
	 * 
	 */
	TBuf<64> GetAppName();
	
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
	TInt SetNotification(TInt uid, TBuf<256> text);
	
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
	 * 
	 * Returns removed items count
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
	 * 
	 * @param uid Notification UID
	 * @param remove
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
	 * 
	 * @param uid Notification UID
	 * @param iconBitmap Icon bitmap 68x68 in ARGB32 format
	 */
	TInt SetNotificationIcon(TInt uid, TPtrC8& iconBitmap);
	
	/**
	 * Closes API connection
	 */
	void Close();
	
	/**
	 * Sets current tap handler
	 * 
	 * @param handler Tap handler, ownership is not transferred
	 */
	void SetTapHandler(IPiglerTapHandler *handler);
private:
	CPiglerTapServer *server;
	TInt Connect();
	TInt SendMessage(TInt function, const TPiglerMessage aMessage);
	TBuf<64> iAppName;
};
