#include "CelestialBody.h"

CelestialBody::CelestialBody(
    double mass,
    QVector3D position,
    QVector3D velocity,
    double radius,
    QString name,
    QColor color
) :
    m_mass(mass),
    m_position(position),
    m_velocity(velocity),
    m_radius(radius),
    m_name(name),
    m_color(color)
{
    // Initialize the history with the starting position
    addPositionToHistory(position);
}

void CelestialBody::addPositionToHistory(const QVector3D& position)
{
    m_positionHistory.push_back(position);

    // Keep the history deque from growing indefinitely
    if (m_positionHistory.size() > MAX_HISTORY_SIZE) {
        m_positionHistory.pop_front();
    }
}

const std::deque<QVector3D>& CelestialBody::getPositionHistory() const
{
    return m_positionHistory;
}
