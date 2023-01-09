#include "app.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    RichcordApp app(argc, argv);
    MainWindow window;
    window.show();
    return RichcordApp::exec();
}
