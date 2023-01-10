#pragma once
#include <QApplication>
#include <QEvent>

#define rApp RichcordApp::getAppInstance()

class RichcordApp : public QApplication {
public:
    RichcordApp(int argc, char *argv[]);

    static RichcordApp *getCurrentInstance();
};
