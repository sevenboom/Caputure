#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>

class QPushButton;

class Toolbar : public QWidget
{
    Q_OBJECT
public:
    explicit Toolbar(QWidget *parent = 0);

signals:
    void sure(); // the "sure" button clicked
    void cancel(); //the "cancel" button clicked

public slots:

private:
    QPushButton *m_sure;
    QPushButton *m_cancel;
};

#endif // TOOLBAR_H
