#include <QApplication>
#include <QWidget>
#include "src/visualization/SolarSystemWidget.h"
#include "src/physics/NBodySimulation.h"
#include "src/physics/CelestialBody.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    NBodySimulation simulation;
    SolarSystemWidget window(&simulation);

    // Data from JPL Horizons for A.D. 2025-Aug-17 00:00:00.0000 TDB
    // All position and velocity units are converted from km to meters (* 1000)

    // 1. The Sun
    CelestialBody sun(
        1.98841e30,                                             // Mass in kg [ 427]
        QVector3D(-6.0900518e5, -8.1693778e5, 2.2523436e4) * 1000, // Position in meters
        QVector3D(1.2793151e-2, -2.2461657e-3, -2.4034488e-4) * 1000, // Velocity in m/s
        695700e3,                                               // Mean Radius in meters [ 428]
        "Sol",
        Qt::yellow
    );

    // 2. Mercury
    CelestialBody mercury(
        3.302e23,                                               // Mass in kg [ 448]
        QVector3D(4.8112912e7, 1.2534573e7, -3.3551387e6) * 1000,
        QVector3D(-2.2298433e1, 4.9150995e1, 6.0630608e0) * 1000,
        2439.4e3,                                               // Mean Radius in meters [ 448]
        "Mercury",
        Qt::gray
    );

    // 3. Venus
    CelestialBody venus(
        48.685e23,                                              // Mass in kg [ 85]
        QVector3D(5.9149889e7, 8.9134484e7, -2.1898010e6) * 1000,
        QVector3D(-2.9268834e1, 1.9223075e1, 1.9534394e0) * 1000,
        6051.84e3,                                              // Mean Radius in meters [ 85]
        "Venus",
        QColor(255, 192, 203) // Pink
    );

    // 4. Earth
    CelestialBody earth(
        5.97219e24,                                             // Mass in kg [ 108]
        QVector3D(1.2193664e8, -8.9829074e7, 2.6755997e4) * 1000,
        QVector3D(1.7046967e1, 2.3982480e1, -2.0504065e-3) * 1000,
        6371.01e3,                                              // Mean Radius in meters [ 108]
        "Terra",
        Qt::blue
    );

    // 5. Mars
    CelestialBody mars(
        6.4171e23,                                              // Mass in kg [ 133]
        QVector3D(-2.0522537e8, -1.2387619e8, 2.4613454e6) * 1000,
        QVector3D(1.3408357e1, -1.8693393e1, -7.2042398e-1) * 1000,
        3389.92e3,                                              // Mean Radius in meters [ 133]
        "Mars",
        Qt::red
    );

    // 6. Jupiter
    CelestialBody jupiter(
        18.9819e26,                                             // Mass in kg [ 156]
        QVector3D(-1.0181245e8, 7.6474981e8, -8.9333999e5) * 1000,
        QVector3D(-1.3104444e1, -1.1029944e0, 2.9780734e-1) * 1000,
        69911e3,                                                // Mean Radius in meters [ 158]
        "Jupiter",
        Qt::darkYellow
    );

    // 7. Saturn
    CelestialBody saturn(
        5.6834e26,                                              // Mass in kg [ 181]
        QVector3D(1.4265937e9, -7.6322695e7, -5.5473210e7) * 1000,
        QVector3D(-1.7519347e-2, 9.6258730e0, -1.6711168e-1) * 1000,
        58232e3,                                                // Mean Radius in meters [ 183]
        "Saturn",
        QColor(210, 180, 140) // Tan
    );

    // 8. Uranus
    CelestialBody uranus(
        86.813e24,                                              // Mass in kg [ 206]
        QVector3D(1.5471279e9, 2.4743727e9, -1.0853613e7) * 1000,
        QVector3D(-5.8244389e0, 3.2929302e0, 8.7432431e-2) * 1000,
        25362e3,                                                // Mean Radius in meters [ 208]
        "Uranus",
        Qt::cyan
    );

    // 9. Neptune
    CelestialBody neptune(
        102.409e24,                                             // Mass in kg [ 403]
        QVector3D(4.4694103e9, 1.2090486e7, -1.0325104e8) * 1000,
        QVector3D(-4.9741308e-2, 5.4678427e0, -1.1175648e-1) * 1000,
        24624e3,                                                // Mean Radius in meters [ 405]
        "Neptune",
        Qt::darkBlue
    );

    // 10. Pluto
    CelestialBody pluto(
        1.307e22,                                               // Mass in kg [ 232]
        QVector3D(2.8200436e9, -4.4571636e9, -3.3878263e8) * 1000, // [ 238]
        QVector3D(4.7262398e0, 1.6817726e0, -1.5550565e0) * 1000,
        1188.3e3,                                               // Mean Radius in meters [ 233]
        "Pluto",
        QColor(240, 230, 140) // Khaki
    );

    // 11. Ceres
    CelestialBody ceres(
        9.38e20,                                                // Mass calculated from GM=62.6284 [ 259]
        QVector3D(4.3466053e8, 1.9686860e6, -8.0076876e7) * 1000, // [ 261]
        QVector3D(-6.2687332e-1, 1.6675374e1, 6.4548741e-1) * 1000,
        469.7e3,                                                // Radius in meters [ 259]
        "Ceres",
        Qt::white
    );

    // 12. Vesta
    CelestialBody vesta(
        2.59e20,                                                // Mass calculated from GM=17.28828 [ 319]
        QVector3D(-7.6558530e7, -3.1260018e8, 1.8527608e7) * 1000,
        QVector3D(2.0461121e1, -5.1493079e0, -2.3374076e0) * 1000, // [ 321]
        261.385e3,                                              // Radius in meters [ 319]
        "Vesta",
        QColor(160, 160, 160)
    );

    // 13. Pallas
    CelestialBody pallas(
        2.04e20,                                                // Mass calculated from GM=13.63 [ 342]
        QVector3D(3.4012674e8, -3.2453223e8, 1.9488659e8) * 1000, // [ 344]
        QVector3D(1.0146614e1, 7.7337061e0, -6.2362275e0) * 1000,
        256.5e3,                                                // Radius in meters [ 342]
        "Pallas",
        QColor(200, 200, 200)
    );

    // 14. Hygiea
    CelestialBody hygiea(
        1.05e20,                                                // Mass calculated from GM=7. [ 365]
        QVector3D(1.6133008e8, 4.9235072e8, 1.8089693e7) * 1000,  // [ 367]
        QVector3D(-1.4481833e1, 4.2567106e0, -8.8118728e-1) * 1000,
        203.56e3,                                               // Radius in meters [ 365]
        "Hygiea",
        QColor(140, 140, 140)
    );

    // 15. Eris
    CelestialBody eris(
        1.62e22,                                                // Mass calculated from avg GM=1079.2 [ 275]
        QVector3D(1.2757080e10, 5.8852690e9, -2.6371115e9) * 1000, // [ 280]
        QVector3D(-8.0893598e-1, 1.4881423e0, 1.6214815e0) * 1000,
        1163e3,                                                 // Radius in meters [ 275]
        "Eris",
        QColor(255, 228, 196) // Bisque
    );

    // 16. Haumea
    CelestialBody haumea(
        3.96e21,                                                // Mass calculated from GM=264.413 [ 295]
        QVector3D(-5.5673355e9, -3.4965085e9, 3.5257792e9) * 1000,
        QVector3D(2.3834145e0, -3.0478895e0, -2.2002487e-1) * 1000,
        799e3,                                                  // Approx. mean radius in meters
        "Haumea",
        Qt::white
    );

    // 17. Interamnia
    CelestialBody interamnia(
        7.49e19,                                                // Mass calculated from GM=5. [ 388]
        QVector3D(-5.0248966e8, -3.5743281e7, -1.5590988e8) * 1000, // [ 390]
        QVector3D(7.2042109e-1, -1.4589749e1, -5.8582291e-1) * 1000,
        153.1565e3,                                             // Radius in meters [ 388]
        "Interamnia",
        QColor(100, 100, 100)
    );


    // Add all bodies to the simulation
    simulation.addBody(sun);
    simulation.addBody(mercury);
    simulation.addBody(venus);
    simulation.addBody(earth);
    simulation.addBody(mars);
    simulation.addBody(jupiter);
    simulation.addBody(saturn);
    simulation.addBody(uranus);
    simulation.addBody(neptune);
    simulation.addBody(pluto);
    simulation.addBody(ceres);
    simulation.addBody(vesta);
    simulation.addBody(pallas);
    simulation.addBody(hygiea);
    simulation.addBody(eris);
    simulation.addBody(haumea);
    simulation.addBody(interamnia);

    // Set up the window
    window.setWindowTitle("Solar System Simulator");
    window.resize(1000, 800);
    window.show();

    // Start the simulation timer
    simulation.start();

    return app.exec();
}