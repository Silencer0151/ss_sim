Solar System Simulation

This project is a real-time, interactive simulation of our solar system, built with C++ and the Qt framework. Its primary goal is to accurately model the gravitational forces between celestial bodies, allowing for realistic orbital mechanics. The project is designed with a modular and scalable architecture, enabling future expansion to include stunning 3D visualizations, asteroid fields, and interactive time controls.

Project Philosophy and Design

The simulation is built on a few core design principles:

    Separation of Concerns: The project's code is divided into distinct, self-contained components for physics, visualization, and the main application loop. This modular approach makes the codebase easier to manage, test, and debug.

    Performance: Using C++ allows for low-level control and high computational efficiency, which is crucial for running complex N-body physics calculations in real time.

    Rich User Interface: The Qt framework was chosen to provide a robust and professional GUI. This allows for the creation of an intuitive interface with controls for the simulation without having to build the entire UI system from scratch.

File and Directory Breakdown

Here is a detailed explanation of the role of each file and directory in the project:

Root Directory

    CMakeLists.txt: This is the master build file for the project. It tells CMake how to compile the C++ source files, link the necessary Qt libraries, and create the final executable. It manages the dependencies and build configurations for the entire project.

    main.cpp: This is the application's entry point. It's responsible for setting up the Qt application, creating an instance of the NBodySimulation and SolarSystemWidget classes, connecting them, and starting the main event loop.

src/physics/

This directory is the heart of the simulation, containing all the logic for the gravitational physics.

    CelestialBody.h and CelestialBody.cpp: These files define and implement the CelestialBody class. This is the fundamental data structure for every object in the simulation. It stores an object's physical properties such as mass, position, velocity, and a name for identification. It is a passive data container, holding the state of a single object.

    NBodySimulation.h and NBodySimulation.cpp: These files define and implement the NBodySimulation class. This is the physics engine of the project. It holds a collection of all CelestialBody objects. In a continuous loop, it calculates the gravitational force between every pair of bodies and then updates their positions and velocities for a small time step.

src/visualization/

This directory handles everything related to rendering and user interaction.

    SolarSystemWidget.h and SolarSystemWidget.cpp: These files define and implement a custom Qt widget that acts as the canvas for the simulation. It's the visual component that receives data from the NBodySimulation and draws the celestial bodies to the screen. It also contains the logic to handle user input for future features like camera controls, panning, zooming, and rotation.

