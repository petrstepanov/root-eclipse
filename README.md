# root-eclipse
A template repository for CERN ROOT based C++ project. Repository contains two makefiles. It can be built with GNU Make and CMake. Additionaly, with the help of CMake generators project can be easily set up in Eclipse IDE for enchanced debugging and development.

CERN ROOT libraries must be installed on the system in order to carry out the build. 

### Recommended OS for software development

Red Hat Enterprise Linux (RHEL) is a gold standard in linux family operating systems for commercial market. RHEL is open-source distribution that comes with paid support. Within the past decade scinetific community (organizations like CERN or Fermilab and JLab) were utilizing a free RHEL clone named CentOS. CentOS is originally a RHEL fork, a community driven project that established good relationships with Red Hat headquaters and even received funding and support since 2014. 

CentOS is currently installed on majority of scientific computing environments. Computing environment nodes are usually rich with CPU and memory resources and meant to be used for running final calculations that process large amounts of data. However, initial program development and debugging often needs to be carried out on a personal computer.

Red Hat provides a free operating system named Fedora that is based on RHEL but oriented more for personal computers rather than enterprise use. Red Hat utilizes Fedora to test the bleeding edge technologies that later might be merged into RHEL. Fedora has the same package structure (RPM) as the RHEL and CentOS. Also Fedora uses the same package manager (YUM, later DNF).

Similarities between RHEL, CentOS and Fedora strongly point towards selecting Fedora as a distribution of choice for the software development of the ROOT-based programs for scientific application.

## Installing CERN ROOT on personal computer

I highly encourage everyone to build ROOT on their personal computer from source. This way many potential issues can be eliminated: mismatch between compiler versions and Python versions installed on your local computer. When compiling ROOT from source, it is possible to turn on a few optional features that are not available in the pre-compiled executable version.

