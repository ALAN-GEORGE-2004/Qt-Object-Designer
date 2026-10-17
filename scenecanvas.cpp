#include "scenecanvas.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPen>

SceneCanvas::SceneCanvas(QWidget *parent)
    : QWidget(parent)
    , m_selectedObject(-1)//initially no object is selected
    , m_dragging(false)//initially no dragging
    , m_draggedObject(-1)//initially no object is dragged
{
    // Give the canvas a reasonable minimum size
    setMinimumSize(500, 400);
    // We want mouseMoveEvent() to be called even when no mouse button is pressed.
    setMouseTracking(true);
    // Allow the canvas to receive keyboard focus
    setFocusPolicy(Qt::StrongFocus);
}

void SceneCanvas::setObjects(const QVector<ObjectData> &objects)
{
    // Copy the objects into our local rendering data
    m_objects = objects;
    // Tell Qt that the widget needs to be repainted
    update();
}

void SceneCanvas::setSelectedObject(int index)
{
    m_selectedObject = index;
    // Redraw so the selection outline changes
    update();
}

void SceneCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    //Background
    painter.fillRect(rect(),Qt::white);
    //Creating pen for drawing the outline
    QPen gridPen(QColor(220, 220, 220));
    painter.setPen(gridPen);//setting the pen
    const int gridSize = 20;
    // Vertical grid lines
    for (int x = 0;x < width();x += gridSize)
    {
        painter.drawLine(x,0,x,height());
    }
    // Horizontal grid lines
    for (int y = 0;y < height();y += gridSize)
    {
        painter.drawLine(0,y,width(),y);
    }
    //Draw objects
    for (int i = 0;i < m_objects.size();++i)
    {
        const ObjectData &object = m_objects.at(i);
        // Don't draw invisible objects
        if (!object.visible)
            continue;
        painter.save();//saving state(initial cordinate axis)
        // Applying object transformations
        painter.translate(object.x,object.y);
        painter.rotate(object.rotation);
        painter.scale(object.scale,object.scale);
        // Draw object according to its type
        if (object.type == "Rectangle")
        {
            painter.drawRect(-40,-30,80,60);
        }
        else if (object.type == "Ellipse")
        {
            painter.drawEllipse(-30,-30,60,60);
        }

        // Draw selection outline
        if (i == m_selectedObject)
        {
            QPen selectionPen(Qt::blue,2);
            painter.setPen(selectionPen);
            painter.drawRect(-45,-35,90,70);
        }
        // Restore painter state
        painter.restore();//restoring the initial cordinate axis
    }
}

void SceneCanvas::mousePressEvent(QMouseEvent *event)
{
    setFocus();
    if (event->button() != Qt::LeftButton)
        return;
    QPointF mousePosition = event->position();
    for (int i = m_objects.size() - 1;i >= 0;--i)
    {
        const ObjectData &object = m_objects.at(i);
        if (!object.visible)
            continue;
        // Build the same transformation used during painting.
        QTransform transform;
        transform.translate(object.x,object.y);
        transform.rotate(object.rotation);
        transform.scale(object.scale,object.scale);
        // Convert mouse position from canvas space to object-local space.
        bool invertible = false;
        QTransform inverse = transform.inverted(&invertible);
        if (!invertible)
            continue;
        QPointF localPosition = inverse.map(mousePosition);
        bool hit = false;
        if (object.type == "Rectangle")
        {
            QRectF rectangle(-40,-30,80,60);
            hit = rectangle.contains(localPosition);
        }
        else if (object.type == "Ellipse")
        {
            QRectF ellipseBounds(-30,-30,60,60);
            QPainterPath ellipsePath;
            ellipsePath.addEllipse(ellipseBounds);
            hit = ellipsePath.contains(localPosition);
        }
        if (hit)
        {
            m_selectedObject = i;
            emit objectClicked(i);
            // Start dragging.
            m_dragging = true;
            m_draggedObject = i;
            // Remember where inside the objectthe mouse was clicked.
            m_dragOffset = mousePosition - QPointF(object.x,object.y);
            update();
            return;
        }
    }
    // Nothing was clicked.
    m_selectedObject = -1;
    m_dragging = false;
    m_draggedObject = -1;
    emit objectClicked(-1);
    update();
}

void SceneCanvas::mouseMoveEvent(QMouseEvent *event)
{
    QPointF position = event->position();
    emit mousePositionChanged(position.toPoint());
    if (!m_dragging)
        return;
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if (m_draggedObject < 0 || m_draggedObject >= m_objects.size())
    {
        return;
    }
    QPointF newPosition = position - m_dragOffset;
    emit objectMoved(m_draggedObject,newPosition);
}

void SceneCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = false;
        m_draggedObject = -1;
    }
    QWidget::mouseReleaseEvent(event);
}