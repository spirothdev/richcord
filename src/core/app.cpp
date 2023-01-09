#include "app.h"

RichcordApp::RichcordApp(int argc, char *argv[])
    : QApplication(argc, argv) {
    discord::Core::Create(1060543817830375476, DiscordCreateFlags_Default, &core);
}

discord::Core *RichcordApp::getDiscordCore() {
    return core;
}

RichcordApp *RichcordApp::getAppInstance() {
    return dynamic_cast<RichcordApp*>(instance());
}

bool RichcordApp::event(QEvent *e) {
    core->RunCallbacks();
    return QApplication::event(e);
}