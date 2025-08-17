#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <QVector3D>
#include <QString>
#include <QColor>
#include <deque>

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

    // Methods for orbital trails
    void addPositionToHistory(const QVector3D& position);
    const std::deque<QVector3D>& getPositionHistory() const;

private:
    double m_mass;
    QVector3D m_position;
    QVector3D m_velocity;
    double m_radius;
    QString m_name;
    QColor m_color;

    // Store the last N positions for drawing trails
    std::deque<QVector3D> m_positionHistory;
    static const size_t MAX_HISTORY_SIZE = 2000;
};

#endif // CELESTIALBODY_H
