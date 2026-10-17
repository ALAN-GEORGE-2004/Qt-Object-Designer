#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QPointF>

#include "objectdata.h"

class ObjectModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ObjectModel(QObject *parent = nullptr);
    //These functions are Qt Functions
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;//for returning the no of rows
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;//for returning the no of colomns
    QVariant data(const QModelIndex &index,int role = Qt::DisplayRole) const override;//for returning the info about the objects
    //int role = Qt::DisplayRole - means the data is used for displaying
    QVariant headerData(int section,Qt::Orientation orientation,int role = Qt::DisplayRole) const override;//for returning the header details
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole) override;//for changing the data if we click on a cell
    //int role = Qt::EditRole - means the data is used for editing
    Qt::ItemFlags flags(const QModelIndex &index) const override;//This tells Qt what the user is allowed to do with a cell

    //These functions are created by us
    void addObject(const ObjectData &object);//to add a new object
    void removeObject(int row);//to remove an object
    void setObjectPosition(int row,const QPointF &position);
    ObjectData objectAt(int row) const;//for retrieving an object

private:
    QVector<ObjectData> m_objects;//for storing information about different objects
};

#endif