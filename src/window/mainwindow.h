#pragma once
#include <QEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QString>

#include "app.h"
#include "discord.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    void connectActivity();
    void disconnectActivity();
    bool isActivityConnected();

public slots:
    void on_activity_connected(discord::Result result);
    void on_activity_disconnected(discord::Result result);
    void on_aboutQt_triggered();
    void on_idEdit_textChanged(const QString &text);
    void on_detailsEdit_textChanged(const QString &text);
    void on_stateEdit_textChanged(const QString &text);
    void on_connectionBtn_clicked(bool checked = false);

protected:
    bool event(QEvent *e) override;

private:
    void setupStatusbarWidgets();

    long long discordId = 1060543817830375476;
    bool _isActivityConnected = false;
    discord::Activity activity{};
    discord::Core *core = nullptr;
    QPushButton *connectionBtn = nullptr;
    Ui::MainWindow *ui;
};