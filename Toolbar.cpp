#include "Toolbar.h"
#include <QHBoxLayout>
#include <QPushButton>

Toolbar::Toolbar(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_TranslucentBackground,false);
    this->setAttribute(Qt::WA_NoSystemBackground,false);

    m_sure = new QPushButton("确定");
    m_cancel = new QPushButton("取消");
//    m_sure->setFixedSize(100,50);
//    m_cancel->setFixedSize(100,50);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_cancel);
    layout->addWidget(m_sure);
    layout->setMargin(5);
    layout->setSpacing(5);

    connect(m_sure, &QPushButton::clicked, this, &Toolbar::sure);
    connect(m_cancel, &QPushButton::clicked, this, &Toolbar::cancel);
}
