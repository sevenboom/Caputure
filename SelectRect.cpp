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

SelectRect::Orientation SelectRect::checkOrientation(const QPoint &point,QPoint &extraPoint)
{
    Orientation orient = None;
    // point orientation
    const QVector<QPoint> orientPoint{
        this->topLeft(),(this->topLeft()+this->topRight())/2,
        this->topRight(),(this->topRight()+this->bottomRight())/2,
        this->bottomRight(),(this->bottomRight()+this->bottomLeft())/2,
        this->bottomLeft(),(this->bottomLeft()+this->topLeft())/2
    };

    for(int i = 0; i <orientPoint.size();++i){
        if(QLineF(orientPoint.at(i),point).length()<m_pointRadius){
            orient = static_cast<Orientation>(i);
            extraPoint = orientPoint.at(i);
            break;
        }
    }

    if(None == orient){
        // x coordinate
        if(point.x() == this->left()){
            orient =Left;
        }else if(point.x() == this->right()){
            orient = Right;
        }
        // y coordinate
        if(point.y() == this->top()){
            orient = Top;
        }else if(point.y() == this->bottom()){
            orient = Bottom;
        }
    }
    return orient;
}

Qt::CursorShape SelectRect::cursorShape(const QPoint &point)
{
    //sync with orientPoint
    const static QVector<Qt::CursorShape> cursorShapes{
       Qt::SizeFDiagCursor,
       Qt::SizeVerCursor,
       Qt::SizeBDiagCursor,
       Qt::SizeHorCursor,
       Qt::SizeFDiagCursor,
       Qt::SizeVerCursor,
       Qt::SizeBDiagCursor,
       Qt::SizeHorCursor};

    const Orientation& o = checkOrientation(point);
    if(o != None){
        return cursorShapes.at(static_cast<int>(o));
    }else{
        if(this->contains(point,true)){
            return Qt::SizeAllCursor;
        }
        return  Qt::ArrowCursor;
    }
}

void SelectRect::moveRect(const QPoint &movePos)
{
    SelectRect tempRect = *this;

    tempRect.moveTo(movePos);
    if(m_parentRect.contains(tempRect,true)){
        this->moveTo(movePos);
    }else{
        if(tempRect.top()<0){
            tempRect.moveTop(0);
        }else if(tempRect.top()>m_parentRect.height()){
            tempRect.moveTop(m_parentRect.height());
        }

        if(tempRect.bottom()>m_parentRect.height()){
            tempRect.moveBottom(m_parentRect.height());
        }else if(tempRect.bottom()<0){
            tempRect.moveBottom(0);
        }

        if(tempRect.left()<0){
            tempRect.moveLeft(0);
        }else if(tempRect.left() > m_parentRect.width()){
            tempRect.moveLeft(m_parentRect.width());
        }

        if(tempRect.right()>m_parentRect.width()){
            tempRect.moveRight(m_parentRect.width());
        }else if(tempRect.right()<0){
            tempRect.moveRight(0);
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

QRect SelectRect::absoluteRect()
{
    int&& left = this->left();
    int&& top = this->top();
    if(this->right()<this->left()){
        left = this->right();
    }
    if(this->bottom()<this->top()){
        top = this->bottom();
    }
    return QRect(left,top,qAbs(this->width()),qAbs(this->height()));
}

QRect SelectRect::parentRect() const
{
    return m_parentRect;
}

void SelectRect::setParentRect(const QRect &parentRect)
{
    m_parentRect = parentRect;
}

void SelectRect::drawSelectBorder(QPainter &paint)
{
    paint.save();
    paint.setPen(QPen(QColor("#7acdfd"),m_lineWidth));
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
