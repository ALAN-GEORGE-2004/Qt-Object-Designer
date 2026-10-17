#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , objectModel(new ObjectModel(this))
    , animationTimer(new QTimer(this))
    , animationRotation(0.0)
    , m_animatedObject(-1)
{
    ui->setupUi(this);
    //adding event filters
    ui->objectTableView->installEventFilter(this);
    ui->sceneCanvas->installEventFilter(this);
    // Connect the model to the table
    ui->objectTableView->setModel(objectModel);
    // Show model data on the canvas
    refreshCanvas();
    //for setting timer interval
    animationTimer->setInterval(16);
    //this connection is for after selecting an object from the table
    connect(ui->objectTableView->selectionModel(),&QItemSelectionModel::currentChanged,this,[this](const QModelIndex &current,const QModelIndex &previous)
        {
            Q_UNUSED(previous);
            if (!current.isValid())
            {
                ui->sceneCanvas->setSelectedObject(-1);
                clearPropertiesPanel();
                return;
            }
            int row = current.row();
            ui->sceneCanvas->setSelectedObject(row);
            updatePropertiesPanel(row);
        });
    // Update canvas when model data changes
    connect(objectModel,&QAbstractItemModel::dataChanged,this,[this](const QModelIndex &topLeft,const QModelIndex &bottomRight,const QList<int> &roles)
        {
            Q_UNUSED(bottomRight);
            Q_UNUSED(roles);
            // Update canvas
            refreshCanvas();
            // Update properties panel
            QModelIndex current = ui->objectTableView->currentIndex();
            if (current.isValid() && current.row() == topLeft.row())
            {
                updatePropertiesPanel(current.row());
            }
        });
    // Mouse position
    connect(ui->sceneCanvas,&SceneCanvas::mousePositionChanged,this,[this](QPoint position)
        {
            ui->mousePositionLabel->setText(
                QString("Mouse: (%1, %2)")
                    .arg(position.x())
                    .arg(position.y())
                );
        });
    //this connection is for after selecting a object in canvas, that object must be selected in the table as well
    connect(ui->sceneCanvas,&SceneCanvas::objectClicked,this,[this](int index)
        {
            if (index >= 0)
            {
                ui->objectTableView->selectRow(index);
            }
            else
            {
                ui->objectTableView->clearSelection();
                ui->objectTableView->setCurrentIndex(QModelIndex());
                clearPropertiesPanel();
            }
        });
    //for name
    connect(ui->nameEdit,&QLineEdit::editingFinished,this,[this]()
        {
            updateSelectedObjectProperty(0,ui->nameEdit->text());
        });
    //for X spin box
    connect(ui->xSpinBox,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,[this](double value)
        {
            value = snapValue(value);
            updateSelectedObjectProperty(2,value);
        });
    //for Y spin box
    connect(ui->ySpinBox,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,[this](double value)
        {
            value = snapValue(value);
            updateSelectedObjectProperty(3,value);
        });
    //for rotation
    connect(ui->rotationSpinBox,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,[this](double value)
        {
            updateSelectedObjectProperty(4,value);
        });
    //for scale
    connect(ui->scaleSpinBox,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,[this](double value)
        {
            updateSelectedObjectProperty(5,value);
        });
    //for visible
    connect(ui->visibleCheckBox,&QCheckBox::toggled,this,[this](bool checked)
        {
            updateSelectedObjectProperty(6,checked);
        });
    //for adding new object
    connect(ui->addObjectButton,&QPushButton::clicked,this,[this]()
        {
            ObjectData object;
            object.name = "New Object";
            object.type = "Rectangle";
            object.x = 100;
            object.y = 100;
            object.rotation = 0;
            object.scale = 1.0;
            object.visible = true;
            //for showing the new object is selected
            int newRow = objectModel->rowCount();
            objectModel->addObject(object);
            ui->objectTableView->selectRow(newRow);
        });
    //for updating canvas when a new object is created
    connect(objectModel,&QAbstractItemModel::rowsInserted,this,[this]()
        {
            refreshCanvas();
        });
    //for updating canvas when an object is deleted
    connect(objectModel,&QAbstractItemModel::rowsRemoved,this,[this]()
        {
            refreshCanvas();
        });
    //for deleting
    connect(ui->deleteObjectButton,&QPushButton::clicked,this,[this]()
        {
            QModelIndex index = ui->objectTableView->currentIndex();
            if (!index.isValid())
                return;
            objectModel->removeObject(index.row());
        });
    //for snap to grid
    connect(ui->snapCheckBox,&QCheckBox::toggled,this,[this](bool enabled)
        {
            if (!enabled)
                return;
            QModelIndex currentIndex = ui->objectTableView->currentIndex();
            if (!currentIndex.isValid())
                return;
            ObjectData object = objectModel->objectAt(currentIndex.row());
            objectModel->setData(objectModel->index(currentIndex.row(),2),snapValue(object.x),Qt::EditRole);
            objectModel->setData(objectModel->index(currentIndex.row(),3),snapValue(object.y),Qt::EditRole);
        });
    //for dragging
    connect(ui->sceneCanvas,&SceneCanvas::objectMoved,this,[this](int index,QPointF position)
        {
            if (index < 0 || index >= objectModel->rowCount())
            {
                return;
            }
            position.setX(snapValue(position.x()));
            position.setY(snapValue(position.y()));
            objectModel->setObjectPosition(index,position);
        });
    //connections for animation
    connect(animationTimer,&QTimer::timeout,this,&MainWindow::animationStep);
    connect(ui->startAnimationButton,&QPushButton::clicked,this,&MainWindow::startAnimation);
    connect(ui->stopAnimationButton,&QPushButton::clicked,this,&MainWindow::stopAnimation);
}


