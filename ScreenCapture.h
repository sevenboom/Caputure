#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <QWidget>
#include <memory>

#include "SelectRect.h"

class Toolbar;

class ScreenCapture : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenCapture(QWidget *parent = 0);
    void setTarget(QPixmap && pix);
    QPixmap seletedPix();

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void capture();

private:
    Toolbar* m_toolbar;

    QPixmap m_bk;// background image
    QPixmap m_sourceBk;

    SelectRect m_selectRect;// caputure area

    bool m_isPressed = false;

    enum Action{
        Drag,
        Resize,
        Nothing
    };
    Action m_action = Nothing;

    QPoint m_pStart;
    QPoint m_pMove;
    QPoint m_pEnd;

    void showToolBar();

};





#endif // SCREENCAPTURE_H
