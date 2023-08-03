#include "QPiglerAPI.h"

void QPiglerAPI::HandleTap(TInt uid)
{
	doHandleTap(uid);
	
	if (handler) {
		handler->HandleTap(uid);
	}
}

void QPiglerAPI::doHandleTap(qint32 notificationId)
{
	emit handleTap(notificationId);
}

void QPiglerAPI::setTapHandler(IPiglerTapHandler *handler)
{
	this->handler = handler;
}

QPiglerAPI::QPiglerAPI(QObject* parent)
	: QObject(parent)
	, api(new PiglerAPI)
	, handler(NULL)
{
	api->SetTapHandler(this);
}

QPiglerAPI::~QPiglerAPI()
{
	delete api;
	delete handler;
}

qint32 QPiglerAPI::init()
{
	return api->Init();
}

QString QPiglerAPI::appName()
{
	TBuf<64> buf = api->GetAppName();
	return QString::fromUtf16(buf.Ptr(), buf.Length());
}

qint32 QPiglerAPI::init(QString name)
{
	if (name.length() > 63) {
		name = name.left(63);
	}
	
	TBuf<64> buf(name.utf16());
	return api->Init(buf);
}

void QPiglerAPI::setAppId(qint32 appId)
{
	api->SetAppId(appId);
}

qint32 QPiglerAPI::setNotification(qint32 notificationId, QString title, QString message)
{
	if (title.length() > 63) {
		title = title.left(63);
	}
	
	if (message.length() > 63) {
		message = message.left(63);
	}
	
	title += "\n" + message;
	
	TBuf<128> buf(title.utf16());
	return api->SetNotification(notificationId, buf);
}

qint32 QPiglerAPI::createNotification(QString title, QString message)
{
	return setNotification(0, title, message);
}

qint32 QPiglerAPI::updateNotification(qint32 notificationId, QString title, QString message)
{
	return setNotification(notificationId, title, message);
}

qint32 QPiglerAPI::removeNotification(qint32 notificationId)
{
	return api->RemoveNotification(notificationId);
}

qint32 QPiglerAPI::removeAllNotifications()
{
	return api->RemoveAllNotifications();
}

qint32 QPiglerAPI::getLastTappedNotification()
{
	return api->GetLastTappedNotification();
}

qint32 QPiglerAPI::setRemoveOnTap(qint32 notificationId, bool remove)
{
	return api->SetRemoveOnTap(notificationId, remove);
}

qint32 QPiglerAPI::setLaunchAppOnTap(qint32 notificationId, bool launch)
{
	return api->SetLaunchAppOnTap(notificationId, launch);
}

qint32 QPiglerAPI::setNotificationIcon(qint32 notificationId, QImage icon)
{
	if (icon.width() > 52 || icon.height() > 52) {
		icon = icon.scaled(52, 52, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
	
	icon = icon.convertToFormat(QImage::Format_ARGB32);
	QByteArray buffer(68 * 68 * 4, 0);
	const uchar* bitmap = icon.constBits();
	char* data = buffer.data();
	
	qint32 height = icon.height();
	qint32 width = icon.width();
	qint32 yOffset = (68 - height) / 2;
	qint32 xOffset = (68 - width) / 2;
	
	for (qint32 y = 0; y < height; ++y) {
		memcpy(data + (((y + yOffset) * 68 + xOffset) * 4), bitmap + (y * width * 4), width * 4);
	}
	
	TPtrC8 buf((const TUint8*) buffer.constData(), buffer.length());
	return api->SetNotificationIcon(notificationId, buf);
}

void QPiglerAPI::close()
{
	return api->Close();
}
