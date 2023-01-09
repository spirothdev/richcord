#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    RichcordTesting testing(this);
    testing.start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_aboutQt_triggered() {
    qApp->aboutQt();
}