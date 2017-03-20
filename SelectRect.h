#ifndef SELECTRECT_H
#define SELECTRECT_H

#include <QRect>
#include <QPainter>

class SelectRect : public QRect
{
public:
    enum Orientation{
        None = 0x0000,
        Top =    0x0001,
        Right =  0x0002,
        Left =   0x0004,
        Bottom = 0x0008,
        RightTop = Top|Right,
        LeftTop = Left|Top,
        RightBottom = Right|Bottom,
        LeftBottom = Left|Bottom
    };
    SelectRect();
    explicit SelectRect(const QRect &parentRect);
    explicit operator QRect() const;
    ~SelectRect() = default;

    void drawSelectFrame(QPainter &paint);
    Orientation checkOrientation(const QPoint& point);
    Qt::CursorShape cursorShape(const QPoint &point);

    void moveRect(const QPoint& movePos);
    void resizeRect(const QPoint &pSource, const QPoint& resizeVector);

    QRect parentRect() const;
    void setParentRect(const QRect &parentRect);

    void setOrient(const Orientation &orient);

private:
    void growRect(const QPoint &pSource, const QPoint &resizeVector);

    Orientation m_orient = None;
    QRect m_parentRect;
    int m_pointRadius = 4;
    int m_lineWidth = 2;
};

#endif // SELECTRECT_H
