#include <QObject>
#include <QString>
#include <QImage>

class PiglerAPI;

class QPiglerAPI : public QObject
{
	Q_OBJECT
	
private:
	PiglerAPI* api;
	
public:
	explicit QPiglerAPI(QObject* parent);
	~QPiglerAPI();
	
public slots:
	qint32 init(QString name);
	qint32 init();
	
	QString appName();
	QString hexAppName();
	
	qint32 setNotification(qint32 notificationId, QString title, QString message);
	qint32 updateNotification(qint32 notificationId, QString title, QString message);
	qint32 createNotification(QString title, QString message);
	
	qint32 removeNotification(qint32 notificationId);
	qint32 removeAllNotifications();
	
	qint32 getLastTappedNotification();
	
	qint32 setRemoveNotificationOnTap(qint32 notificationId, bool remove);
	
	qint32 setNotificationIcon(qint32 notificationId, QImage icon);
	
	void close();
};
