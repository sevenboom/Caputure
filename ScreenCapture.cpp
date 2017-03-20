#include "ScreenCapture.h"

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
//    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setMouseTracking(true);

    const int &dWidth = QApplication::desktop()->width();
    const int &dHeight = QApplication::desktop()->height();
    this->setFixedSize(dWidth, dHeight);
    m_bk = QPixmap(this->size());
    m_selectRect.setParentRect(this->rect());
    m_selectRect.setRect(this->width()/4,this->height()/4,this->width()/2, this->height()/2);
}

void ScreenCapture::setTarget(QPixmap &&pix)
{
    m_sourceBk = pix;
}

QPixmap &&ScreenCapture::seletedPix()
{
//    if
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
    m_selectRect.drawSelectFrame(paint);
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
//        this->close();
        qApp->quit();
    }
    if(e->button() == Qt::LeftButton){
        m_isPressed = true;
        m_pStart = e->globalPos();
        if(m_selectRect.contains(e->globalPos(),true)){
            m_action = Drag;
        }else{
            const SelectRect::Orientation &orient = m_selectRect.checkOrientation(e->globalPos());
            if(orient != SelectRect::None){
                m_action = Resize;
                m_selectRect.setOrient(orient);
            }
//            m_selectRect.setTopLeft(e->globalPos()); //select mode
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
//            m_selectRect.setBottomRight(e->globalPos());// slected mode
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
}

void ScreenCapture::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
//        this->close();
        qApp->quit();
    case Qt::Key_Control|Qt::Key_Alt|Qt::Key_Q:

        this->show();
    default:
        break;
    }
}


/************************************************************************!
 * \brief CaptureControl
 ************************************************************************/

//#include <QPushButton>

//CaptureControl::CaptureControl(QWidget *parent):
//    QWidget(parent)
//{
//    this->setFixedSize(120,30);

//    m_cancel = new QPushButton(QStringLiteral("取消"), this);
//    m_sure = new QPushButton(QStringLiteral("确定"), this);

//    QHBoxLayout *layout = new QHBoxLayout(this);
//    layout->addWidget(m_cancel);
//    layout->addWidget(m_sure);
//    layout->setMargin(5);
//    layout->setSpacing(10);
//}

//void CaptureControl::savePix()
//{
////    m_sourceBk
//}
