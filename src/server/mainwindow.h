#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>  // uinque_ptr

#include <QtCore/QObject>  // Q_OBJECT
#include <QtCore/QSettings>
#include <QtGui/QMainWindow>

#include "server.h"  // Server

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    void init();

private:
    void readSettings();
    void saveSettings();

    Q_SLOT void serverSocketStateChanged(QAbstractSocket::SocketState socketState);

    // button handlers
    Q_SLOT void on_startButton_clicked();
    Q_SLOT void on_stopButton_clicked();
    Q_SLOT void on_clientsButton_clicked();

    Server server;

    // application settings
    QSettings settings;

    std::unique_ptr<Ui::MainWindow> ui;
};

#endif  // MAINWINDOW_H
