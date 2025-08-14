#include "SolarSystemWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QColor>

SolarSystemWidget::SolarSystemWidget(NBodySimulation* simulation, QWidget* parent)
    : QWidget(parent), m_simulation(simulation)
{
    // Connect the simulation's signal to this widget's update slot
    connect(m_simulation, &NBodySimulation::simulationStepCompleted, this, &SolarSystemWidget::updateView);
}

void SolarSystemWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    
    // Fill the background black
    painter.fillRect(event->rect(), Qt::black);

    // Orbital scale: 1 pixel represents 5e9 meters
    const double orbitalScale = 5e9;

    // Define the center of the screen
    const QPointF center(width() / 2.0, height() / 2.0);

    const auto& bodies = m_simulation->getBodies();
    for (const auto& body : bodies) {
        // Convert astronomical position to screen coordinates
        QPointF screenPos(
            center.x() + body.getPosition().x() / orbitalScale,
            center.y() + body.getPosition().y() / orbitalScale
        );

        // Set a fixed, small size for planets and a larger size for the sun
        double screenRadius;
        if (body.getName() == "Sun") {
            screenRadius = 25.0; // A fixed size of 25 pixels
        } else {
            screenRadius = 5.0; // A fixed size of 5 pixels
        }
        
        // Draw the body
        painter.setBrush(body.getColor());
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(screenPos, screenRadius, screenRadius);
    }
}

void SolarSystemWidget::updateView()
{
    update(); // Tells the widget to repaint itself
}