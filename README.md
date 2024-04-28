# SpectralRift Engine

Simple Game/App Engine written in C++20, focused on writing and maintaining a single codebase and making sure it runs everywhere with minimal efforts.

## Modules
SpectralRift is heavily centered under a custom Module System which allows the customization of the engine. These are some of the bundled ones:

- **EngineCore**
  - Contains all the core functionality of the engine that can be extended accordingly by other modules.
- **BackendOpenGL**
  - Contains a basic implementation for the OpenGL Graphical Backend which is used by the engine for graphical-related stuff.
- **PlatformNX**
  - Contains platform-specific implementations for the NX (Switch) Game Console.
- **PlatformPlayStation2**
  - Contains platform-specific implementations for graphics, input, sound and rendering on the Sony PlayStation 2 game console.
- **PlatformUnix**
  - Contains platform-specific implementations for input, sound and window management under most Unix-based Operating Systems.
- **PlatformWindows**
  - Contains platform-specific implementations for input, sound and window management under the Windows Operating System.

## Platforms
Platform support is currently under heavy development, but these all the platforms on which SpectralRift will be able to run in the future and their status:

- [ ] **Windows**
  - [ ] Window Creation and Management
  - [ ] Input
  - [ ] Sound
- [ ] **Unix**
    - [ ] Window Creation and Management
    - [ ] Input
    - [ ] Sound
- [ ] **NX**
    - [ ] Window Management
    - [ ] Input
    - [ ] Sound
- [ ] **PlayStation 2**
    - [ ] Graphics Rendering
    - [ ] Input
    - [ ] Sound

# Configuration

SpectralRift provides a simple way on how to configure things, and that is through custom definitions. You may set them while building the engine, or use "set" calls from your project to set everything accordingly.

- **ENGINE_LIBRARY_TYPE**
  - This value specifies whether the library will be built statically (the value must be **STATIC**) or as a shared library (the value must be **SHARED**)
  - It defaults to **SHARED**
- **ENGINE_MODULES_DIRS**
  - This value contains a semicolon-separated list of paths in which the engine will look for modules specified under the **ENGINE_MODULES** variable.
  - By default, it will look in the "<engine root>/modules" folder.
- **ENGINE_MODULES**
  - This value must contain a semicolon-separated list of module names for which they will be looked after inside the paths specified under the **"ENGINE_MODULES_DIRS"** variable. The engine will be linked with those extensions.
  - It defaults to **"EngineCore"**