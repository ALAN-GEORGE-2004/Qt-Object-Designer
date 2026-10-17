#ifndef LEVELWIDGET_H
#define LEVELWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QtMath>

class LevelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LevelWidget(QWidget *parent = nullptr);
    void setLevel(int level);
    int level() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_level;
};

#endif