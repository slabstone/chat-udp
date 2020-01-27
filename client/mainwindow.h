#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>  // uinque_ptr

#include <QtCore/QObject>  // Q_OBJECT
#include <QtCore/QSettings>
#include <QtGui/QMainWindow>

#include "client.h"  // Client

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();  // = default
    void init();

private:
    void readSettings();
    void saveSettings();

    Q_SLOT void onReturnPressed();
    Q_SLOT void onMessageReceived(const QString& message);

    // button handlers
    Q_SLOT void on_connectButton_clicked();
    Q_SLOT void on_disconnectButton_clicked();

    Client client;

    // application settings
    QSettings settings;

    std::unique_ptr<Ui::MainWindow> ui;
};

#endif  // MAINWINDOW_H
