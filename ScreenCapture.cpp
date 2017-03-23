#include "ScreenCapture.h"
#include "Toolbar.h"

#include <QMouseEvent>
#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>

ScreenCapture::ScreenCapture(QWidget *parent) : QWidget(parent),
  m_selectRect(this->rect())
{
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setMouseTracking(true);

    const int &dWidth = QApplication::desktop()->width();
    const int &dHeight = QApplication::desktop()->height();
    this->setFixedSize(dWidth, dHeight);
    m_bk = QPixmap(this->size());
    m_selectRect.setParentRect(this->rect());
    m_selectRect.setRect(this->width()/4,this->height()/4,this->width()/2, this->height()/2);

    m_toolbar =new Toolbar(this);
    m_toolbar->setMaximumSize(120,50);
    m_toolbar->hide();
}

void ScreenCapture::setTarget(QPixmap &&pix)
{
    m_sourceBk = pix;
}

QPixmap ScreenCapture::seletedPix()
{
    return m_sourceBk.copy(m_selectRect);
}

void ScreenCapture::paintEvent(QPaintEvent *)
{
    m_bk.fill(QColor(0,0,0,100));

    QPainter paint;

    paint.begin(&m_bk);
    paint.save();
    //sub the select rect
    paint.setCompositionMode(QPainter::CompositionMode_SourceOut);
    paint.fillRect(m_selectRect,QColor(0,0,0,1));
    paint.restore();
    m_selectRect.drawSelectBorder(paint);
    paint.end();

    paint.begin(this);
    paint.drawPixmap(this->rect(),m_sourceBk);
    paint.setCompositionMode(QPainter::CompositionMode_SourceOver);
    paint.drawPixmap(this->rect(),m_bk);
    paint.end();

}

void ScreenCapture::mousePressEvent(QMouseEvent *e)
{

    if(e->button() == Qt::RightButton){
        qApp->quit();
    }
    if(e->button() == Qt::LeftButton){
        m_toolbar->hide();
        m_isPressed = true;
        m_pStart = e->globalPos();

        QPoint extraPoint;
        const SelectRect::Orientation &orient = m_selectRect.checkOrientation(e->globalPos(),extraPoint);

        if(orient != SelectRect::None){
            m_action = Resize;
            m_selectRect.setOrient(orient);
            // 判断实际精确点是不是鼠标的点，不是则需要改变选框的大小，使鼠标在选框精确的方位点上
            if(!extraPoint.isNull()){
                m_selectRect.resizeRect(extraPoint,e->globalPos()-extraPoint);
                update();
            }
        }else if(m_selectRect.contains(e->globalPos(),true)){
            m_action = Drag;
        }
    }
}


void ScreenCapture::mouseMoveEvent(QMouseEvent *e)
{
    if(m_isPressed){
        if(m_pMove.isNull()){
            m_pMove = m_pStart;
        }

        if(Drag == m_action){
            m_selectRect.moveRect(e->globalPos() - m_pMove+m_selectRect.topLeft());
        }else if(Resize == m_action){
            m_selectRect.resizeRect(m_pStart,e->globalPos()-m_pStart);
        }

        m_pMove = e->globalPos();
    }
    else{
        this->setCursor(m_selectRect.cursorShape(e->globalPos()));
    }
    update();
}

void ScreenCapture::mouseReleaseEvent(QMouseEvent *e)
{
    if(m_isPressed){
        if(m_pMove.isNull()){
            m_pMove = m_pStart;
        }

        if(Drag == m_action){
            m_selectRect.moveRect(e->globalPos() - m_pMove+m_selectRect.topLeft());
        }else if(Resize == m_action){
            m_selectRect.resizeRect(m_pStart,e->globalPos()-m_pStart);
//            m_selectRect.setBottomRight(e->globalPos());
        }
    }

    m_pMove.setX(0),m_pMove.setY(0);
    m_isPressed = false;
    m_action = Nothing;
    update();
    showToolBar();
}

void ScreenCapture::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        qApp->quit();
    case Qt::Key_Control|Qt::Key_Alt|Qt::Key_Q:

        this->show();
    default:
        break;
    }
}

void ScreenCapture::showToolBar()
{
    const QRect& rect = m_selectRect.absoluteRect();

    const int &x = rect.right()-m_toolbar->width();
    int &&y = rect.bottom();

    if(y+m_toolbar->height() > this->height()){
        y = rect.top()-m_toolbar->height();
        if(y<0){
            y = 0;
        }
    }

    m_toolbar->move(x,y);
    m_toolbar->show();
    m_toolbar->raise();

    qDebug() << m_toolbar->geometry() << m_toolbar->isVisible();
}
