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
}