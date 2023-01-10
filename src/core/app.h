#pragma once
#include <QApplication>

#define rApp RichcordApp::getCurrentInstance()

class RichcordApp : public QApplication {
public:
    RichcordApp(int argc, char *argv[]);

    static RichcordApp *getCurrentInstance();
};
