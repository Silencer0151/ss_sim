#include "SolarSystemWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QColor>
#include <cmath>

SolarSystemWidget::SolarSystemWidget(NBodySimulation* simulation, QWidget* parent)
    : QWidget(parent), 
      m_simulation(simulation),
      m_scale(1e10), // Initial scale: 1 pixel = 1e10 meters
      m_viewOffset(0, 0),
      m_selectedBodyIndex(-1) // Initialize with no body selected
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

        double radiusInKm = body.getRadius() / 1000.0;
        double screenRadius;

        if (body.getName() == "Sol") {
            screenRadius = 2.5 * std::log10(radiusInKm);
            screenRadius = std::max(10.0, screenRadius);
        } else {
            screenRadius = 1.5 * std::log10(radiusInKm);
            screenRadius = std::max(2.0, screenRadius);
        }
        
        painter.setBrush(body.getColor());
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(screenPos, screenRadius, screenRadius);

        if (m_scale < 5e9) {
            painter.setPen(Qt::white);
            QPointF textPos(screenPos.x() + screenRadius + 5, screenPos.y());
            painter.drawText(textPos, body.getName());
        }
    }

    // --- Draw Selection Info and Highlight ---
    if (m_selectedBodyIndex != -1 && m_selectedBodyIndex < bodies.size()) {
        const auto& selectedBody = bodies[m_selectedBodyIndex];

        QPointF screenPos(
            viewCenter.x() + selectedBody.getPosition().x() / m_scale,
            viewCenter.y() + selectedBody.getPosition().y() / m_scale
        );
        double radiusInKm = selectedBody.getRadius() / 1000.0;
        double screenRadius;
        if (selectedBody.getName() == "Sol") {
            screenRadius = 2.5 * std::log10(radiusInKm);
            screenRadius = std::max(10.0, screenRadius);
        } else {
            screenRadius = 1.5 * std::log10(radiusInKm);
            screenRadius = std::max(2.0, screenRadius);
        }

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::cyan, 2));
        painter.drawEllipse(screenPos, screenRadius + 5, screenRadius + 5);

        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setPointSize(10);
        painter.setFont(font);

        QString infoText = QString("Selected: %1\n"
                                   "Mass: %2 kg\n"
                                   "Radius: %3 km\n"
                                   "Position: (%4, %5, %6) m\n"
                                   "Velocity: (%7, %8, %9) m/s")
                           .arg(selectedBody.getName())
                           .arg(selectedBody.getMass(), 0, 'e', 2)
                           .arg(selectedBody.getRadius() / 1000.0, 0, 'f', 0)
                           .arg(selectedBody.getPosition().x(), 0, 'e', 2)
                           .arg(selectedBody.getPosition().y(), 0, 'e', 2)
                           .arg(selectedBody.getPosition().z(), 0, 'e', 2)
                           .arg(selectedBody.getVelocity().x(), 0, 'f', 2)
                           .arg(selectedBody.getVelocity().y(), 0, 'f', 2)
                           .arg(selectedBody.getVelocity().z(), 0, 'f', 2);

        QRectF textRect = QRectF(10, 10, 300, 150);
        painter.setBrush(QColor(0, 0, 0, 150));
        painter.setPen(Qt::NoPen);
        painter.drawRect(textRect.adjusted(-5, -5, 5, 5));
        
        painter.setPen(Qt::white);
        painter.drawText(textRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, infoText);
    }
}

void SolarSystemWidget::wheelEvent(QWheelEvent *event)
{
    double zoomFactor = 1.2;
    if (event->angleDelta().y() > 0) {
        m_scale /= zoomFactor;
    } else {
        m_scale *= zoomFactor;
    }
    update();
}

void SolarSystemWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        const QPointF viewCenter(width() / 2.0 + m_viewOffset.x(), height() / 2.0 + m_viewOffset.y());
        auto& bodies = m_simulation->getBodies();
        bool bodyClicked = false;

        for (int i = 0; i < bodies.size(); ++i) {
            const auto& body = bodies[i];
            QPointF screenPos(
                viewCenter.x() + body.getPosition().x() / m_scale,
                viewCenter.y() + body.getPosition().y() / m_scale
            );
            double radiusInKm = body.getRadius() / 1000.0;
            double screenRadius;
            if (body.getName() == "Sol") {
                screenRadius = 2.5 * std::log10(radiusInKm);
                screenRadius = std::max(10.0, screenRadius);
            } else {
                screenRadius = 1.5 * std::log10(radiusInKm);
                screenRadius = std::max(2.0, screenRadius);
            }

            if ((event->pos() - screenPos).manhattanLength() < screenRadius * 1.5) {
                m_selectedBodyIndex = i;
                bodyClicked = true;
                break;
            }
        }
        
        if (!bodyClicked) {
            m_selectedBodyIndex = -1;
        }

        m_lastMousePos = event->pos();
        update();
    }
}

void SolarSystemWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_viewOffset += delta;
        m_lastMousePos = event->pos();
        update();
    }
}


void SolarSystemWidget::updateView()
{
    update();
}
