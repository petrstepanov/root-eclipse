# Template Project for Debuging CERN ROOT Scripts in Eclipse IDE

**Please refer to the main article published on the [CERN ROOT blog](https://root.cern/blog/debuging-root-scripts-in-eclipse)**.

This repository demonstrated how to convert a ROOT script (meant to be executed with Cint interpreter) into a ROOT-based program that can be compiled into an executable and used with a debugger. 

Copy your ROOT script(s) and place them in next in the `src/` project folder. Include your ROOT script in the `src/main.cpp` file and call your script entry point function from `main()` in `src/main.cpp`:

```
// Include your script file
#include <yourRootScript.C>

int main(int argc, char **argv) {
	...

	// Call your ROOT script entry function
	yourRootScript();

	...
}
```

For the build to succeed, we need to satisfy following criteria:

* As opposed to running your ROOT script with Cling interpreter, standalone build requires to include headers `#include <...>` to be explicitly defined in the `src/yourRootScript.C` file for every class used in the program.

* Depending on your script code, extra ROOT libraries may need to be specified in `CMakeLists.txt` with CMake `list(APPEND LIB_NAMES "<root-library-name>")` command. List of available extra ROOT libraries [can be found here](https://cliutils.gitlab.io/modern-cmake/chapters/packages/ROOT.html#the-right-way-targets).

* Some of the ROOT classes require generation of dictionary and shared library. These are: GUI classes that utilize signals and slots functionality; classes with implemented ClassDef and ClassImp directives (that use functions like ClassName(), InheritsFrom() etc...); custom RooFit PDF classes inherited from RooAbsPdf, etc. For every such class, add a corresponding line in the `src/LinkDef.h` file: `#pragma link C++ class MyClassThatRequiresLibrary+`.

* If adding new C++ and header files to the project, simply place them under the `src/` folder. CMake will find them automatically.

Now your ROOT script is transformed into a ROOT-based program that is ready to be set up in Eclipse IDE with CMake4Eclipse plugin.

> Feel free to continue reading setup instructions of a CERN ROOT-based project in Eclipse IDE on [the ROOT blog](https://root.cern/blog/debuging-root-scripts-in-eclipse/).