package org.pigler.tester;

import javax.microedition.lcdui.Image;

import org.pigler.api.PiglerAPI;

public class PiglerAPILayer {
	
	private PiglerAPI api;

	public PiglerAPILayer() {
		api = new PiglerAPI();
	}

	public void init(String string) throws Exception {
		api.init(string);
	}

	public void close() {
		api.close();
	}

	public int createNotification(String string, String string2, Image icon, boolean b) throws Exception {
		return api.createNotification(string, string2, icon, b);
	}

	public void removeNotification(int uid) throws Exception {
		api.removeNotification(uid);
	}

	public int removeAllNotifications() throws Exception {
		return api.removeAllNotifications();
	}

	public void updateNotification(int uid, String string, String string2) throws Exception {
		api.updateNotification(uid, string, string2);
	}

}
