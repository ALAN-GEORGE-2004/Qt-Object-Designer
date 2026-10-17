#include "objectmodel.h"

ObjectModel::ObjectModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    // Create the first object
    ObjectData cube;
    cube.name = "Cube";
    cube.type = "Rectangle";
    cube.x = 150;
    cube.y = 100;
    cube.rotation = 0;
    cube.scale = 1.0;
    cube.visible = true;
    // Create the second object
    ObjectData circle;
    circle.name = "Circle";
    circle.type = "Ellipse";
    circle.x = 300;
    circle.y = 200;
    circle.rotation = 0;
    circle.scale = 1.0;
    circle.visible = true;
    // Add the objects to our model
    m_objects.append(cube);
    m_objects.append(circle);
}

int ObjectModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_objects.size();
}

int ObjectModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7;
}

QVariant ObjectModel::data(const QModelIndex &index,int role) const
{
    // Make sure the index is valid
    if (!index.isValid())
        return QVariant();
    if (role != Qt::DisplayRole && role != Qt::EditRole)
    {
        return QVariant();
    }
    // Get the object belonging to this row
    const ObjectData &object = m_objects.at(index.row());
    // Decide which value to return
    switch (index.column())
    {
    case 0: return object.name;
    case 1: return object.type;
    case 2: return object.x;
    case 3: return object.y;
    case 4: return object.rotation;
    case 5: return object.scale;
    case 6: return object.visible;
    }
    return QVariant();//if colomn is less than 0 or greater than 6
}

QVariant ObjectModel::headerData(int section,Qt::Orientation orientation,int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    // Horizontal headers = columns
    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0: return "Name";
        case 1: return "Type";
        case 2: return "X";
        case 3: return "Y";
        case 4: return "Rotation";
        case 5: return "Scale";
        case 6: return "Visible";
        }
    }
    // Vertical headers = row numbers
    if (orientation == Qt::Vertical)//to give numbering to each row
    {
        return section + 1;
    }
    return QVariant();
}

bool ObjectModel::setData(const QModelIndex &index,const QVariant &value,int role)
{
    if (!index.isValid())
        return false;
    if (role != Qt::EditRole)
        return false;
    // Get the object we want to modify
    ObjectData &object = m_objects[index.row()];
    // Change the appropriate property
    switch (index.column())
    {
    case 0: object.name = value.toString();
            break;
    case 1: object.type = value.toString();
            break;
    case 2: object.x = value.toDouble();
            break;
    case 3: object.y = value.toDouble();
            break;
    case 4: object.rotation = value.toDouble();
            break;
    case 5: object.scale = value.toDouble();
            break;
    case 6: object.visible = value.toBool();
            break;
    default: return false;
    }
    // Tell Qt that the data changed
    emit dataChanged(index,index,{Qt::DisplayRole,Qt::EditRole});
    return true;
}

Qt::ItemFlags ObjectModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled| Qt::ItemIsSelectable| Qt::ItemIsEditable;
}

void ObjectModel::addObject(const ObjectData &object)
{
    int newRow = m_objects.size();
    // Tell Qt that we are about to insert a row
    beginInsertRows(QModelIndex(),newRow,newRow);
    // Actually add the object
    m_objects.append(object);
    // Tell Qt the insertion is finished
    endInsertRows();
}

void ObjectModel::removeObject(int row)
{
    if (row < 0 || row >= m_objects.size())
    {
        return;
    }
    // Tell Qt that we are removing a row
    beginRemoveRows(QModelIndex(),row,row);
    // Remove the object
    m_objects.removeAt(row);
    // Tell Qt the removal is finished
    endRemoveRows();
}

ObjectData ObjectModel::objectAt(int row) const
{
    if (row < 0 || row >= m_objects.size())
    {
        return {};
    }
    return m_objects.at(row);
}

void ObjectModel::setObjectPosition(int row,const QPointF &position)
{
    if (row < 0 || row >= m_objects.size())
    {
        return;
    }
    ObjectData &object = m_objects[row];
    object.x = position.x();
    object.y = position.y();
    emit dataChanged(index(row, 2),index(row, 3),{Qt::DisplayRole,Qt::EditRole});
}