#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow), activity({}) {
    ui->setupUi(this);
    activity.SetInstance(true);
    setupStatusbarWidgets();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::connectActivity() {
    if (connectionBtn->isEnabled()) {
        connectionBtn->setEnabled(false);
    }
    if (!ui->idEdit->isReadOnly()) {
        ui->idEdit->setReadOnly(true);
    }
    if (!ui->detailsEdit->isReadOnly()) {
        ui->detailsEdit->setReadOnly(true);
    }
    if (!ui->stateEdit->isReadOnly()) {
        ui->stateEdit->setReadOnly(true);
    }

    discord::Core::Create(discordId, DiscordCreateFlags_Default, &core);
    core->ActivityManager().UpdateActivity(activity, [this](discord::Result result) { on_activity_connected(result); });
    statusBar()->showMessage(tr("Connecting..."));
}

void MainWindow::disconnectActivity() {
    if (connectionBtn->isEnabled()) {
        connectionBtn->setEnabled(false);
    }

    core->ActivityManager().ClearActivity([this](discord::Result result) { on_activity_disconnected(result); });
    statusBar()->showMessage(tr("Disconnecting..."));
}

bool MainWindow::isActivityConnected() {
    return core != nullptr && _isActivityConnected;
}

void MainWindow::on_activity_connected(discord::Result result) {
    if (result != discord::Result::Ok) {
        disconnectActivity();
        return;
    }
    if (!connectionBtn->isEnabled()) {
        connectionBtn->setEnabled(true);
    }

    _isActivityConnected = true;
    connectionBtn->setText(tr("Disconnect"));
    statusBar()->showMessage(tr("Connected to Discord."));
}

void MainWindow::on_activity_disconnected(discord::Result) {
    if (core != nullptr) {
        delete core;
        core = nullptr;
    }
    if (!connectionBtn->isEnabled()) {
        connectionBtn->setEnabled(true);
    }
    if (ui->idEdit->isReadOnly()) {
        ui->idEdit->setReadOnly(false);
    }
    if (ui->detailsEdit->isReadOnly()) {
        ui->detailsEdit->setReadOnly(false);
    }
    if (ui->stateEdit->isReadOnly()) {
        ui->stateEdit->setReadOnly(false);
    }

    _isActivityConnected = false;
    connectionBtn->setText(tr("Connect"));
    statusBar()->showMessage(tr("Disconnected from Discord."));
}

void MainWindow::on_aboutQt_triggered() {
    rApp->aboutQt();
}

void MainWindow::on_idEdit_textChanged(const QString &text) {
    discordId = text.toLongLong();
}

void MainWindow::on_detailsEdit_textChanged(const QString &text) {
    activity.SetDetails(text.toUtf8().constData());
}

void MainWindow::on_stateEdit_textChanged(const QString &text) {
    activity.SetState(text.toUtf8().constData());
}

void MainWindow::on_connectionBtn_clicked(bool) {
    if (!isActivityConnected()) {
        connectActivity();
    } else {
        disconnectActivity();
    }
}

void MainWindow::setupStatusbarWidgets() {
    if (connectionBtn != nullptr) return;
    connectionBtn = new QPushButton(tr("Connect"), this);
    connectionBtn->setShortcut(tr("Alt+C"));
    connect(connectionBtn, &QPushButton::clicked, this, &MainWindow::on_connectionBtn_clicked);
    statusBar()->addPermanentWidget(connectionBtn);
}

bool MainWindow::event(QEvent *e) {
    if (core != nullptr) core->RunCallbacks();
    return QMainWindow::event(e);
}