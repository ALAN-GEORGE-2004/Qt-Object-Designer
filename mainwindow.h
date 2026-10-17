#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalBlocker>
#include <QKeyEvent>
#include <QtMath>
#include <QTimer>
#include "objectmodel.h"
#include "levelwidget.h"
#include "scenecanvas.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ObjectModel *objectModel;
    void refreshCanvas();
    void updatePropertiesPanel(int row);
    void updateSelectedObjectProperty(int column,const QVariant &value);
    void clearPropertiesPanel();
    void deleteSelectedObject();
    double snapValue(double value) const;
    QTimer *animationTimer;
    double animationRotation;
    void startAnimation();
    void stopAnimation();
    void animationStep();
    int m_animatedObject;

protected:
    bool eventFilter(QObject *watched,QEvent *event) override;
};

#endif