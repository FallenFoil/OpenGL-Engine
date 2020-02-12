# Windows

## Engine

### Executing

This repository provides the executable. Just open the file *engine.exe* in the *bin* folder.

### Compiling with Visual Studio

To compile with Visual Studio, follow these steps:
- Open CMake and provide the path to the source code folder (*src*) and the binary folder (*bin*), then hit **configure**. CMake will say that *GLUT*, *GLEW*, *DevIL* and *AntTweakBar* don't exists and you need to provide the path to it. Give the path to the *toolkit* folder, then it **configure** again. When configure ends, hit **Generate**M;
- Open the project with Visual Studio, choose **Release** instead to **Debug**. Hit start (green arrow);
- If you encounter the following error: "*'exit': redefinition; '__declspec(noreturn)' or '[[noreturn]]' differs*" do this:
  - Right click on the project name in the Solution Explorer tab and select Properties -> C/C++ -> Preprocessor -> Preprocessor definitions;
  - Append **GLUT_BUILDING_LIB** to the existing definitions.
  - Try starting again.