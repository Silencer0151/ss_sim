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

// NBodySimulation.cpp

void NBodySimulation::step()
{
    // Store current and new accelerations for Velocity Verlet
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

    // 2. Update positions based on current velocity and acceleration
    // p(t + dt) = p(t) + v(t) * dt + 0.5 * a(t) * dt^2
    for (size_t i = 0; i < m_bodies.size(); ++i) {
        QVector3D newPosition = m_bodies[i].getPosition() +
                                m_bodies[i].getVelocity() * dt +
                                0.5 * currentAccelerations[i] * dt * dt;
        m_bodies[i].setPosition(newPosition);
    }

    // 3. Second pass: calculate new accelerations (a(t + dt)) based on new positions
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

    // 4. Update velocities based on the average of current and new accelerations
    // v(t + dt) = v(t) + 0.5 * (a(t) + a(t + dt)) * dt
    for (size_t i = 0; i < m_bodies.size(); ++i) {
        QVector3D newVelocity = m_bodies[i].getVelocity() +
                                0.5 * (currentAccelerations[i] + newAccelerations[i]) * dt;
        m_bodies[i].setVelocity(newVelocity);
    }

    emit simulationStepCompleted();
}