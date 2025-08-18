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
    // Initialize both history types with the starting position
    addPositionToHistory(position);
}

void CelestialBody::addPositionToHistory(const QVector3D& position)
{
    // Add to the short-term fading trail
    m_positionHistory.push_back(position);
    if (m_positionHistory.size() > MAX_HISTORY_SIZE) {
        m_positionHistory.pop_front();
    }

    // Add to the long-term full orbit trace
    m_fullOrbitTrace.push_back(position);
}

const std::deque<QVector3D>& CelestialBody::getPositionHistory() const
{
    return m_positionHistory;
}

// Getter for the full orbit trace
const std::vector<QVector3D>& CelestialBody::getFullOrbitTrace() const
{
    return m_fullOrbitTrace;
}
