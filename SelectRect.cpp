#include "SelectRect.h"

#include <QMap>
#include <QDebug>

SelectRect::SelectRect(const QRect &parentRect):
    m_parentRect(parentRect)
{

}

SelectRect::SelectRect()
{

}

SelectRect::operator QRect() const
{
    QRect rect(this->topLeft(), this->bottomRight());
    return std::move(rect);
}

SelectRect::Orientation SelectRect::checkOrientation(const QPoint &point)
{
    Orientation orient = None;
// x coordinate
    if(point.x() == this->left()){
        orient = Orientation(orient|Left);
    }else if(point.x() == this->right()){
        orient = Orientation(orient|Right);
    }
// y coordinate
    if(point.y() == this->top()){
        orient = Orientation(orient|Top);
    }else if(point.y() == this->bottom()){
        orient =  Orientation(orient|Bottom);
    }

//    qDebug() <<"orientation is"<< orient;

    return orient;
}

Qt::CursorShape SelectRect::cursorShape(const QPoint &point)
{
    if(!this->contains(point)){
        return Qt::ArrowCursor;
    }

    //syscn with keyPoints
    const QMultiMap<Qt::CursorShape, Orientation> cursorShapes{
        {Qt::SizeFDiagCursor,LeftTop},
        {Qt::SizeVerCursor,Top},
        {Qt::SizeBDiagCursor,RightTop},
        {Qt::SizeHorCursor,Right},
        {Qt::SizeFDiagCursor,RightBottom},
        {Qt::SizeVerCursor,Bottom},
        {Qt::SizeBDiagCursor,LeftBottom},
        {Qt::SizeHorCursor,Left}};

    const Orientation& o = checkOrientation(point);
    if(o != None){
        return cursorShapes.key(o);
    }else/*if(this->contains(point,true))*/{
        return Qt::SizeAllCursor;
    }
}

void SelectRect::moveRect(const QPoint &movePos)
{
    SelectRect tempRect = *this;
//    qDebug()<< "source rect" << QRect(*this)<< "temp rect" <<
    tempRect;
    tempRect.moveTo(movePos);
    if(m_parentRect.contains(tempRect,true)){
        this->moveTo(movePos);
    }else{
        if(tempRect.top()<0){
            tempRect.moveTop(0);
        }

        if(tempRect.bottom()>m_parentRect.height()){
            tempRect.moveBottom(m_parentRect.height());
        }

        if(tempRect.left()<0){
            tempRect.moveLeft(0);
        }

        if(tempRect.right()>m_parentRect.width()){
            tempRect.moveRight(m_parentRect.width());
        }

        *this = std::move(tempRect);
    }
}

void SelectRect::resizeRect(const QPoint &pSource, const QPoint &resizeVector)
{
    if(None == m_orient){
        return ;
    }
    SelectRect tempRect = *this;
    tempRect.growRect(pSource,resizeVector);
    if(m_parentRect.contains(tempRect)){
        this->growRect(pSource,resizeVector);
    }else{
        if(tempRect.top()<0){
            tempRect.setTop(0);
        }

        if(tempRect.bottom()>m_parentRect.height()){
            tempRect.setBottom(m_parentRect.height());
        }

        if(tempRect.left()<0){
            tempRect.setLeft(0);
        }

        if(tempRect.right()>m_parentRect.width()){
            tempRect.setRight(m_parentRect.width());
        }
        *this = std::move(tempRect);
    }
}

void SelectRect::growRect(const QPoint &pSource, const QPoint &resizeVector)
{
    switch(m_orient){
    case LeftTop:
        this->setTopLeft(pSource+resizeVector);
        break;
    case Top:
        this->setTop(pSource.y()+resizeVector.y());
        break;
    case RightTop:
        this->setTopRight(pSource+resizeVector);
        break;
    case Right:
        this->setRight(pSource.x()+resizeVector.x());
        break;
    case RightBottom:
        this->setBottomRight(pSource+resizeVector);
        break;
    case Bottom:
        this->setBottom(pSource.y()+resizeVector.y());
        break;
    case LeftBottom:
        this->setBottomLeft(pSource+resizeVector);
        break;
    case Left:
        this->setLeft(pSource.x()+resizeVector.x());
        break;
    default:
        break;
    }
}

void SelectRect::setOrient(const Orientation &orient)
{
    m_orient = orient;
}

QRect SelectRect::parentRect() const
{
    return m_parentRect;
}

void SelectRect::setParentRect(const QRect &parentRect)
{
    m_parentRect = parentRect;
}

void SelectRect::drawSelectFrame(QPainter &paint)
{
    paint.save();
    paint.setPen(QPen(QColor("skyblue"),m_lineWidth));
    paint.drawRect(*this);
    paint.setPen(QPen(QColor("lightblue"),1));
    paint.setBrush(Qt::white);
    paint.drawEllipse(this->topLeft(),m_pointRadius,m_pointRadius);
    paint.drawEllipse(this->topRight(),m_pointRadius,m_pointRadius);
    paint.drawEllipse(this->bottomLeft(),m_pointRadius,m_pointRadius);
    paint.drawEllipse(this->bottomRight(),m_pointRadius,m_pointRadius);
    paint.drawEllipse((this->topLeft()+this->topRight())/2,m_pointRadius,m_pointRadius);
    paint.drawEllipse((this->topRight()+this->bottomRight())/2,m_pointRadius,m_pointRadius);
    paint.drawEllipse((this->bottomRight()+this->bottomLeft())/2,m_pointRadius,m_pointRadius);
    paint.drawEllipse((this->bottomLeft()+this->topLeft())/2,m_pointRadius,m_pointRadius);

    paint.restore();
}
