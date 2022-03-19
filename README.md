# AugmentedReality
A simple AR application using WPF, OpenCV, and OpenGL

![image](https://user-images.githubusercontent.com/6503344/158944092-62ac313e-38f6-4a3e-9c48-6ec5c3a8ab6d.png)
![image](https://user-images.githubusercontent.com/6503344/158944123-91f2aaec-9d7f-40b6-a37e-98cad894ce5f.png)

## Compilation
Run `./build-dependencies.ps1 [configurations]` to generate the necessary dependencies required to build the solution. You may pass either `Release`, `Debug`, or `Release,Debug` to generate the dependencies for the corresponding configuration. By default, all are generated. This step will probably take a long time, but it only needs to be done once.

Once completed, open the solution in Visual Studio and simply build it.