Scripts that install most recent CERN ROOT (and Geant4) version on Fedora linux [can be found here](https://github.com/petrstepanov/fedora-scripts). Scripts builds ROOT with debug symbols and all options turned on. Geant4 is built with debug symbols and most common options. Installation instructions are following:
```
cd ~/Downloads
git clone https://github.com/petrstepanov/fedora-scripts && cd ~/Downloads
chmod +x install-root-latest.sh
sh ./install-root-latest.sh
chmod +x install-geant4-latest.sh
sh ./install-geant4-latest.sh
```

Scripts locate the latest ROOT and Geant versions on the CERN servers, download the framework source code to corresponding `~/Source/` folder, satisfyes the dependencies, build the frameworks, installs binaries in `~/Applications/` folder and sets up the environment variables.

Feel free to reach out to [Petr Stepanov](https://github.com/petrstepanov/) with respect to the scripts issues. Find more information about compilation of CERN ROOT from source [here](https://root.cern/install/build_from_source/). Geant4 build documentation can be found [here](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/installguide.html).

ROOT (and Geant4) can be installed on pretty much any OS. However, prerequisities -- required list of libraries and their names vary depending on the specific Linux distribution used. Please refer to the [list of the ROOT dependencies](https://root.cern/install/dependencies/) which I have updated and tested on the most popular recent Linux distributions.

## CMake approach
CMake option comes with benefits. CMake can automatically generate cross-platform makefiles, detect external libraries. In particular CMake can pcreate a project structure that can be later opened in IDE of your choice (Eclipse, Visual Studio,…). Then you can enjoy such features like code autocompletion, hilighting and debugging.

### Setting up Eclipse project

First, install Eclipse IDE. This process is documented in the [Chapter 6 of my dissertation](https://petrstepanov.com/static/petr-stepanov-dissertation-latest.pdf). Next, check out the Git repository into the desired location on your computer. I usually keep most of the Git repositories in `~/Development` folder. First we check out the Git repository.
```
mkdir -p ~/Development && cd ~/Development
git clone https://github.com/petrstepanov/root-eclipse
cd ../
mkdir root-eclipse-project
cd root-eclipse-project
```
The out-of-source project generator build is initiated via following command:
```
cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../root-eclipse
```

Next we need to set up an Eclipse project. Thankfully, CMake has an automated way of generating the Eclipse project. This saves quite a time for application developers. The general CMake command is `cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../project/source`, where the `../project/source` path must be relative or absolute path of the project source folder that contains Cmake cache file `CMakeLists.txt`.  Generally speaking there are two options:
1. **In-source build**. Eclipse project folder files are located the source program folder next to the `CMakeLists.txt` file. It is not favorable because project files will interfere with the Git tree and multiple excludes in `.gitignore` will be required. Also I've experienced Eclipse indexer issued using this method.
2. **Out-of-source build**. Eclipse project is located outside of the Git repository. This option is preferrable because the project, build and executable files are separated trom the Git project tree. However, in this option I ran into a problem, namely the Makefile targets were not generated automatically.

So, there seem to be problems with each type of build. However, it turned out that the out-of-source project build _located inside the Git tree_ works without any issues. Also the project folder can be simply excluded in `.gitignore` with one line. We will follow this option now and create a directory for the Eclipse project.
```
cd ~/Development/glass-prototype/
mkdir eclipse && cd eclipse
```

Note, `eclipse` folder is already added to the `.gitignore`. The out-of-source project generator build is initiated via following command:
```
cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../source
```

Cmake will create an Eclipse project and will link ROOT and Geant4 includes (header files) into the project. However **extensive debugging requires access to Geant and ROOT source files**. An effective way of linking ROOT and Geant4 framework source files to Eclipse project is following:
* Copy all ROOT and Geant4 source files in a same parent folder without nested directory structure. This greatly improves Eclipse indexing speed.
* Create a symlink to this folder in `eclipse` project folder.
* In Eclipse mark above symlink as *Source Folder*.

If ROOT and Geant4 were installed with the suggested scripts on Fedora, their sources can be copied to a dedicated parent folder `~/Source/root-geant-sources/` via a simple script:
```
cd ~/Downloads
git clone https://github.com/petrstepanov/fedora-scripts && cd ~/Downloads
chmod +x copy-root-geant-sources.sh
sh ./copy-root-geant-sources.sh
```

Next, create a symlink in the Eclipse project tree: 
```
cd ~/Development/glass-prototype/eclipse
ln -s ~/Source/root-geant-sources/
```

Open Eclipse and go to File → Open Projects from File System... Specify the project location in the modal dialog by clicking the "Directory..." button. Locate the `~/Development/glass-prototype/eclipse` project folder. Click "Finish". 

Next we need to tweak Eclipse project structure in order for the Eclipse Indexer to work correctly:

* Firstly, Cmake links program source folder to the project twice: as `[Source directory]` and `[Subprojects]`. Indexer is not working correctly when source code is opened in `[Source directory]`. Therefore we will filter out the `[Source Folder]` contents from the build and indexer. Right click the `[Source directory]` node, select "Properties". Go to "Resouce" → "Resource Filters". Click "Add Filter...". Select option buttons "Exclude All", "Files and Folders". In "Filter Details" specify "Name", "matches", "*". Click "Ok", "Apply and Close".
* Secondly the `root-geant-sources` symlink with framework source code needs to be accessed by the indexer. We need mark it as *Source Folder*. Right click the top most project node. Select `New → Source Folder`. Specify the `root-geant-sources` symlink.
* Lastly, we will need to add the `draw` folder to the sources list to ensure proper indexing of the ROOT scripts. Right click the top project node and aelect `New → Source Folder`. Specify `./[Subprojects]/draw` folder.

Finally in Eclipse menu select `Prooject → C/C++ Index → Rebuild`. Depending on the speed of your hard drive and memory indexing will require from several minutes to about an hour.

More information regarding the CMake Eclipse generator can be found: [on Mantid project page](https://www.mantidproject.org/Setting_up_Eclipse_projects_with_CMake), [on Javier V. Gómez website](https://jvgomez.github.io/pages/how-to-configure-a-cc-project-with-eclipse-and-cmake.html), [on Cmake Wiki](https://gitlab.kitware.com/cmake/community/-/wikis/home), [again on Cmake Wiki](https://gitlab.kitware.com/cmake/community/-/wikis/doc/editors/Eclipse-CDT4-Generator) and [again on Cmake Wiki](https://gitlab.kitware.com/cmake/community/-/wikis/doc/editors/Eclipse-UNIX-Tutorial).

#### Setting up the Debug and Run Configurations

Besides the Geant4 source files compiled into the `glass` executable, source code contains some ROOT scriupts under the `./draw/` folder. These scripts are designated for plotting and analyzing the output data. Debug configuration for the `glass` executable as well as the run configurations for the above scripts are needed to be set up in the Eclipse IDE.

We will start from setting up the main Debug configuration for Geant4 `glass` program.

1. Expand 🎯 Build Targets under the main project node in the Project Explorer window. Double-click "All" to compile the executable.
2. In menu "Run" open "Debug configurations". Right click "C/C++ Application" and select "New Configuration".
3. Click "Browse" next to the "Project" field. Select "glass-Debug@eclipse"
4. Click "Search Project" next to the "C/C++ Application" field. Specify the "glass" execuable built earlier.
5. Select "Enable Auto Build"
6. Go to the "Debugger" tab. Uncheck "Stop on startup at:".

Follow the below steps to create run configurations for every ROOT script under the `./draw/folder`:

1. In menu "Run" open "Run Configurations".  Right click "C/C++ Application" and select "New Configuration".
2. In the "Name" field enter the name of the ROOT script you want to create the configuration for, e.g. `scintTransDet` (or `detectors`, `materials`)
3. Click "Browse" button next to the "C/C++ Application" field. Locate your CERN "root" executable file. Mine is installed under `~/Applications/root.../bin/root`.
4. Select "Enable auto build". This is importnat because before running the root scripts they need to be copied to the `./eclipse` folder. `CMakeLists.txt` takes care of that.
5. Go to "Arguments" tab. Specify the name of the ROOT script from the `./draw` folder to be launched, e.g. `./draw/scintTransDet.cpp`.

Repeat the above process for any script you would like to run in the Eclipse. Eclipse configuration outlined above contains ROOT source files and headers. Therefore code navigation and autocompletion for the ROOT scripts does work.


### Setting up project in Eclipse
Project contains test CMakeLists.txt to generate a ROOT-based eclipse project. It is better to generate Eclipse project out-of-source:
```
cd ../
mkdir root-eclipse-project
cd root-eclipse-project
cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../root-eclipse
```
### Adding ROOT sources to Eclipse


## Compiling and installing with GNU Make
This option might seem rudimentary. However it gives a good understanding of about building a CERN ROT project. Also this option is useful for running the code if user does not have root permisions on computer.

GNU makefile is located in the root project folder and named `Makefile-GNU`. Makefile is universal and can be used on macOS and Linux. First, define the executable name on the line 20:
```
APP_NAME=my-application
```
Next, open up the Terminal and compile the project. There are a few options to build the executable.
```
make Makefile_GNU production
make Makefile_GNU install
```
Above command will produce the dictionary, compile the shared library, compile source files, link the executable. Followed by the installation of the shared library to the default ROOT library location and installation of the executable.
```
make Makefile_GNU release
```
Release option is useful on a computer where you do not have administrative permissions. Installation of the shared library or modifying the shared libraries include path is not possible. Here produced executable is linked against the shared library with a relative path. Therefore user can still run the program.
```
make Makefile_GNU debug
```
Finally, an executable with debug symbols can be produced. Please not that it is better to compile ROOT framework with debug symbols as well.
