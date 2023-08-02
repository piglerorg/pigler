package org.pigler.api;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

import com.nokia.mid.ui.DirectUtils;
import com.nokia.mj.impl.rt.legacy.LegacyRtPort;
import com.nokia.mj.impl.rt.legacy.MIDEventServer;
import com.nokia.mj.impl.rt.support.Finalizer;
import com.nokia.mj.impl.rt.support.Jvm;

/**
 * Pigler Notifications Java API
 * 
 * @version 1.0, API level 2
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

	private static final MIDEventServer eventServer = new MIDEventServer("java-piglerapi");
	private Finalizer finalizer;
	private int eventSourceHandle;
	private int apiHandle;
	private boolean closed;
	private IPiglerTapHandler listener;
	private boolean initialized;
	
	/**
	 * Creates API instance
	 */
	public PiglerAPI() {
		finalizer = registerFinalize();
		eventSourceHandle = _createEventSource(eventServer.getHandle());
		apiHandle = _createAPI(eventSourceHandle);
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
		int res = _init(eventSourceHandle, apiHandle, LegacyRtPort.getMidletUid(), "JavaApp_" + Integer.toHexString(LegacyRtPort.getMidletUid()));
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
		if (appName.length() > 63) {
			appName = appName.substring(0, 63);
		}
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
	 * @throws PiglerException If connection is not ready
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
	 * @param icon Icon, recommended size is 52x52
	 * @param removeOnTap Remove on tapped
	 * @return Notification id
	 * 
	 * @throws IllegalStateException If connection not initialized or closed
	 * @throws PiglerException If connection is not ready
	 * @throws PiglerException If creation is failed
	 * 
	 * @see PiglerAPI#updateNotification(int, String, String, Image)
	 * @see PiglerAPI#setRemoveNotificationOnTap(int, boolean)
	 * @see PiglerAPI#updateNotification(int, Image)
	 */
	public int createNotification(String title, String text, Image icon, boolean removeOnTap) throws Exception {
		checkClosed();
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
		int res = _setNotification(eventSourceHandle, apiHandle, 0, title + "\n" + text);
		if (res < 0) {
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
	 * @param icon Icon, recommended size is 52x52
	 * 
	 * @throws IllegalStateException if connection not initialized or closed
	 * @throws IllegalArgumentException If notification id is wrong
	 * @throws PiglerException If connection is not ready
	 * @throws PiglerException If update is failed
	 * 
	 * @see PiglerAPI#updateNotification(int, String, String)
	 * @see PiglerAPI#updateNotification(int, Image)
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
	 * @throws PiglerException If connection is not ready
	 * @throws PiglerException If update is failed
	 */
	public void updateNotification(int uid, String title, String text) throws Exception {
		if (uid <= 0) {
			throw new IllegalArgumentException();
		}
		checkClosed();
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
		int res = _setNotification(eventSourceHandle, apiHandle, uid, title + "\n" + text);
		if (res < 0) {
			throw new PiglerException("Update notification text error:" + res);
		}
	}
	/**
	 * Updates notification icon
	 * 
	 * @param uid Notification id
	 * @param icon Icon, recommended size is 52x52
	 * 
	 * @throws IllegalStateException if connection not initialized or closed
	 * @throws IllegalArgumentException If notification id is wrong
	 * @throws PiglerException If connection is not ready
	 * @throws PiglerException If update is failed
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
	 * @throws PiglerException If connection is not ready
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
	 * @throws PiglerException If connection is not ready
	 */
	public int removeAllNotifications() {
		checkClosed();
		return _removeAllNotifications(eventSourceHandle, apiHandle);
	}
	
	/**
	 * @return Last tapped notification id
	 * 
	 * @throws IllegalStateException if connection not initialized or closed
	 * @throws PiglerException If connection is not ready
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
	 * @throws PiglerException If connection is not ready
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
	 * @throws PiglerException If connection is not ready
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
	
	private int setNotificationIcon(int uid, Image icon) {
		if (icon == null) return 0;
		Image scaledIcon = DirectUtils.createImage(68, 68, 0);
		
		// icon scaling
		final int targetSize = 52;
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
			(68 - icon.getWidth()) >> 1, (68 - icon.getHeight()) >> 1, Graphics.LEFT | Graphics.TOP);
		
		icon = null;
		
		int[] rgb = new int[68 * 68];
		scaledIcon.getRGB(rgb, 0, 68, 0, 0, 68, 68);
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
	
}
