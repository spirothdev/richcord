#pragma once
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QEvent>
#include <QString>
#include <QStyleFactory>

#include "discord.h"

#define rApp RichcordApp::getCurrentInstance()

class RichcordApp : public QApplication {
    Q_OBJECT

public:
    RichcordApp(int argc, char *argv[]);

    ~RichcordApp() override;

    discord::Activity getActivity();
    discord::Core *getDiscordCore();
    bool isActivityShown();
    long long getDiscordAppId();
    void changeDiscordAppId(long long id);
    void applyActivityChanges();
    void resetActivityToDefault();

    static RichcordApp *getCurrentInstance();

signals:
    void activityChangeStarted();
    void activityChangeSuccess();
    void activityChangeFailure(discord::Result code);

protected:
    bool event(QEvent *e) override;

private:
    void setupActivityCallbacks();
    void clearActivity();
    void initDiscordCore();
    void destroyDiscordCore();

    discord::Activity _activity{};
    discord::Core *_discordCore = nullptr;
    bool _isActivityShown = false;
    long long _discordAppId = 1060543817830375476; // Default App Id
};
