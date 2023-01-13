#include "app.h"

RichcordApp::RichcordApp(int argc, char *argv[])
    : QApplication(argc, argv) {
    initDiscordCore();
    setupActivityCallbacks();
    resetActivityToDefault();
}

RichcordApp::~RichcordApp() {
    destroyDiscordCore();
}

discord::Activity RichcordApp::getActivity() {
    return _activity;
}

discord::User *RichcordApp::getDiscordUser() {
    return _discordUser;
}

discord::Core *RichcordApp::getDiscordCore() {
    return _discordCore;
}

long long RichcordApp::getDiscordAppId() {
    return _discordAppId;
}

void RichcordApp::changeDiscordAppId(long long id) {
    if (_discordCore != nullptr) destroyDiscordCore();
    if (_discordAppId != id) _discordAppId = id;
    initDiscordCore();
    applyActivityChanges();
}

void RichcordApp::applyActivityChanges() {
    if (_discordCore == nullptr) {
        qWarning() << "applyActivityChanges was called before GameSDK was even initialized.";
        return;
    }

    _discordCore->ActivityManager().UpdateActivity(_activity, [this](discord::Result result) {
        if (result != discord::Result::Ok) {
            emit activityChangeFailure(result);
            return;
        }

        emit activityChangeSuccess();
    });

    emit activityChangeStarted();
}

void RichcordApp::resetActivityToDefault() {
    if (_discordCore == nullptr) {
        qWarning() << "Cannot reset activity to default if GameSDK is destroyed.";
        return;
    }

    _activity.SetDetails("version 1.0.0");
    _activity.SetState("Idle");
    _activity.GetAssets().SetLargeImage("default");
    _activity.GetTimestamps().SetStart(QDateTime::currentMSecsSinceEpoch());
    _activity.SetInstance(!_activity.GetInstance());
    applyActivityChanges();
}

RichcordApp *RichcordApp::getCurrentInstance() {
    return dynamic_cast<RichcordApp*>(instance());
}

bool RichcordApp::event(QEvent *e) {
    if (_discordCore != nullptr) _discordCore->RunCallbacks();
    return QApplication::event(e);
}

void RichcordApp::setupActivityCallbacks() {
    if (_discordCore == nullptr) {
        qWarning() << "Setting up activity events but GameSDK is destroyed.";
        return;
    }

    connect(this, &RichcordApp::activityChangeSuccess, this, [this]() {
        qDebug() << "Changes to activity applied successfully.";
    });
    connect(this, &RichcordApp::activityChangeFailure, this, [this](discord::Result code) {
        auto codeString = QString::number(static_cast<double>(code));
        qCritical() << "An error occurred while applying activity changes: " << codeString;
    });
}

void RichcordApp::setupDiscordUserEvents() {
    if (_discordCore == nullptr) {
        qWarning() << "Setting up user events but GameSDK is destroyed.";
        return;
    }

    _discordCore->UserManager().OnCurrentUserUpdate.Connect([this]() {
        _discordCore->UserManager().GetCurrentUser(_discordUser);
        emit discordUserUpdated(_discordUser);
    });
}

void RichcordApp::initDiscordCore() {
    discord::Core::Create(_discordAppId, DiscordCreateFlags_Default, &_discordCore);
    setupDiscordUserEvents();
}

void RichcordApp::destroyDiscordCore() {
    delete _discordCore;
}
