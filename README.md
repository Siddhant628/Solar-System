# Solar System Simulation

A simulation of the solar system in DirectX 11 using a framerwork developed by Paul Varcholik - [Framework](https://bitbucket.org/pvarcholik/directx-essentials-livelessons).

Run **SolarSystem project** in **DirectX solution** to run the simulation. Use **WASD** and **mouse** to move around.

In the simulation, a point light is used to illuminate planets using diffuse lighting. Astronomical objects support ambient light and the light attenuates with distance from the sun.

Each planet follows a circular orbit around the Sun. And the Sun isn't of the right scale for visual reasons. However, each astronomical object has the right scale, orbital distance, orbital period, rotational period and axial tilt. The earth has its moon as well, and it is modeled with all the factors that other astronomical objects are modeled with.
