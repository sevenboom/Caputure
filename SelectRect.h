#ifndef SELECTRECT_H
#define SELECTRECT_H

#include <QRect>
#include <QPainter>

class SelectRect : public QRect
{
public:
    enum Orientation{
        None = -1,
        LeftTop =  0,
        Top     =  1,
        RightTop = 2,
        Right,
        RightBottom,
        Bottom,
        LeftBottom,
        Left
    };

    SelectRect();
    explicit SelectRect(const QRect &parentRect);
    explicit operator QRect() const;
    ~SelectRect() = default;

    void drawSelectBorder(QPainter &paint);
    Orientation checkOrientation(const QPoint& point, QPoint &extraPoint = QPoint());
    Qt::CursorShape cursorShape(const QPoint &point);

    void moveRect(const QPoint& movePos);
    void resizeRect(const QPoint &pSource, const QPoint& resizeVector);

    QRect parentRect() const;
    void setParentRect(const QRect &parentRect);

    void setOrient(const Orientation &orient);

    QRect absoluteRect();

private:
    void growRect(const QPoint &pSource, const QPoint &resizeVector);

    Orientation m_orient = None;
    QRect m_parentRect;
    int m_pointRadius = 4;
    int m_lineWidth = 2;
};

#endif // SELECTRECT_H
