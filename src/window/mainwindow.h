#pragma once
#include <QDebug>
#include <QFile>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeySequence>
#include <QMainWindow>
#include <QShortcut>
#include <QString>

#include "app.h"
#include "discord.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:
    void on_aboutQt_triggered();

private:
    void setupStyleSheet();
    void initShortcuts();

    Ui::MainWindow *ui;
};
