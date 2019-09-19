# Basic Usage

To get started with the framework, you will want to add a new folder called projects in the root of the framework. This folder will contain all of our projects that we want to add to our solution. The structure for these folders is as follows:

```cpp
premake_build.bat
premake5.exe
external
. . .
projects
   Project 1
      src
      res
   Project 2
      src
      res
   . . .
```

After adding a new folder for projects, you can run `premake_build.bat` to compile the solution (by default this will compile in VS 2019). If you need to change the Visual Studio version, or build for another IDE, you can modify the one-line `premake_build.bat`, and change `vs2019` to whatever platform is applicable. See the [premake wiki](https://github.com/premake/premake-core/wiki/Using-Premake) for all available platforms

# Project Layout
Projects consist of two folders, `res` and `src`. `res` will contain any files that should be copied to the build output. For instance, this is where you would want to put assets that you want to load in. `src` will contain all of the source code for the project. I would highly reccomend to use the `Show All Files` view in Visual Studio Solution Explorer when working in the framework.

# Generated folders
When compiling a project, your build tool will create 2 folders, `bin` for the output of the build, and `obj` for intermediate build files. These folders can be removed to save space when transferring the framework between devices. Visual Studio will also generate a hidden `.vs` folder, which can be safely deleted. 
>  Important: Do not delete the .git folder if you wish to track changes using GIT

# Sending/Submitting Projects
To send a project to someone else using the framework, you will only need to send them your project folder (ex: Project 1 from the example above)