#include "app.h"

RichcordApp::RichcordApp(int argc, char *argv[])
    : QApplication(argc, argv) {}

RichcordApp *RichcordApp::getCurrentInstance() {
    return dynamic_cast<RichcordApp*>(instance());
}