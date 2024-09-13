/**
 * Copyright (c) 2023-2024 Arman Jussupgaliyev
 * Copyright (c) 2023 Mathew (curoviyxru)
 */

package org.pigler.api;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

import com.nokia.mid.ui.DirectUtils;
import com.nokia.mj.impl.rt.legacy.LegacyRtPort;
import com.nokia.mj.impl.rt.legacy.MIDEventServer;
import com.nokia.mj.impl.rt.legacy.ToolkitInvoker;
import com.nokia.mj.impl.rt.support.Finalizer;
import com.nokia.mj.impl.rt.support.Jvm;

/**
 * Pigler Notifications Java API
 * 
 * @version 1.4, API level 4
 * 
 * @author Shinovon
 */
public final class PiglerAPI {
	
	static {
		try {
			Jvm.loadSystemLibrary("piglerjavaapi");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 *  Popup's duration is long (default is short)
	 */
	public static final int DiscreetPopupDurationLong = 0x00000001;
	/**
	 *  Popup switches the device light on
	 */
	public static final int DiscreetPopupLightsOn = 0x00000002;
	/**
	 *  Popup plays the confirmation tone
	 */
	public static final int DiscreetPopupConfirmationTone = 0x00000004;
	/**
	 *  Popup plays the warning tone
	 */
	public static final int DiscreetPopupWarningTone = 0x00000008;
	/**
	 *  Popup plays the error tone
	 */
	public static final int DiscreetPopupErrorTone = 0x00000010;

	private static final MIDEventServer eventServer = new MIDEventServer("java-piglerapi");
	private Finalizer finalizer;
	private int eventSourceHandle;
	private int apiHandle;
	private boolean closed;
	private IPiglerTapHandler listener;
	private boolean initialized;
	private String appName;
	private boolean isBelle;
	
	/**
	 * Creates API instance
	 */
	public PiglerAPI() {
		finalizer = registerFinalize();
		eventSourceHandle = _createEventSource(eventServer.getHandle());
		apiHandle = _createAPI(eventSourceHandle);
		String s = System.getProperty("microedition.platform");
		isBelle = s != null && s.charAt(s.indexOf("platform_version=") + 19) > '2';
		if (!isBelle) _startAnnaServer(eventSourceHandle, apiHandle);
	}

	/**
	 * Initializes API connection
	 * 
	 * @return Missed notification id or 0
	 * 
	 * @throws IllegalStateException If already initialized
	 * @throws PiglerException If initialization failed
	 * 
	 * @see PiglerAPI#init(String)
	 * @see PiglerAPI#close()
	 */
	public int init() throws Exception {
		if (initialized) throw new IllegalStateException();
		//int res = _initRandom(serverHandle, apiHandle, LegacyRtPort.getMidletUid());
		String appName = "JavaApp_" + Integer.toHexString(LegacyRtPort.getMidletUid());
		this.appName = appName;
		int res = _init(eventSourceHandle, apiHandle, LegacyRtPort.getMidletUid(), appName);
		if (res < 0) {
			throw new PiglerException("Init error: " + res);
		}
		initialized = true;
		return res;
	}

	/**
	 * Initializes API connection with specified app name
	 * 
	 * @param appName App name
	 * @return Missed notification id or 0
	 * 
	 * @throws IllegalStateException If already initialized
	 * @throws PiglerException If initialization failed
	 * 
	 * @see PiglerAPI#close()
	 */
	public int init(String appName) throws Exception {
		if (initialized) throw new IllegalStateException();
		if (appName == null) {
			throw new NullPointerException("appName");
		}
		if (appName.length() == 0) {
			throw new IllegalArgumentException("appName");
		}
		if (appName.length() > 63) {
			appName = appName.substring(0, 63);
		}
		this.appName = appName;
		int res = _init(eventSourceHandle, apiHandle, LegacyRtPort.getMidletUid(), appName);
		if (res < 0) {
			throw new PiglerException("Init error: " + res);
		}
		initialized = true;
		return res;
	}
	
	/**
	 * Gets API version of installed plugin
	 * 
	 * @return Supported API level
	 * 
	 * @throws IllegalStateException If connection not initialized or closed
	 * @throws PiglerException -18 If connection is not ready
	 * 
	 * @since API level 2
	 */
	public int getAPIVersion() throws Exception {
		checkClosed();
		return _getAPIVersion(eventSourceHandle, apiHandle);
	}

	/**
	 * Creates notification
	 * 
	 * @param title Title text
	 * @param text Bottom text
	 * @param icon Icon, see {@link PiglerAPI#getIconSize()}
	 * @param removeOnTap Remove on tapped
	 * @return Notification id
	 * 
	 * @throws IllegalStateException If connection not initialized or closed
	 * @throws PiglerException -18 If connection is not ready
	 * @throws PiglerException If creation is failed
	 * 
	 * @see PiglerAPI#updateNotification(int, String, String, Image)
	 * @see PiglerAPI#setRemoveNotificationOnTap(int, boolean)
	 * @see PiglerAPI#updateNotification(int, Image)
	 * @see PiglerAPI#getIconSize()
	 */
	public int createNotification(String title, String text, Image icon, boolean removeOnTap) throws Exception {
		checkClosed();
		
		int res = _setNotification(eventSourceHandle, apiHandle, 0, formatText(title, text));
		if (res < 0) {
			if (res == -9) {
				throw new PiglerException("No more notification slots left");
			}
			throw new PiglerException("Create notification error: " + res);
		}
		_setRemoveNotificationOnTap(eventSourceHandle, apiHandle, 0, removeOnTap);
		if (icon != null) {
			setNotificationIcon(res, icon);
		}
		return res;
	}
	
	/**
	 * Updates notification
	 * 
	 * @param uid Notification id
	 * @param title Title text
	 * @param text Bottom text
	 * @param icon Icon, see {@link PiglerAPI#getIconSize()}
	 * 
	 * @throws IllegalStateException if connection not initialized or closed
	 * @throws IllegalArgumentException If notification id is wrong
	 * @throws PiglerException If connection is not ready
	 * @throws PiglerException If update is failed
	 * 
	 * @see PiglerAPI#updateNotification(int, String, String)
	 * @see PiglerAPI#updateNotification(int, Image)
	 * @see PiglerAPI#getIconSize()
	 */
	public void updateNotification(int uid, String title, String text, Image icon) throws Exception {
		updateNotification(uid, title, text);
		updateNotification(uid, icon);
	}
	
	/**
	 * Updates notification text
	 * 
	 * @param uid Notification id
	 * @param title Title text
	 * @param text Bottom text
	 * 
	 * @throws IllegalStateException if connection not initialized or closed
	 * @throws IllegalArgumentException If notification id is wrong
	 * @throws PiglerException -18 If connection is not ready
	 * @throws PiglerException If update is failed
	 */
	public void updateNotification(int uid, String title, String text) throws Exception {
		if (uid <= 0) {
			throw new IllegalArgumentException();
		}
		checkClosed();
		int res = _setNotification(eventSourceHandle, apiHandle, uid, formatText(title, text));
		if (res < 0) {
			throw new PiglerException("Update notification text error:" + res);
		}
	}
	
	/**
	 * Updates notification icon
	 * 
	 * @param uid Notification id
	 * @param icon Icon, see {@link PiglerAPI#getIconSize()}
	 * 
	 * @throws IllegalStateException if connection not initialized or closed
	 * @throws IllegalArgumentException If notification id is wrong
	 * @throws PiglerException -18 If connection is not ready
	 * @throws PiglerException If update is failed
	 * 
	 * @see PiglerAPI#getIconSize()
	 */
	public void updateNotification(int uid, Image icon) throws Exception {
		if (uid <= 0) {
			throw new IllegalArgumentException();
		}
		checkClosed();
		if (icon == null) {
			throw new NullPointerException("icon");
		}
		int res = setNotificationIcon(uid, icon);
		if (res < 0) {
			throw new PiglerException("Update notification icon error: " + res);
		}
	}
	
	/**
	 * Removes notification 
	 * 
	 * @param uid Notification id
	 * 
	 * @throws IllegalStateException if connection not initialized or closed
	 * @throws PiglerException -18 If connection is not ready
	 * @throws PiglerException If remove is failed
	 */
	public void removeNotification(int uid) throws Exception {
		checkClosed();
		int res = _removeNotification(eventSourceHandle, apiHandle, uid);
		if (res < 0) {
			throw new PiglerException("Remove notification error: " + res);
		}
	}
	
	/**
	 * Removes all notifications created by this app or session
	 * 
	 * @return Removed notifications count
	 * 
	 * @throws IllegalStateException if connection not initialized or closed
	 * @throws PiglerException -18 If connection is not ready
	 */
	public int removeAllNotifications() {
		checkClosed();
		return _removeAllNotifications(eventSourceHandle, apiHandle);
	}
	
	/**
	 * @return Last tapped notification id
	 * 
	 * @throws IllegalStateException if connection not initialized or closed
	 * @throws PiglerException -18 If connection is not ready
	 */
	public int getLastTappedNotification() {
		checkClosed();
		return _getLastTappedNotification(eventSourceHandle, apiHandle);
	}
	
	/**
	 * Marks that notification needs to be removed on tap.<br>
	 * By default it's true
	 * 
	 * @param uid Notification id
	 * @param remove
	 * 
	 * @throws IllegalStateException if connection not initialized or closed
	 * @throws PiglerException -18 If connection is not ready
	 * @throws PiglerException If update is failed
	 */
	public void setRemoveNotificationOnTap(int uid, boolean remove) throws Exception {
		checkClosed();
		int res = _setRemoveNotificationOnTap(eventSourceHandle, apiHandle, uid, remove);
		if (res < 0) {
			throw new PiglerException("Update notification error: " + res);
		}
	}
	
	/**
	 * Marks that app needs to be launched on notification tap.<br>
	 * By default it's true
	 * 
	 * @param uid Notification id
	 * @param launch
	 * 
	 * @throws IllegalStateException if connection not initialized or closed
	 * @throws PiglerException -18 If connection is not ready
	 * @throws PiglerException If update is failed
	 * 
	 * @since API level 2
	 */
	public void setLaunchAppOnTap(int uid, boolean launch) throws Exception {
		checkClosed();
		int res = _setLaunchAppOnTap(eventSourceHandle, apiHandle, uid, launch);
		if (res < 0) {
			throw new PiglerException("Update notification error: " + res);
		}
	}
	
	/**
	 * Sets notification tap handler for this app
	 * 
	 * @param listener Tap handler
	 */
	public void setListener(IPiglerTapHandler listener) {
		this.listener = listener;
	}
	
	/**
	 * Closes API connection
	 */
	public void close() {
		if (closed || apiHandle == 0) return;
		closed = true;
		_close(eventSourceHandle, apiHandle);
	}
	
	/**
	 * @return Current app name
	 */
	public String getAppName() {
		return appName;
	}

	/**
	 * @return Maximum number of notifications that can be created at one time, negative value if unknown
	 * <p>Depends on platform</p>
	 * 
	 * @since Java API 1.2 / API level 4
	 */
	public int getMaxNotificationsCount() {
		if (apiHandle == 0 || !initialized) {
			return -1;
		}
		int r = _getMaxNotificationsCount(eventSourceHandle, apiHandle);
		return r < 0 ? (isBelle ? 100 : r) : r;
	}
	
	/**
	 * @return Number of created notifications by this app
	 * 
	 * @throws IllegalStateException if connection not initialized or closed
	 * @throws PiglerException -18 If connection is not ready
	 * @throws PiglerException -5 If function is not supported
	 * 
	 * @since Java API 1.2, API level 3
	 */
	public int getNotificationsCount() throws Exception {
		checkClosed();
		return _getNotificationsCount(eventSourceHandle, apiHandle);
	}
	
	/**
	 * @return Number of created notifications by all apps
	 * 
	 * @throws IllegalStateException if connection not initialized or closed
	 * @throws PiglerException -18 If connection is not ready
	 * @throws PiglerException -5 If function is not supported
	 * 
	 * @since Java API 1.4, API level 4
	 */
	public int getGlobalNotificationsCount() throws Exception {
		checkClosed();
		int r = _getGlobalNotificationsCount(eventSourceHandle, apiHandle);
		return r;
	}
	
	/**
	 * @return Item has only one line
	 * 
	 * @since Java API 1.4
	 */
	public boolean isSingleLine() {
		return !isBelle;
	}
	
	/**
	 * @return Preferred icon size dimension
	 * 
	 * @since Java API 1.4
	 */
	public int getIconSize() {
		return isBelle ? 52 : 28;
	}
	
	/**
	 * Shows global popup
	 * 
	 * @param title Title
	 * @param text Text, may be null
	 * @param flags Popup flags
	 * @since Java API 1.4
	 * 
	 * @see PiglerAPI#DiscreetPopupDurationLong
	 * @see PiglerAPI#DiscreetPopupLightsOn
	 * @see PiglerAPI#DiscreetPopupConfirmationTone
	 * @see PiglerAPI#DiscreetPopupWarningTone
	 * @see PiglerAPI#DiscreetPopupErrorTone
	 */
	public void showGlobalPopup(String title, String text, int flags) {
		if (title == null) {
			throw new NullPointerException("title");
		}
		if (text == null) {
			text = "";
		}
		if((flags & ~(DiscreetPopupDurationLong
				| DiscreetPopupLightsOn
				| DiscreetPopupConfirmationTone
				| DiscreetPopupWarningTone
				| DiscreetPopupErrorTone)) != 0) {
			throw new IllegalArgumentException("flags");
		}
		ToolkitInvoker inv = ToolkitInvoker.getToolkitInvoker();
		int res = _showGlobalDiscreetPopup(eventSourceHandle, inv.toolkitGetHandle(inv.getToolkit()), title, text, flags, LegacyRtPort.getMidletUid());
		if (res < 0) {
			throw new PiglerException("showGlobalPopup error: " + res);
		}
	}
	
	private void checkClosed() {
		if (!initialized || closed || apiHandle == 0) {
			throw new IllegalStateException();
		}
	}
	
	private void notificationCallback(int uid) {
		if(listener != null) {
			listener.handleNotificationTap(uid);
		}
	}
	
	private String formatText(String title, String text) {
		if (title == null) {
			title = "";
		}
		if (text == null) {
			text = "";
		}
		if (title.length() > 63) {
			title = title.substring(0, 63);
		}
		if (text.length() > 63) {
			text = text.substring(0, 63);
		}
		if (!isBelle) {
			if (text.length() > 0) {
				title = title + " " + text;
			}
			title = title.trim();
		} else {
			if (text.length() > 0) {
				title = title + "\n" + text;
			}
		}
		if (title.length() == 0) {
			throw new IllegalArgumentException();
		}
		return title;
	}
	
	private int setNotificationIcon(int uid, Image icon) {
		if (icon == null) return 0;
		int size = _getBitmapSize(eventSourceHandle, apiHandle);
		if (size == 0) return 0;
		if (size < 0) size = 68;
		Image scaledIcon = DirectUtils.createImage(size, size, 0);
		
		// icon scaling
		final int targetSize = getIconSize();
		int w = icon.getWidth();
		int h = icon.getHeight();
		
		if (w > targetSize || h > targetSize) {
			if (w > h) {
				icon = ImageUtils.resize(icon, targetSize, targetSize * h / w);
			} else {
				icon = ImageUtils.resize(icon, targetSize * w / h, targetSize);
			}
		}
		
		scaledIcon.getGraphics().drawImage(icon, 
			(size - icon.getWidth()) >> 1, (size - icon.getHeight()) >> 1, Graphics.LEFT | Graphics.TOP);
		
		icon = null;
		
		int[] rgb = new int[size * size];
		scaledIcon.getRGB(rgb, 0, size, 0, 0, size, size);
		return _setNotificationIcon(eventSourceHandle, apiHandle, uid, rgb);
	}
	
	private Finalizer registerFinalize() {
		return new Finalizer() {
			public void finalizeImpl() {
				close();
				_dispose(eventSourceHandle, apiHandle);
				eventSourceHandle = apiHandle = 0;
			}
		};
	}

	private native int _createEventSource(int eventServerHandle);
	private native int _createAPI(int serverHandle);
	private native void _dispose(int serverHandle, int apiHandle);
	private native int _initRandom(int serverHandle, int apiHandle, int midletID);
	private native int _init(int serverHandle, int apiHandle, int midletID, String appName);
	private native int _setNotification(int serverHandle, int apiHandle, int uid, String text);
	private native int _removeNotification(int serverHandle, int apiHandle, int uid);
	private native int _removeAllNotifications(int serverHandle, int apiHandle);
	private native int _getLastTappedNotification(int serverHandle, int apiHandle);
	private native int _setRemoveNotificationOnTap(int serverHandle, int apiHandle, int uid, boolean remove);
	private native int _setNotificationIcon(int serverHandle, int apiHandle, int uid, int[] rgb);
	private native void _close(int serverHandle, int apiHandle);
	private native int _getAPIVersion(int serverHandle, int apiHandle);
	private native int _setLaunchAppOnTap(int serverHandle, int apiHandle, int uid, boolean launch);
	private native int _getNotificationsCount(int serverHandle, int apiHandle);
	private native int _getMaxNotificationsCount(int serverHandle, int apiHandle);
	private native int _getGlobalNotificationsCount(int serverHandle, int apiHandle);
	private native int _getBitmapSize(int serverHandle, int apiHandle);
	private native int _startAnnaServer(int serverHandle, int apiHandle);
	private native int _showGlobalDiscreetPopup(int serverHandle, int toolkitHandle, String title, String text, int flags, int uid);
	
}
