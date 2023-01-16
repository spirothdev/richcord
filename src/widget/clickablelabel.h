#pragma once
#include <QLabel>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QString>
#include <QWidget>

class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    ClickableLabel(const QString &text, QWidget *parent = nullptr);
    ClickableLabel(QWidget *parent = nullptr);

public slots:
    void click();

protected:
    void keyPressEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;

signals:
    void clicked();
};
