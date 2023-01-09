#pragma once
#include <QApplication>
#include <QEvent>

#include "discord.h"

#define RCORD_APP RichcordApp::getAppInstance()

class RichcordApp : public QApplication {
public:
    RichcordApp(int argc, char *argv[]);

    discord::Core *getDiscordCore();

    static RichcordApp *getAppInstance();

protected:
    bool event(QEvent *e) override;

private:
    discord::Core *core;
};
