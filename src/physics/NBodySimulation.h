#ifndef NBODYSIMULATION_H
#define NBODYSIMULATION_H

#include <QObject>
#include <QTimer>
#include <vector>
#include "CelestialBody.h"

class NBodySimulation : public QObject
{
    Q_OBJECT

public:
    NBodySimulation(QObject* parent = nullptr);

    void addBody(const CelestialBody& body);
    const std::vector<CelestialBody>& getBodies() const;
    void start();
    void stop();
    
signals:
    void simulationStepCompleted();

private slots:
    void step();

private:
    std::vector<CelestialBody> m_bodies;
    QTimer m_timer;
};

#endif // NBODYSIMULATION_H