#include "NBodySimulation.h"
#include <QDebug>
#include <cmath>

const double G = 6.67430e-11;

NBodySimulation::NBodySimulation(QObject* parent)
    : QObject(parent),
      m_timeStep(3600), // Base time step is 1 hour
      m_timeScale(1.0)  // Initial speed is 1x
{
    // Set up a timer to drive the simulation loop
    m_timer.setInterval(16); // ~60 FPS for smooth animation
    connect(&m_timer, &QTimer::timeout, this, &NBodySimulation::step);
}

// Note: The parameter is now a non-const reference to allow modification (e.g., adding history)
void NBodySimulation::addBody(CelestialBody& body)
{
    m_bodies.push_back(body);
}

// Note: The return type is now a non-const reference
std::vector<CelestialBody>& NBodySimulation::getBodies()
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

void NBodySimulation::play()
{
    m_timer.start();
}

void NBodySimulation::pause()
{
    m_timer.stop();
}

void NBodySimulation::setTimeScale(int scalePercentage)
{
    // Use a logarithmic scale for better control over a wide range of speeds
    // This maps the slider's 0-100 range to a time scale of 0.01x to 100x
    // 50 on the slider will correspond to 1x speed.
    double power = (static_cast<double>(scalePercentage) / 50.0) - 1.0;
    m_timeScale = std::pow(10.0, power);
}


void NBodySimulation::step()
{
    double dt = m_timeStep * m_timeScale;

    std::vector<QVector3D> currentAccelerations;
    std::vector<QVector3D> newAccelerations;

    // 1. First pass: calculate current accelerations (a(t))
    for (size_t i = 0; i < m_bodies.size(); ++i) {
        QVector3D totalForce(0, 0, 0);
        for (size_t j = 0; j < m_bodies.size(); ++j) {
            if (i == j) continue;
            QVector3D r = m_bodies[j].getPosition() - m_bodies[i].getPosition();
            double r_sq = QVector3D::dotProduct(r, r);
            if (r_sq == 0.0) continue;
            double r_mag = std::sqrt(r_sq);
            double F_mag = (G * m_bodies[i].getMass() * m_bodies[j].getMass()) / r_sq;
            QVector3D F = (F_mag / r_mag) * r;
            totalForce += F;
        }
        currentAccelerations.push_back(totalForce / m_bodies[i].getMass());
    }

    // 2. Update positions and add to history
    for (size_t i = 0; i < m_bodies.size(); ++i) {
        QVector3D newPosition = m_bodies[i].getPosition() +
                                m_bodies[i].getVelocity() * dt +
                                0.5 * currentAccelerations[i] * dt * dt;
        m_bodies[i].setPosition(newPosition);
        m_bodies[i].addPositionToHistory(newPosition); // Add to trail history
    }

    // 3. Second pass: calculate new accelerations (a(t + dt))
    for (size_t i = 0; i < m_bodies.size(); ++i) {
        QVector3D totalForce(0, 0, 0);
        for (size_t j = 0; j < m_bodies.size(); ++j) {
            if (i == j) continue;
            QVector3D r = m_bodies[j].getPosition() - m_bodies[i].getPosition();
            double r_sq = QVector3D::dotProduct(r, r);
            if (r_sq == 0.0) continue;
            double r_mag = std::sqrt(r_sq);
            double F_mag = (G * m_bodies[i].getMass() * m_bodies[j].getMass()) / r_sq;
            QVector3D F = (F_mag / r_mag) * r;
            totalForce += F;
        }
        newAccelerations.push_back(totalForce / m_bodies[i].getMass());
    }

    // 4. Update velocities
    for (size_t i = 0; i < m_bodies.size(); ++i) {
        QVector3D newVelocity = m_bodies[i].getVelocity() +
                                0.5 * (currentAccelerations[i] + newAccelerations[i]) * dt;
        m_bodies[i].setVelocity(newVelocity);
    }

    emit simulationStepCompleted();
}
