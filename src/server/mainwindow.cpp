#include "mainwindow.h"

#include <QtGui/QMessageBox>

#include "ui_mainwindow.h"

MainWindow::MainWindow() :
    settings("settings.ini", QSettings::IniFormat),
    ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::init()
{
    readSettings();

    QObject::connect(&server,
        SIGNAL(socketStateChanged(QAbstractSocket::SocketState)),
        SLOT(serverSocketStateChanged(QAbstractSocket::SocketState)));
}

void MainWindow::readSettings()
{
    ui->addressLineEdit->setText(settings.value("Address").toString());
    ui->portLineEdit->setText(settings.value("Port").toString());
}

void MainWindow::saveSettings()
{
    settings.setValue("Address", ui->addressLineEdit->text());
    settings.setValue("Port", ui->portLineEdit->text());
}

Q_SLOT void MainWindow::serverSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    ui->socketStateLineEdit->setText(QString::number(socketState));
}

void MainWindow::on_startButton_clicked()
{
    server.start(ui->addressLineEdit->text(),
        ui->portLineEdit->text().toUInt());
}

void MainWindow::on_stopButton_clicked()
{
    server.stop();
}

void MainWindow::on_clientsButton_clicked()
{
    QMessageBox::information(this,
        QObject::tr("Client List"),
        server.clients().at(0));
}
