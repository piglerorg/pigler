#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class QPiglerAPI;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);

    void showExpanded();

private slots:
    void on_pushBtn_clicked();

    void on_popBtn_clicked();

    void on_removeAllBtn_clicked();

    void on_updateBtn_clicked();
    
    void notificationTapped(qint32 id);

private:
    Ui::MainWindow *ui;
    QPiglerAPI *api;
    QList<qint32> notifications;
    
    void log(QString str);
};

#endif // MAINWINDOW_H
