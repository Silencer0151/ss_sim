#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <QVector3D>
#include <QString>
#include <QColor>

class CelestialBody
{
public:
    CelestialBody(
        double mass,
        QVector3D position,
        QVector3D velocity,
        double radius,
        QString name,
        QColor color
    );

    double getMass() const { return m_mass; }
    QVector3D getPosition() const { return m_position; }
    QVector3D getVelocity() const { return m_velocity; }
    double getRadius() const { return m_radius; }
    QString getName() const { return m_name; }
    QColor getColor() const { return m_color; }

    void setPosition(const QVector3D& position) { m_position = position; }
    void setVelocity(const QVector3D& velocity) { m_velocity = velocity; }

private:
    double m_mass;
    QVector3D m_position;
    QVector3D m_velocity;
    double m_radius;
    QString m_name;
    QColor m_color;
};

#endif // CELESTIALBODY_H