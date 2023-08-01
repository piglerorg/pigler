#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QDebug>
#include "QPiglerAPI.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , api(new QPiglerAPI(this))
    , notifications()
{

    ui->setupUi(this);
    
    connect(api, SIGNAL(handleTap(qint32, QString, QString, bool)), this, SLOT(notificationTapped(qint32, QString, QString, bool)));
    
    qint32 error = api->init("HelloWorld");
    if(error) {
        log("API init error: " + QString::number(error));
        api->deleteLater();
        api = NULL;
    } else {
        log("API initialized: " + api->appName());
    }
}

void MainWindow::notificationTapped(qint32 id, QString name, QString text, bool remove)
{
	log("Notification tapped: " + QString::number(id) + " " + text);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
    showMaximized();
}

void MainWindow::on_pushBtn_clicked()
{
    if(!api) return;
    
    qint32 uid = api->createNotification(ui->titleText->text(), ui->bottomText->text());
    log("Push: " + QString::number(uid));

    if(uid > 0) {
        notifications.append(uid);
        log("Icon: " + QString::number(api->setNotificationIcon(uid, QImage(ui->iconText->text()))));
    }
}

void MainWindow::on_popBtn_clicked()
{
    if(!api) return;
    if(notifications.empty()) return;
    
    qint32 uid = notifications.takeLast();
    log("Pop: " + QString::number(api->removeNotification(uid)));
}

void MainWindow::on_removeAllBtn_clicked()
{
    if(!api) return;
    
    notifications.clear();
    log("Remove all: " + QString::number(api->removeAllNotifications()));
}

void MainWindow::on_updateBtn_clicked()
{
    if(!api) return;
    if(notifications.empty()) return;
    
    qint32 uid = api->updateNotification(notifications.takeLast(), ui->titleText->text(), ui->bottomText->text());
    log("Update: " + QString::number(uid));
}

void MainWindow::log(QString str)
{
    ui->label->setText(ui->label->text() + str + "\n");
    qDebug() << str;
}
