#include <QApplication>
#include <QWidget>
#include "src/visualization/SolarSystemWidget.h"
#include "src/physics/NBodySimulation.h"
#include "src/physics/CelestialBody.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create the simulation and the visualization widget
    NBodySimulation simulation;
    SolarSystemWidget window(&simulation);

    //the sun
    CelestialBody sun(
        1.989e30,               // mass in kg
        QVector3D(0, 0, 0),     // position at origin
        QVector3D(0, 0, 0),     // velocity is zero
        6.96e8,                 // radius in meters
        "Sol",
        Qt::yellow
    );

    //home
    CelestialBody earth(
        5.972e24,                   // mass in kg
        QVector3D(1.5e11, 0, 0),    // position 1au away from sol
        QVector3D(0, 2.98e4, 0),    // velocity tangential to the orbit 
        6.37e6,                     // radius in meters
        "Terra",
        Qt::blue
    );

    simulation.addBody(sun);
    simulation.addBody(earth);

    // Set up the window
    window.setWindowTitle("Solar System Simulator");
    window.resize(1000, 800);
    window.show();

    // Start the simulation timer
    simulation.start();

    return app.exec();
}