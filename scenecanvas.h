#ifndef SCENECANVAS_H
#define SCENECANVAS_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QTransform>
#include <QPainterPath>

#include "objectdata.h"

class SceneCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit SceneCanvas(QWidget *parent = nullptr);
    void setObjects(const QVector<ObjectData> &objects);//This is how the MainWindow will give the canvas the objects it needs to draw.
    void setSelectedObject(int index);//This tells the canvas which object is currently selected.

signals:
    void objectClicked(int index);//to show which object is clicked
    void mousePositionChanged(QPoint position);//to show mouse position changed
    void objectMoved(int index,QPointF position);//for dragging

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QVector<ObjectData> m_objects;
    int m_selectedObject;
    bool m_dragging;
    int m_draggedObject;
    QPointF m_dragOffset;
};

#endif