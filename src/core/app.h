#pragma once
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QEvent>
#include <QString>

#include "discord.h"

#define rApp RichcordApp::getCurrentInstance()

class RichcordApp : public QApplication {
    Q_OBJECT

public:
    RichcordApp(int argc, char *argv[]);

    ~RichcordApp() override;

    discord::Activity getActivity();
    discord::User *getDiscordUser();
    discord::Core *getDiscordCore();
    long long getDiscordAppId();
    void changeDiscordAppId(long long id);
    void applyActivityChanges();
    void resetActivityToDefault();

    static RichcordApp *getCurrentInstance();

signals:
    void activityChangeStarted();
    void activityChangeSuccess();
    void activityChangeFailure(discord::Result code);
    void discordUserUpdated(discord::User *user);

protected:
    bool event(QEvent *e) override;

private:
    void setupActivityCallbacks();
    void setupDiscordUserEvents();
    void initDiscordCore();
    void destroyDiscordCore();

    discord::Activity _activity{};
    discord::User *_discordUser = nullptr;
    discord::Core *_discordCore = nullptr;
    long long _discordAppId = 1060543817830375476; // Default App Id
};
