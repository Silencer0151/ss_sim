#ifndef SOLARSYSTEMWIDGET_H
#define SOLARSYSTEMWIDGET_H

#include <QWidget>
#include "../physics/NBodySimulation.h"

class SolarSystemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SolarSystemWidget(NBodySimulation* simulation, QWidget* parent = nullptr);

protected:
    // Override paint event for custom drawing
    void paintEvent(QPaintEvent* event) override;

    // Override mouse and wheel events for user interaction
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    
private slots:
    void updateView();

private:
    NBodySimulation* m_simulation;

    // View control variables
    double m_scale;         // How many meters one pixel represents
    QPointF m_viewOffset;   // The offset of the view center from the widget center
    QPoint m_lastMousePos;  // For calculating panning delta
};

#endif // SOLARSYSTEMWIDGET_H
