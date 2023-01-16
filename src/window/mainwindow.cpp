#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initialize();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_aboutQt_triggered() {
    rApp->aboutQt();
}

void MainWindow::on_reconnect_clicked() {
    rApp->applyActivityChanges();
    syncPreviewFromActivity();
}

void MainWindow::on_previewAppIdEdit_clicked() {
    auto *idInputDialog = new QInputDialog(this);
    idInputDialog->setWindowTitle(tr("Modify"));
    idInputDialog->setInputMode(QInputDialog::TextInput);
    idInputDialog->setLabelText(tr("Application ID:"));
    idInputDialog->setOkButtonText(tr("Save"));
    idInputDialog->setTextValue(ui->previewAppIdText->text());

    connect(idInputDialog, &QInputDialog::textValueSelected, this, [this](const QString &text) {
        QString newId = text;

        if (newId.length() < 1) {
            newId = ui->previewAppIdText->text();
        }
        for (const QChar letter : qAsConst(newId)) {
            if (letter.isDigit()) continue;
            newId.remove(letter);
        }

        ui->previewAppIdText->setText(newId);
        rApp->changeDiscordAppId(newId.toLongLong());
    });

    idInputDialog->show();
}

void MainWindow::on_previewDetails_clicked() {
    auto *detailsInputDialog = new QInputDialog(this);
    detailsInputDialog->setWindowTitle(tr("Modify"));
    detailsInputDialog->setInputMode(QInputDialog::TextInput);
    detailsInputDialog->setLabelText(tr("Details:"));
    detailsInputDialog->setOkButtonText(tr("Save"));
    detailsInputDialog->setTextValue(ui->previewDetails->text());

    connect(detailsInputDialog, &QInputDialog::textValueSelected, this, [this](const QString &text) {
        QString newDetails = text;

        if (newDetails.length() < 1) {
            newDetails = ui->previewDetails->text();
        }

        ui->previewDetails->setText(newDetails);
        rApp->getActivity().SetDetails(newDetails.toUtf8().constData());
        rApp->applyActivityChanges();
    });

    detailsInputDialog->show();
}

void MainWindow::on_previewState_clicked() {
    auto *stateInputDialog = new QInputDialog(this);
    stateInputDialog->setWindowTitle(tr("Modify"));
    stateInputDialog->setInputMode(QInputDialog::TextInput);
    stateInputDialog->setLabelText(tr("State:"));
    stateInputDialog->setOkButtonText(tr("Save"));
    stateInputDialog->setTextValue(ui->previewState->text());

    connect(stateInputDialog, &QInputDialog::textValueSelected, this, [this](const QString &text) {
        QString newState = text;

        if (newState.length() < 1) {
            newState = ui->previewState->text();
        }

        ui->previewState->setText(newState);
        rApp->getActivity().SetState(newState.toUtf8().constData());
        rApp->applyActivityChanges();
    });

    stateInputDialog->show();
}

void MainWindow::syncPreviewFromActivity() {
    auto activity = rApp->getActivity();
    ui->previewAppIdText->setText(QString::number(rApp->getDiscordAppId()));
    ui->previewDetails->setText(activity.GetDetails());
    ui->previewState->setText(activity.GetState());
}

void MainWindow::enablePreviewEditing() {
    ui->previewAppIdText->setEnabled(true);
    ui->previewAppIdEdit->setEnabled(true);
    ui->previewDetails->setEnabled(true);
    ui->previewState->setEnabled(true);
    ui->previewAssets->setEnabled(true);
}

void MainWindow::disablePreviewEditing() {
    ui->previewAppIdText->setEnabled(false);
    ui->previewAppIdEdit->setEnabled(false);
    ui->previewDetails->setEnabled(false);
    ui->previewState->setEnabled(false);
    ui->previewAssets->setEnabled(false);
}

void MainWindow::connectActivityCallbacks() {
    connect(rApp, &RichcordApp::activityChangeStarted, this, [this]() {
        disablePreviewEditing();
        ui->statusBar->showMessage(tr("Applying activity changes..."));
    });
    connect(rApp, &RichcordApp::activityChangeSuccess, this, [this]() {
        enablePreviewEditing();
        syncPreviewFromActivity();
        ui->reconnect->setVisible(false);
        ui->statusBar->showMessage(tr("Activity changes applied successfully."), 10000);
    });
    connect(rApp, &RichcordApp::activityChangeFailure, this, [this](discord::Result code) {
        auto codeDouble = static_cast<double>(code);
        enablePreviewEditing();
        ui->reconnect->setVisible(true);
        ui->statusBar->showMessage(tr("Failed to apply activity changes. (error code: %1)").arg(codeDouble));
    });
}

void MainWindow::initShortcuts() {
    new QShortcut(Qt::Key_Escape, this, this, [this]() {
        // Toggle menu bar using escape key
        menuBar()->setVisible(!menuBar()->isVisible());
    });
}

void MainWindow::initialize() {
    // If running on Windows, set the background color to white
#ifdef Q_OS_WINDOWS
    setStyleSheet("QMainWindow { background-color: white; }");
#endif

    // Hide menu bar and initialize window shortcuts
    ui->menuBar->hide();
    initShortcuts();

    // Connect window to activity callbacks, and apply pending activity changes
    connectActivityCallbacks();
    syncPreviewFromActivity();
    rApp->applyActivityChanges();
}
