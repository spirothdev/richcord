#include "clickablelabel.h"

ClickableLabel::ClickableLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent) {}

ClickableLabel::ClickableLabel(QWidget *parent)
    : QLabel(parent) {}

void ClickableLabel::click() {
    emit clicked();
}

void ClickableLabel::keyPressEvent(QKeyEvent *ev) {
    click();
    QLabel::keyPressEvent(ev);
}

void ClickableLabel::mousePressEvent(QMouseEvent *ev) {
    click();
    QLabel::mousePressEvent(ev);
}
