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
    void setTimeScaleAlternative(int scalePercentage);
    int getSubSteps() const { return m_subSteps; }
    
signals:
    void simulationStepCompleted();

private slots:
    void step();

private:
    std::vector<CelestialBody> m_bodies;
    QTimer m_timer;
    double m_baseTimeStep;      // Rename from m_timeStep
    double m_timeScale;
    double m_maxTimeStep;       // Maximum safe timestep for integration
    int m_subSteps;             // Number of physics substeps per frame
};

#endif // NBODYSIMULATION_H