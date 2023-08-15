# PiglerPlugin
### by [shinovon](https://github.com/shinovon) and [curoviyxru](https://github.com/curoviyxru)
The indicator plugin implementation, currently avaliable for Symbian/Nokia Belle only, written in Symbian C++.<br>
The purpose of this plugin is to provide easy and open-source API that provides Symbian's notification features.

![Screenshot](screenshot.png)

# Building
You will need Carbide.c++ v2.7, Qt SDK and Belle SDK for QT SDK<br>
Links: <a href="https://mrrosset.github.io/Symbian-Archive/Dep&IDE.html">Carbide</a>,  <a href="https://mrrosset.github.io/Symbian-Archive/SDKs-Langs.html">Qt SDK and Belle SDK</a>

Note: This guide does not show how to build java-library and java-tester


## Setting up carbide workspace
Open Preferences

![image](https://github.com/piglerorg/pigler/assets/43963888/9b2eb07e-41e0-42ff-8fae-7228f28f482b)

Go to SDK Preferences, add new SDK

![image](https://github.com/piglerorg/pigler/assets/43963888/9c8a3e1a-a298-4dc1-929a-141794044a85)

Specify the path of Belle SDK, fill in the fields as shown here

![image](https://github.com/piglerorg/pigler/assets/43963888/58c8100f-d7f5-451c-8765-8080c5a567eb)

Then go to Qt, add Qt version and make it default

![image](https://github.com/piglerorg/pigler/assets/43963888/e0959905-71ad-413d-b56d-427a32feb20a)

![image](https://github.com/piglerorg/pigler/assets/43963888/e4207a0e-fc5e-4f03-8ab8-aa86708890e8)

## Importing projects
Open context menu in Project Explorer, select "Import"

![image](https://github.com/piglerorg/pigler/assets/43963888/72889f69-58ac-4083-9f51-b4d86c6b5f3a)

Import plugin & sym-library as Symbian OS Bld.inf file

![image](https://github.com/piglerorg/pigler/assets/43963888/41caf6be-fe73-489c-9723-22520347c060)

![image](https://github.com/piglerorg/pigler/assets/43963888/8cde9a00-1f1b-4958-acb1-d643d1d3af08)

![image](https://github.com/piglerorg/pigler/assets/43963888/6103a974-e39c-4b96-a532-d269b413b569)


![image](https://github.com/piglerorg/pigler/assets/43963888/560d4ecd-2f15-4a82-b553-a048e81fbdcd)


Import qt-library & qt-tester as Qt Project

![image](https://github.com/piglerorg/pigler/assets/43963888/7f9c47a0-93be-4f10-848c-5d6ef6330b4e)

![image](https://github.com/piglerorg/pigler/assets/43963888/81f561c8-07e8-49a9-9174-17241bf346d0)



# Planned things:
* [x] Implement indicator icons and various images formats support
* [x] Find a workaround for missing indicators bug until any system indicator is shown on Symbian Belle
* [x] Implement tap handling/callbacking for indicators for native
* [x] Implement tap handling/callbacking for indicators for Java
* [x] Implement icon sending and resizing
* [x] Java JNI methods
* [x] Qt utility methods
* [x] Application launch on indicator tap
* [x] Do not open app on tap option
* [ ] Javadoc, API docs, building guides
* [ ] Implement a range of standard icons for notifications
* [ ] Reboot phone after package install and uninstall
* [ ] Show indicators on lock screen
* [ ] System vibration function
* [ ] Automatic package creation and installation with JNI library
* [ ] Notification permissions management
* [ ] Placeholder icon for notifications without icon
* [ ] Discreet popup show feature
* [ ] ~~Implement a plugin for status pane top indicators~~
* [ ] ~~Notifications saving to DB and load them on system start~~
* [ ] ~~System notification sound / beeping function~~
* [ ] ~~System notification light function~~
* [ ] ~~Research for workarounds for Symbian Anna and Symbian 9.4~~
* [ ] ~~Implement an TCP (or UDP?) server for J2ME and other technology-based apps~~
* [ ] ~~Icon color change depending on theme (as stated in docs)~~
