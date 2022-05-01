# AugmentedReality

A simple AR application using WPF, OpenCV, and OpenGL

![image](https://user-images.githubusercontent.com/6503344/165653318-ba68bed1-8688-4b75-a0bf-2b6d77be0e52.png)

## Compilation

### Preconditions

The following are necessary for building this project
* Visual Studio 2022
* Latest Conan
* Latest CMake (necessary for building some Conan dependencies)

### Steps

Run `./build-dependencies.ps1 [configurations]` to generate the necessary dependencies required to build the solution. You may pass either `Release`, `Debug`, or `Release,Debug` to generate the dependencies for the corresponding configuration. By default, all are generated. This step will probably take a long time, but it only needs to be done once.

Once completed, open the solution in Visual Studio and simply build it.
