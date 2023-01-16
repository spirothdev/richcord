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

discord::Core *RichcordApp::getDiscordCore() {
    return _discordCore;
}

bool RichcordApp::isActivityShown() {
    return _isActivityShown;
}

long long RichcordApp::getDiscordAppId() {
    return _discordAppId;
}

void RichcordApp::changeDiscordAppId(long long id) {
    bool wasActivityShown = _isActivityShown;
    if (_discordAppId == id) return;
    if (_discordCore != nullptr) destroyDiscordCore();
    _discordAppId = id;
    initDiscordCore();
    if (wasActivityShown) applyActivityChanges();
}

void RichcordApp::applyActivityChanges() {
    if (_discordCore == nullptr) {
        qWarning() << "applyActivityChanges was called before GameSDK was even initialized.";
        return;
    }
    if (_isActivityShown) {
        clearActivity();
    }

    _discordCore->ActivityManager().UpdateActivity(_activity, [this](discord::Result result) {
        if (result != discord::Result::Ok && result != discord::Result::TransactionAborted) {
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
    if (_isActivityShown) applyActivityChanges();
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

    connect(this, &RichcordApp::activityChangeStarted, this, [this]() {
        qDebug() << "Applying changes to activity...";
    });
    connect(this, &RichcordApp::activityChangeSuccess, this, [this]() {
        qDebug() << "Changes to activity applied successfully.";
        _isActivityShown = true;
    });
    connect(this, &RichcordApp::activityChangeFailure, this, [this](discord::Result code) {
        auto codeString = QString::number(static_cast<double>(code));
        qCritical() << "An error occurred while applying activity changes:" << codeString;
    });
}

void RichcordApp::clearActivity() {
    if (_discordCore == nullptr) {
        qWarning() << "clearActivity was called before GameSDK was even initialized.";
        return;
    }
    if (!_isActivityShown) {
        qWarning() << "clearActivity was called but activity is already cleared.";
        return;
    }

    _discordCore->ActivityManager().ClearActivity([this](discord::Result result) {
        if (result != discord::Result::Ok && result != discord::Result::TransactionAborted) {
            auto resultStr = QString::number(static_cast<double>(result));
            qWarning() << "An activity error occurred while clearing:" << resultStr;
        }

        _isActivityShown = false;
    });
}

void RichcordApp::initDiscordCore() {
    discord::Core::Create(_discordAppId, DiscordCreateFlags_Default, &_discordCore);
    qDebug() << "Initialized Discord GameSDK with App ID:" << QString::number(_discordAppId);
}

void RichcordApp::destroyDiscordCore() {
    if (_isActivityShown) {
        // We don't use clearActivity here as it doesn't know when it will receive the callback
        _discordCore->ActivityManager().ClearActivity([this](discord::Result result) {
            if (result != discord::Result::Ok && result != discord::Result::TransactionAborted) {
                auto resultStr = QString::number(static_cast<double>(result));
                qWarning() << "An activity error occurred while destroying Discord GameSDK:" << resultStr;
            }

            _isActivityShown = false;
            delete _discordCore;
            _discordCore = nullptr;
        });
    } else {
        delete _discordCore;
        _discordCore = nullptr;
    }
}
