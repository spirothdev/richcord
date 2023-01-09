#pragma once
#include <QDebug>
#include <QObject>

#include "app.h"
#include "discord.h"

/** These functions were used in the legacy Discord-RPC library.
void presenceReady(const DiscordUser *request);
void presenceDisconnected(int, const char *msg);
void presenceError(int code, const char *msg);
*/

class RichcordTesting : public QObject {
    Q_OBJECT

public:
    explicit RichcordTesting(QObject *parent = nullptr);
    ~RichcordTesting() override;

    void start();
    void stop();

private:
    bool isRunning = false;
};