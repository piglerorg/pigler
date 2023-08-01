package org.pigler.api;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

import com.nokia.mid.ui.DirectUtils;
import com.nokia.mj.impl.rt.support.Finalizer;
import com.nokia.mj.impl.rt.support.Jvm;

public final class PiglerAPI {
	
	static {
		try {
			Jvm.loadSystemLibrary("piglerjavaapi");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private int serverHandle;
	private int apiHandle;
	private Finalizer finalizer;
	private boolean closed;
	
	public PiglerAPI() {
		finalizer = registerFinalize();
		serverHandle = _createFunctionServer();
		apiHandle = _createAPI(serverHandle);
	}

	public void init() throws Exception {
		if (closed || apiHandle == 0) {
			throw new PiglerException("API connection closed");
		}
		int res = _initRandom(serverHandle, apiHandle);
		if (res < 0) {
			throw new PiglerException("Init error: " + res);
		}
	}

	public void init(String appName) throws Exception {
		if (closed || apiHandle == 0) {
			throw new PiglerException("API connection closed");
		}
		if (appName == null) {
			throw new NullPointerException("appName");
		}
		if (appName.length() > 63) {
			appName = appName.substring(0, 63);
		}
		int res = _init(serverHandle, apiHandle, appName);
		if (res < 0) {
			throw new PiglerException("Init error: " + res);
		}
	}
	
	public int createNotification(String title, String text, Image icon, boolean removeOnTap) throws Exception {
		if (closed || apiHandle == 0) {
			throw new PiglerException("API connection closed");
		}
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
		int res = _setNotification(serverHandle, apiHandle, 0, title + "\n" + text);
		if (res < 0) {
			throw new PiglerException("Create notification error: " + res);
		}
		if(removeOnTap) {
			_setRemoveNotificationOnTap(serverHandle, apiHandle, 0, removeOnTap);
		}
		if (icon != null) {
			setNotificationIcon(res, icon);
		}
		return res;
	}
	
	public void updateNotification(int uid, String title, String text, Image icon) throws Exception {
		updateNotification(uid, title, text);
		updateNotification(uid, icon);
	}
	
	public void updateNotification(int uid, String title, String text) throws Exception {
		if (closed || apiHandle == 0) {
			throw new PiglerException("API connection closed");
		}
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
		int res = _setNotification(serverHandle, apiHandle, uid, title + "\n" + text);
		if (res < 0) {
			throw new PiglerException("Update notification text error:" + res);
		}
	}
	
	public void updateNotification(int uid, Image icon) throws Exception {
		if (closed || apiHandle == 0) {
			throw new PiglerException("API connection closed");
		}
		if (icon == null) {
			throw new NullPointerException("icon");
		}
		int res = setNotificationIcon(uid, icon);
		if (res < 0) {
			throw new PiglerException("Update notification icon error: " + res);
		}
	}
	
	public void removeNotification(int uid) throws Exception {
		if (closed || apiHandle == 0) {
			throw new PiglerException("API connection closed");
		}
		int res = _removeNotification(serverHandle, apiHandle, uid);
		if (res < 0) {
			throw new PiglerException("Remove notification error: " + res);
		}
	}
	
	public int removeAllNotifications() {
		if (closed || apiHandle == 0) {
			throw new PiglerException("API connection closed");
		}
		return _removeAllNotifications(serverHandle, apiHandle);
	}
	
	public int getLastTappedNotification() {
		if (closed || apiHandle == 0) {
			throw new PiglerException("API connection closed");
		}
		return _getLastTappedNotification(serverHandle, apiHandle);
	}
	
	public void setRemoveNotificationOnTap(int uid, boolean remove) throws Exception {
		if (closed || apiHandle == 0) {
			throw new PiglerException("API connection closed");
		}
		int res = _setRemoveNotificationOnTap(serverHandle, apiHandle, uid, remove);
		if (res < 0) {
			throw new PiglerException("Update notification error: " + res);
		}
	}
	
	public void close() {
		if (closed || apiHandle == 0) return;
		closed = true;
		_close(serverHandle, apiHandle);
	}
	
	private int setNotificationIcon(int uid, Image icon) {
		if (icon == null) return 0;
		Image scaledIcon = DirectUtils.createImage(68, 68, 0);
		
		// icon scaling
		final int targetSize = 53;
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
		return _setNotificationIcon(serverHandle, apiHandle, uid, rgb);
	}
	
	private Finalizer registerFinalize() {
		return new Finalizer() {
			public void finalizeImpl() {
				close();
				_dispose(serverHandle, apiHandle);
				serverHandle = apiHandle = 0;
			}
		};
	}

	private native int _createFunctionServer();
	private native int _createAPI(int serverHandle);
	private native void _dispose(int serverHandle, int apiHandle);
	private native int _initRandom(int serverHandle, int apiHandle);
	private native int _init(int serverHandle, int apiHandle, String appName);
	private native int _setNotification(int serverHandle, int apiHandle, int uid, String text);
	private native int _removeNotification(int serverHandle, int apiHandle, int uid);
	private native int _removeAllNotifications(int serverHandle, int apiHandle);
	private native int _getLastTappedNotification(int serverHandle, int apiHandle);
	private native int _setRemoveNotificationOnTap(int serverHandle, int apiHandle, int uid, boolean remove);
	private native int _setNotificationIcon(int serverHandle, int apiHandle, int uid, int[] rgb);
	private native void _close(int serverHandle, int apiHandle);
	
}
