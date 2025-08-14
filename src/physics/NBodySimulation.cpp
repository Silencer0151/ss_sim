#include "NBodySimulation.h"
#include <QDebug>
#include <cmath>

const double G = 6.67430e-11;

// time step for sim in seconds
// smaller is more accurate but slower
const double dt = 3600; // 1 hour

NBodySimulation::NBodySimulation(QObject* parent)
    : QObject(parent)
{
    // Set up a timer to drive the simulation loop
    m_timer.setInterval(16); // ~60 FPS
    connect(&m_timer, &QTimer::timeout, this, &NBodySimulation::step);
}

void NBodySimulation::addBody(const CelestialBody& body)
{
    m_bodies.push_back(body);
}

const std::vector<CelestialBody>& NBodySimulation::getBodies() const
{
    return m_bodies;
}

void NBodySimulation::start()
{
    m_timer.start();
}

void NBodySimulation::stop()
{
    m_timer.stop();
}

void NBodySimulation::step()
{
    std::vector<QVector3D> newPositions;
    std::vector<QVector3D> newVelocities;

    // First pass: calculate forces and new velocities/positions
    for (size_t i = 0; i < m_bodies.size(); ++i) {
        QVector3D totalForce(0, 0, 0);
        for (size_t j = 0; j < m_bodies.size(); ++j) {
            if (i == j) continue;

            QVector3D r = m_bodies[j].getPosition() - m_bodies[i].getPosition();
            double r_sq = QVector3D::dotProduct(r, r);
            
            // Check for r_sq being zero to prevent division by zero
            if (r_sq == 0.0) continue; 
            
            double r_mag = std::sqrt(r_sq);
            double F_mag = (G * m_bodies[i].getMass() * m_bodies[j].getMass()) / r_sq;
            QVector3D F = (F_mag / r_mag) * r;
            totalForce += F;
        }

        QVector3D acceleration = totalForce / m_bodies[i].getMass();
        QVector3D newVelocity = m_bodies[i].getVelocity() + acceleration * dt;
        QVector3D newPosition = m_bodies[i].getPosition() + newVelocity * dt; // Using new velocity for better accuracy

        newPositions.push_back(newPosition);
        newVelocities.push_back(newVelocity);
    }
    
    // Second pass: apply the calculated updates
    for (size_t i = 0; i < m_bodies.size(); ++i) {
        m_bodies[i].setPosition(newPositions[i]);
        m_bodies[i].setVelocity(newVelocities[i]);
    }
    
    emit simulationStepCompleted();
}