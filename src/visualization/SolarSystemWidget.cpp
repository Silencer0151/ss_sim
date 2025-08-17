#include "SolarSystemWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QColor>

SolarSystemWidget::SolarSystemWidget(NBodySimulation* simulation, QWidget* parent)
    : QWidget(parent), 
      m_simulation(simulation),
      m_scale(1e10), // Initial scale: 1 pixel = 1e10 meters
      m_viewOffset(0, 0)
{
    // Connect the simulation's signal to this widget's update slot
    connect(m_simulation, &NBodySimulation::simulationStepCompleted, this, &SolarSystemWidget::updateView);
    
    // Set a strong focus policy to receive keyboard events if needed later
    setFocusPolicy(Qt::StrongFocus);
}

void SolarSystemWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Fill the background black
    painter.fillRect(event->rect(), Qt::black);

    // Define the center of the screen, adjusted by the user's panning
    const QPointF viewCenter(width() / 2.0 + m_viewOffset.x(), height() / 2.0 + m_viewOffset.y());

    auto& bodies = m_simulation->getBodies();
    for (const auto& body : bodies) {
        // --- Draw Orbital Trails ---
        const auto& history = body.getPositionHistory();
        if (history.size() > 1) {
            for (size_t i = 0; i < history.size() - 1; ++i) {
                QPointF p1(
                    viewCenter.x() + history[i].x() / m_scale,
                    viewCenter.y() + history[i].y() / m_scale
                );
                QPointF p2(
                    viewCenter.x() + history[i+1].x() / m_scale,
                    viewCenter.y() + history[i+1].y() / m_scale
                );

                // Calculate alpha for a fading effect (trail is dimmer at the start)
                int alpha = static_cast<int>(200.0 * (static_cast<double>(i) / history.size()));
                QColor trailColor = body.getColor();
                trailColor.setAlpha(alpha);
                
                // Calculate width for a tapering effect (thicker at the body)
                double width = 1.5 * (static_cast<double>(i) / history.size());
                painter.setPen(QPen(trailColor, width));
                painter.drawLine(p1, p2);
            }
        }

        // --- Draw the Celestial Body ---
        QPointF screenPos(
            viewCenter.x() + body.getPosition().x() / m_scale,
            viewCenter.y() + body.getPosition().y() / m_scale
        );

        // --- Log scaling ---
        // A simple scaling for the body's radius on screen
        double radiusInKm = body.getRadius() / 1000.0;
        double screenRadius;
        //double screenRadius = body.getRadius() / m_scale * 500; // Exaggerate size

        if (body.getName() == "Sol") {
            screenRadius = 2.5 * std::log10(radiusInKm);
            screenRadius = std::max(10.0, screenRadius); // Sun is at least 10px
        } else {
            screenRadius = 1.5 * std::log10(radiusInKm);
            screenRadius = std::max(2.0, screenRadius); // Others are at least 2px
        }
        
        painter.setBrush(body.getColor());
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(screenPos, screenRadius, screenRadius);

        if (m_scale < 5e9) {
            painter.setPen(Qt::white);
            // Offset the text slightly from the body's center
            QPointF textPos(screenPos.x() + screenRadius + 5, screenPos.y());
            painter.drawText(textPos, body.getName());
        }
    }
}

void SolarSystemWidget::wheelEvent(QWheelEvent *event)
{
    // Zoom in/out by adjusting the scale factor
    if (event->angleDelta().y() > 0) {
        m_scale *= 0.8; // Zoom in
    } else {
        m_scale *= 1.25; // Zoom out
    }
    update(); // Redraw with the new scale
}

void SolarSystemWidget::mousePressEvent(QMouseEvent *event)
{
    // Record the starting position of a mouse drag
    if (event->button() == Qt::LeftButton) {
        m_lastMousePos = event->pos();
    }
}

void SolarSystemWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Pan the view by calculating the delta from the last mouse position
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_viewOffset += delta;
        m_lastMousePos = event->pos();
        update(); // Redraw with the new offset
    }
}


void SolarSystemWidget::updateView()
{
    update(); // Tells the widget to repaint itself
}
