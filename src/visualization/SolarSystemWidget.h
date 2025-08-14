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
    void paintEvent(QPaintEvent* event) override;
    
private slots:
    void updateView();

private:
    NBodySimulation* m_simulation;
};

#endif // SOLARSYSTEMWIDGET_H