/**
 * Copyright (c) 2023-2024 Arman Jussupgaliyev
 * Copyright (c) 2023 Mathew (curoviyxru)
 */

#ifndef PIGLERAPI
#define PIGLERAPI

#include <e32base.h>

class TPiglerMessage;
class CPiglerTapServer;
class CAknIndicatorPlugin;

/**
 * Notification tap handler
 */
class IPiglerTapHandler
{
public:
	/**
	 * @param uid Notification UID
	 */
	virtual void HandleTap(TInt uid) = 0;
};

/**
 * Pigler Notifications API
 * 
 * @version v5
 */
class PiglerAPI: private RSessionBase
{
public:
	PiglerAPI();
	~PiglerAPI();
	
	/**
	 * Initializes API connection with specified app name
	 * 
	 * @return Connection error, or notification UID
	 * if app was started by tap handle event,
	 * otherwise KErrNone is returned
	 * 
	 * @param appName Application name
	 */
	TInt Init(TBuf<64> appName);
	
	/**
	 * Initializes API connection with random app name
	 * 
	 * Don't use if you need notification callbacks! <br>
	 * All notifications will be removed on connection close
	 * 
	 * @see PiglerAPI#Init(TBuf<64>)
	 * 
	 * @return Connection error, or notification UID
	 * if app was started by tap handle event,
	 * otherwise KErrNone is returned
	 */
	TInt Init();
	
	/**
	 * Sets application ID that needs to be launched on notification tap,
	 * must be called before Init()
	 * 
	 * @param appId Symbian app UID
	 */
	void SetAppId(TInt appId);
	
	/**
	 * @return API version of installed plugin or error code
	 * 
	 * <p>Error codes:</p>
	 * KErrNotReady if connection was not initialized <br>
	 * 
	 * @since v2
	 */
	TInt GetAPIVersion();
	
	/**
	 * @return Current app name
	 */
	TBuf<64> GetAppName();
	
	/**
	 * Adds or updates notification
	 * 
	 * <p>Number of notifications that can be created is limited and depends on the platform</p>
	 * 
	 * @return Notification UID or error code
	 * 
	 * <p>Error codes:</p>
	 * KErrNotFound if there is no such item with that uid <br>
	 * KErrAccessDenied if item was created by another app <br>
	 * KErrNotReady if connection was not initialized <br>
	 * KErrOverflow if no notification slots left <br>
	 * KErrAlreadyExists if application exceeded maximum number of notifications per app <br>
	 * 
	 * @param uid Notification UID, 0 - to create
	 * @param text Notification text
	 */
	TInt SetNotification(TInt uid, TBuf<256> text);
	
	/**
	 * Removes notification by its UID
	 * 
	 * @return Error code
	 * 
	 * <p>Error codes:</p>
	 * KErrNotFound if there is no such item with that uid <br>
	 * KErrAccessDenied if item was created by another app <br>
	 * KErrNotReady if connection was not initialized <br>
	 * 
	 * @param uid Notification UID
	 */
	TInt RemoveNotification(TInt uid);
	
	/**
	 * Removes all notifications created by this app
	 * 
	 * @return Removed items count or error code
	 * 
	 * <p>Error codes:</p>
	 * KErrNotReady if connection was not initialized <br>
	 */
	TInt RemoveAllNotifications();
	
	/**
	 * Gets last tapped notification
	 * 
	 * @return Notification UID or error code
	 * 
	 * <p>Error codes:</p>
	 * KErrNotFound if app is not initialized <br>
	 * KErrNone if no notification has been tapped yet <br>
	 * KErrNotReady if connection was not initialized <br>
	 */
	TInt GetLastTappedNotification();
	
	/**
	 * Marks that notification needs to be removed on tap,
	 * by default this setting is true
	 * 
	 * @return Error code
	 * 
	 * <p>Error codes:</p>
	 * KErrNotFound if there is no such item with that uid <br>
	 * KErrAccessDenied if item was created by another app <br>
	 * KErrNotReady if connection was not initialized <br>
	 * 
	 * @param uid Notification UID
	 * @param remove
	 */
	TInt SetRemoveOnTap(TInt uid, TBool remove);
	
	/**
	 * Marks that app needs to be launched on notification tap,
	 * by default this setting is true
	 * 
	 * @return Error code
	 * 
	 * <p>Error codes:</p>
	 * KErrNotFound if there is no such item with that uid <br>
	 * KErrAccessDenied if item was created by another app <br>
	 * KErrNotReady if connection was not initialized <br>
	 * 
	 * @param uid Notification UID
	 * @param launchOnTap
	 * 
	 * @see PiglerAPI#SetAppId(TInt)
	 * 
	 * @since v2
	 */
	TInt SetLaunchAppOnTap(TInt uid, TBool launchOnTap);
	
	/**
	 * Sets notification icon
	 * 
	 * @return Error code
	 * 
	 * <p>Error codes:</p>
	 * KErrNotFound if there is no such item with that uid <br>
	 * KErrAccessDenied if item was created by another app <br>
	 * KErrUnderflow if icon or mask have the smaller size than 68x68 <br>
	 * KErrNotReady if connection was not initialized <br>
	 * 
	 * @param uid Notification UID
	 * @param iconBitmap Icon bitmap 68x68 in ARGB32 format, 
	 * since v4 bitmap size is obtained by {@link #GetBitmapSize}
	 */
	TInt SetNotificationIcon(TInt uid, TPtrC8& iconBitmap);
	
	/**
	 * Gets number of notifications created by this app
	 * 
	 * @return Notifications count or error code
	 * 
	 * <p>Error codes:</p>
	 * KErrNotReady if connection was not initialized <br>
	 * KErrNotSupported
	 * 
	 * @since v3
	 */
	TInt GetNotificationsCount();
	
	/**
	 * Gets maximum number of notifications that can be created by plugin
	 * 
	 * @return Max notifications count or error code
	 * 
	 * <p>Error codes:</p>
	 * KErrNotReady if connection was not initialized <br>
	 * KErrNotSupported
	 * 
	 * @since v4
	 */
	TInt GetMaxNotificationsCount();
	
	/**
	 * Gets number of notifications created by all applications
	 * 
	 * @return Global notifications count or error code
	 * 
	 * <p>Error codes:</p>
	 * KErrNotReady if connection was not initialized <br>
	 * KErrNotSupported
	 * 
	 * @since v4
	 */
	TInt GetGlobalNotificationsCount();
	
	/**
	 * Starts server on Anna and N97, required to call before initializing connection
	 * 
	 * @since v4
	 */
	TInt StartAnnaServer();
	
	/**
	 * Returns bitmap dimension
	 * 
	 * @since v4
	 */
	TInt GetBitmapSize();
	
	/**
	 * 
	 * @since v5
	 */
	TInt GetTextLines();
	
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
	CPiglerTapServer *iServer;
	TInt Connect();
	TInt SendMessage(TInt function, const TPiglerMessage aMessage);
	TInt Remove();
	TBuf<64> iAppName;
	TBool iConnected;
	TInt iAppId;
	TBool iRandom;
};

#endif
