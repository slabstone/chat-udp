#include "mainwindow.h"

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
}

void MainWindow::readSettings()
{
    ui->addressLineEdit->setText(settings.value("Address").toString());
    ui->portLineEdit->setText(settings.value("Port").toString());
    ui->nameLineEdit->setText(settings.value("Name").toString());
}

void MainWindow::saveSettings()
{
    settings.setValue("Address", ui->addressLineEdit->text());
    settings.setValue("Port", ui->portLineEdit->text());
    settings.setValue("Name", ui->nameLineEdit->text());
}

void MainWindow::onReturnPressed()
{
    client.sendMessage(ui->inputLineEdit->text());
    ui->inputLineEdit->clear();
}

void MainWindow::onMessageReceived(const QString& message)
{
    ui->chatTextEdit->appendPlainText(message);
}

void MainWindow::on_connectButton_clicked()
{
    client.setName(ui->nameLineEdit->text());
    client.connect(ui->addressLineEdit->text(),
        ui->portLineEdit->text().toUInt());
    QObject::connect(&client,
        SIGNAL(messageReceived(QString)),
        SLOT(onMessageReceived(QString)));
    QObject::connect(ui->inputLineEdit,
        SIGNAL(returnPressed()),
        SLOT(onReturnPressed()));
}

void MainWindow::on_disconnectButton_clicked()
{
    client.disconnect();
}
