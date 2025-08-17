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

    void addBody(CelestialBody& body);
    std::vector<CelestialBody>& getBodies();
    void start();
    void stop();

public slots:
    // controls for the sim
    void play();
    void pause();
    void setTimeScale(int scalePercentage); // slider for 0-100
    
signals:
    void simulationStepCompleted();

private slots:
    void step();

private:
    std::vector<CelestialBody> m_bodies;
    QTimer m_timer;
    double m_timeStep;
    double m_timeScale;
};

#endif // NBODYSIMULATION_H