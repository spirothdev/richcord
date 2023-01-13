#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->menubar->hide();
    initShortcuts();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_aboutQt_triggered() {
    rApp->aboutQt();
}

void MainWindow::setupStyleSheet() {
    QFile styleSheetFile(":/stylesheets/discord.qss");

    if (!styleSheetFile.exists()) {
        qWarning() << "Cannot find specified stylesheet for " << metaObject()->className();
        return;
    }

    styleSheetFile.open(QFile::ReadOnly);
    setStyleSheet(QLatin1String(styleSheetFile.readAll()));
    ensurePolished();
    styleSheetFile.close();
}

void MainWindow::initShortcuts() {
    new QShortcut(Qt::Key_Escape, this, this, [this]() {
        menuBar()->setVisible(!menuBar()->isVisible());
    });
}
