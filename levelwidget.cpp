#include "levelwidget.h"

LevelWidget::LevelWidget(QWidget *parent)
    : QWidget(parent)
    , m_level(0)
{
    setMinimumHeight(30);
}

void LevelWidget::setLevel(int level)
{
    // Keep the value between 0 and 100.
    m_level = qBound(0,level,100);
    // Tell Qt that the widget needs to be painted again.
    update();
}

int LevelWidget::level() const
{
    return m_level;
}

void LevelWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // Background
    painter.fillRect(rect(),Qt::white);
    // Progress bar rectangle
    QRect barRect(10,5,width() - 20,height() - 10);
    // Draw border.
    painter.setPen(QPen(Qt::black,1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(barRect);
    // Calculate filled width
    int filledWidth =qRound(barRect.width()* m_level/ 100.0);
    QRect filledRect(barRect.left(),barRect.top(),filledWidth,barRect.height());
    // Draw progress
    painter.fillRect(filledRect,Qt::black);
    // Draw percentage text
    painter.setPen(Qt::red);
    painter.drawText(barRect,Qt::AlignCenter,QString("%1%").arg(m_level));
}