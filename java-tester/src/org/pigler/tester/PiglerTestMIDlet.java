package org.pigler.tester;
import java.util.Stack;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.Item;
import javax.microedition.lcdui.ItemCommandListener;
import javax.microedition.lcdui.StringItem;
import javax.microedition.lcdui.TextField;
import javax.microedition.midlet.MIDlet;

public class PiglerTestMIDlet extends MIDlet implements CommandListener, ItemCommandListener, PiglerAPIHandlerLayer {
	
	private Form form;
	private PiglerAPILayer api;
	private Image icon;

	private StringItem log;
	private TextField titleText;
	private TextField bottomText;
	
	private Stack notifications;

	public PiglerTestMIDlet() {
		notifications = new Stack();
		
		form = new Form("Pigler notifications test tool");
		form.addCommand(new Command("Exit", Command.EXIT, 1));
		form.setCommandListener(this);
		
		titleText = new TextField("Title", "", 256, TextField.ANY);
		form.append(titleText);
		
		bottomText = new TextField("Bottom text", "", 256, TextField.ANY);
		form.append(bottomText);
		
		StringItem pushBtn = new StringItem(null, "Push", StringItem.BUTTON);
		pushBtn.setLayout(Item.LAYOUT_EXPAND);
		Command pushCmd = new Command("Push", Command.ITEM, 2);
		pushBtn.addCommand(pushCmd);
		pushBtn.setDefaultCommand(pushCmd);
		pushBtn.setItemCommandListener(this);
		form.append(pushBtn);
		
		StringItem updateBtn = new StringItem(null, "Update", StringItem.BUTTON);
		updateBtn.setLayout(Item.LAYOUT_EXPAND | Item.LAYOUT_NEWLINE_AFTER);
		Command updateCmd = new Command("Update", Command.ITEM, 3);
		updateBtn.addCommand(updateCmd);
		updateBtn.setDefaultCommand(updateCmd);
		updateBtn.setItemCommandListener(this);
		form.append(updateBtn);
		
		StringItem popBtn = new StringItem(null, "Pop", StringItem.BUTTON);
		popBtn.setLayout(Item.LAYOUT_EXPAND | Item.LAYOUT_NEWLINE_AFTER);
		Command popCmd = new Command("Pop", Command.ITEM, 3);
		popBtn.addCommand(popCmd);
		popBtn.setDefaultCommand(popCmd);
		popBtn.setItemCommandListener(this);
		form.append(popBtn);
		
		StringItem removeAllBtn = new StringItem(null, "Remove all", StringItem.BUTTON);
		removeAllBtn.setLayout(Item.LAYOUT_EXPAND | Item.LAYOUT_NEWLINE_AFTER);
		Command removeAllCmd = new Command("Remove all", Command.ITEM, 4);
		removeAllBtn.addCommand(removeAllCmd);
		removeAllBtn.setDefaultCommand(removeAllCmd);
		removeAllBtn.setItemCommandListener(this);
		form.append(removeAllBtn);
		
		log = new StringItem("", "");
		form.append(log);
		
		try {
			icon = Image.createImage("/testicon.png");
		} catch (Exception e) {
			e.printStackTrace();
			log("Icon load: " + e.toString());
		}
		
		String version = System.getProperty("org.pigler.api.version");
		if(version == null) {
			log("Missing PiglerJavaAPI extension!");
			return;
		}
		log("PiglerJavaAPI version: " + version);
		try {
			api = new PiglerAPILayer();
			api.setListener(this);
			api.init("Example");
			log("API loaded successfuly");
		} catch (Throwable e) {
			e.printStackTrace();
			log("API load: " + e.toString());
		}
	}
	
	private void log(String text) {
		log.setText(log.getText()+text+"\n");
	}

	protected void destroyApp(boolean unconditional) {
		if(api != null) {
			api.close();
			api = null;
		}
	}

	protected void pauseApp() {
	}

	protected void startApp() {
		Display.getDisplay(this).setCurrent(form);
	}

	public void commandAction(Command c, Displayable d) {
		if(c.getCommandType() == Command.EXIT) {
			destroyApp(false);
			notifyDestroyed();
			return;
		}
		if(api == null) return;
		switch(c.getPriority()) {
		case 2:
			try {
				int uid = api.createNotification(titleText.getString(), bottomText.getString(), icon, true);
				notifications.push(new Integer(uid));
				log("Pushed: " + uid);
			} catch (Exception e) {
				e.printStackTrace();
				log("Push: " + e.toString());
			}
			break;
		case 3:
			try {
				int uid = ((Integer) notifications.pop()).intValue();
				api.removeNotification(uid);
				log("Removed: " + String.valueOf(uid));
			} catch (Exception e) {
				e.printStackTrace();
				log("Pop: " + e.toString());
			}
			break;
		case 4:
			try {
				log("Remove all: " + String.valueOf(api.removeAllNotifications()));
				notifications.removeAllElements();
			} catch (Exception e) {
				e.printStackTrace();
				log("Remove all: " + e.toString());
			}
			break;
		case 5:
			try {
				int uid = ((Integer) notifications.lastElement()).intValue();
				api.updateNotification(uid, titleText.getString(), bottomText.getString());
				log("Updated: " + uid);
			} catch (Exception e) {
				e.printStackTrace();
				log("Update: " + e.toString());
			}
			break;
		}
	}

	public void commandAction(Command c, Item item) {
		commandAction(c, (Displayable) null);
	}

	public void handleNotificationTap(int uid) {
		log("Tap: " + uid);
	}

}