MainWindow::~MainWindow()
{
    delete ui;
}

// Send model objects to canvas
void MainWindow::refreshCanvas()
{
    QVector<ObjectData> objects;
    // Get every object from the model
    for (int i = 0;i < objectModel->rowCount();++i)
    {
        objects.append(objectModel->objectAt(i));
    }
    // Give the objects to the canvas
    ui->sceneCanvas->setObjects(objects);
}

//setting properties from the table to the properties panel after selecting from table or from canvas
void MainWindow::updatePropertiesPanel(int row)
{
    if (row < 0 || row >= objectModel->rowCount())
    {
        return;
    }
    //QSignalBlocker - Temporarily silence this widget's signals.
    ObjectData object = objectModel->objectAt(row);
    QSignalBlocker nameBlocker(ui->nameEdit);
    QSignalBlocker xBlocker(ui->xSpinBox);
    QSignalBlocker yBlocker(ui->ySpinBox);
    QSignalBlocker rotationBlocker(ui->rotationSpinBox);
    QSignalBlocker scaleBlocker(ui->scaleSpinBox);
    QSignalBlocker visibleBlocker(ui->visibleCheckBox);
    ui->nameEdit->setText(object.name);
    ui->xSpinBox->setValue(object.x);
    ui->ySpinBox->setValue(object.y);
    ui->rotationSpinBox->setValue(object.rotation);
    ui->scaleSpinBox->setValue(object.scale);
    ui->visibleCheckBox->setChecked(object.visible);
}

//for updating the table after change is done in properties panel
void MainWindow::updateSelectedObjectProperty(int column,const QVariant &value)
{
    QModelIndex currentIndex = ui->objectTableView->currentIndex();
    if (!currentIndex.isValid())
        return;
    QModelIndex propertyIndex = objectModel->index(currentIndex.row(),column);
    objectModel->setData(propertyIndex,value,Qt::EditRole);
}

//for clearing the values after an object is deleted
void MainWindow::clearPropertiesPanel()
{
    QSignalBlocker nameBlocker(ui->nameEdit);
    QSignalBlocker xBlocker(ui->xSpinBox);
    QSignalBlocker yBlocker(ui->ySpinBox);
    QSignalBlocker rotationBlocker(ui->rotationSpinBox);
    QSignalBlocker scaleBlocker(ui->scaleSpinBox);
    QSignalBlocker visibleBlocker(ui->visibleCheckBox);
    ui->nameEdit->clear();
    ui->xSpinBox->setValue(0);
    ui->ySpinBox->setValue(0);
    ui->rotationSpinBox->setValue(0);
    ui->scaleSpinBox->setValue(1.0);
    ui->visibleCheckBox->setChecked(false);
}

//applying event filters
bool MainWindow::eventFilter(QObject *watched,QEvent *event)
{
    if (watched == ui->objectTableView || watched == ui->sceneCanvas)//if anything happens in table or canvas
    {
        if (event->type() == QEvent::KeyPress)//if event is keyPress
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Delete || keyEvent->key() == Qt::Key_Backspace)//if key is delete or backspace
            {
                deleteSelectedObject();
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched,event);
}

//deleting the selected object
void MainWindow::deleteSelectedObject()
{
    QModelIndex currentIndex = ui->objectTableView->currentIndex();
    if (!currentIndex.isValid())
        return;
    int row = currentIndex.row();
    if (row == m_animatedObject)
    {
        stopAnimation();
        m_animatedObject = -1;
    }
    else if (row < m_animatedObject)
    {
        --m_animatedObject;
    }
    objectModel->removeObject(row);
    int remainingObjects = objectModel->rowCount();
    if (remainingObjects == 0)
    {
        ui->objectTableView->clearSelection();
        ui->sceneCanvas->setSelectedObject(-1);
        clearPropertiesPanel();
        return;
    }
    int newRow = row;
    if (newRow >= remainingObjects)
    {
        newRow = remainingObjects - 1;
    }
    ui->objectTableView->selectRow(newRow);
}

//for snap to grid functionality
double MainWindow::snapValue(double value) const
{
    if (!ui->snapCheckBox->isChecked())
        return value;
    const double gridSize = 20.0;
    return qRound(value / gridSize) * gridSize;
}

//for animation to start
void MainWindow::startAnimation()
{
    QModelIndex currentIndex = ui->objectTableView->currentIndex();
    if (!currentIndex.isValid())
        return;
    m_animatedObject = currentIndex.row();
    ObjectData object = objectModel->objectAt(m_animatedObject);
    animationRotation = object.rotation;
    animationTimer->start();
}

void MainWindow::stopAnimation()
{
    animationTimer->stop();
    m_animatedObject = -1;
}

//for animation to end
void MainWindow::animationStep()
{
    if (m_animatedObject < 0 || m_animatedObject >= objectModel->rowCount())
    {
        animationTimer->stop();
        return;
    }
    animationRotation += 2.0;
    if (animationRotation >= 360.0)
    {
        animationRotation -= 360.0;
    }
    objectModel->setData(objectModel->index(m_animatedObject,4),animationRotation,Qt::EditRole);
    int progress = qRound((animationRotation / 360.0)* 100.0);
    ui->animationLevelWidget->setLevel(progress);
}