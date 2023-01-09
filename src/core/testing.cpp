#include "testing.h"

/**
void presenceReady(const DiscordUser *request) {
    qInfo() << "Connected to user ID: " << request->userId;
}

void presenceDisconnected(int, const char *msg) {
    qInfo() << "Presence disconnected from client: " << msg;
}

void presenceError(int code, const char *msg) {
    qInfo() << "Presence error: (error code " << code << ") " << msg;
}
*/

RichcordTesting::RichcordTesting(QObject *parent)
    : QObject(parent) {
}

RichcordTesting::~RichcordTesting() = default;

void RichcordTesting::start() {
    if (isRunning) {
        qWarning() << "Presence testing has already started.";
        return;
    }

    discord::Activity activity = discord::Activity();
    activity.SetType(discord::ActivityType::Watching);
    activity.SetState("Currently testing Rich Presence!");
    activity.SetDetails("what if matulog ta?");
    activity.SetInstance(true);

    RCORD_APP->getDiscordCore()->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        if (result != discord::Result::Ok) {
            qWarning() << "Cannot display Rich Presence.";
            return;
        }
    });
}

void RichcordTesting::stop() {
    if (!isRunning) {
        qWarning() << "Presence testing hasn't been started.";
        return;
    }

    RCORD_APP->getDiscordCore()->ActivityManager().ClearActivity([](discord::Result result) {
        if (result != discord::Result::Ok) {
            return;
        }
    });
}