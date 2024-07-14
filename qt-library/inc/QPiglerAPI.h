#ifndef QPIGLERAPI
#define QPIGLERAPI

#include <QObject>
#include <QString>
#include <QImage>

#include "PiglerAPI.h"

class QPiglerAPI;

class QPiglerAPI : public QObject, private IPiglerTapHandler
{
	Q_OBJECT
	
private:
	PiglerAPI *api;
	IPiglerTapHandler *handler;
	
public:
	explicit QPiglerAPI(QObject* parent = 0);
	~QPiglerAPI();
	
public slots:
	/**
	 * @see PiglerAPI#Init(TBuf<64>)
	 */
	qint32 init(QString name);
	/**
	 * @see PiglerAPI#Init()
	 */
	qint32 init();
	
	/**
	 * @see PiglerAPI#SetAppId(TInt)
	 * @param appId Symbian app UID
	 */
	void setAppId(qint32 appId);
	
	/**
	 * @see PiglerAPI#GetAppName()
	 * @return Current app name
	 */
	QString appName();
	
	/**
	 * @see PiglerAPI#SetNotification(TInt, TBuf<256>)
	 * @param notificationId Notification id or 0 to create
	 * @param title Title text
	 * @param message Bottom text
	 * @return Notification id or error code
	 */
	qint32 setNotification(qint32 notificationId, QString title, QString message);
	/**
	 * @see PiglerAPI#SetNotification(TInt, TBuf<256>)
	 * @see QPiglerAPI#setNotification(qint32, QString, QString)
	 * @param notificationId Notification id
	 * @param title Title text
	 * @param message Bottom text
	 * @return Notification id or error code
	 */
	qint32 updateNotification(qint32 notificationId, QString title, QString message);
	/**
	 * @see PiglerAPI#SetNotification(TInt, TBuf<256>)
	 * @see QPiglerAPI#setNotification(qint32, QString, QString)
	 * @param title Title text
	 * @param message Bottom text
	 * @return Notification id or error code
	 */
	qint32 createNotification(QString title, QString message);
	
	/**
	 * @see PiglerAPI#RemoveNotification(TInt)
	 * @param notificationId Notification id
	 * @return Error code
	 */
	qint32 removeNotification(qint32 notificationId);
	/**
	 * @see PiglerAPI#RemoveAllNotifications()
	 * @return Removed notifications count or error code
	 */
	qint32 removeAllNotifications();
	
	/**
	 * @see PiglerAPI#GetLastTappedNotification()
	 * @return Notification id or error code
	 */
	qint32 getLastTappedNotification();
	
	/**
	 * @see PiglerAPI#SetRemoveOnTap(TInt, TBool)
	 * @param notificationId Notification id
	 * @param remove
	 * @return Error code
	 */
	qint32 setRemoveOnTap(qint32 notificationId, bool remove);
	/**
	 * @see PiglerAPI#SetLaunchAppOnTap(TInt, TBool)
	 * @param notificationId Notification id
	 * @param launch
	 * @return Error code
	 */
	qint32 setLaunchAppOnTap(qint32 notificationId, bool launch);
	
	/**
	 * @see PiglerAPI#SetNotificationIcon(TInt, TPtrC8&)
	 * @param notificationId Notification id
	 * @param icon Icon image, recommended size is 52x52
	 * @return
	 */
	qint32 setNotificationIcon(qint32 notificationId, QImage icon);
	
	/**
	 * @see PiglerAPI#GetNotificationsCount()
	 * @return Notifications count or error code
	 * @since v3
	 */
	qint32 getNotificationsCount();
	
	/**
	 * @see PiglerAPI#GetMaxNotificationsCount()
	 * @return Max notifications count or error code
	 * @since v4
	 */
	qint32 getMaxNotificationsCount();
	
	/**
	 * @see PiglerAPI#Close()
	 */
	void close();
	
	/**
	 * @see PiglerAPI#SetTapHandler(IPiglerTapHandler*)
	 * @param handler Tap handler
	 */
	void setTapHandler(IPiglerTapHandler *handler);

private:
	virtual void HandleTap(TInt uid);

private slots:
	void doHandleTap(qint32 notificationId);
	
signals:
	void handleTap(qint32 notificationId);
};

#endif
