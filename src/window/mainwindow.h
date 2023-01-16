#pragma once
#include <QChar>
#include <QDateTime>
#include <QDebug>
#include <QInputDialog>
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
    void on_reconnect_clicked();
    void on_previewAppIdEdit_clicked();
    void on_previewDetails_clicked();
    void on_previewState_clicked();

private:
    void syncPreviewFromActivity();
    void enablePreviewEditing();
    void disablePreviewEditing();
    void connectActivityCallbacks();
    void initShortcuts();
    void initialize();

    Ui::MainWindow *ui;
};
