# Template Project for Standalone Build of a CERN ROOT based application
A template repository for CERN ROOT based C++ project. Project supports CMake and GNU Make builds. Additionaly, with the help of CMake generators project can be easily set up in Eclipse IDE for enchanced debugging and development.

When writing your own ROOT program, place your sources and headers under the `src` folder. Also make sure you add corresponding class names to the `LinkDef.h` file for proper dictionary deneration.

### Recommended OS for software development

Red Hat Enterprise Linux (RHEL) is a gold standard in linux family operating systems for commercial market. RHEL is open-source distribution that comes with paid support. Within the past decade scinetific community (organizations like CERN or Fermilab and JLab) were utilizing a free RHEL clone named CentOS. CentOS is originally a RHEL fork, a community driven project that established good relationships with Red Hat headquaters and even received funding and support since 2014. 

CentOS is currently installed on majority of scientific computing environments. Computing environment nodes are usually rich with CPU and memory resources and meant to be used for running final calculations that process large amounts of data. However, initial program development and debugging often needs to be carried out on a personal computer.

Red Hat provides a free operating system named Fedora that is based on RHEL but oriented more for personal computers rather than enterprise use. Red Hat utilizes Fedora to test the bleeding edge technologies that later might be merged into RHEL. Fedora has the same package structure (RPM) as the RHEL and CentOS. Also Fedora uses the same package manager (YUM, later DNF).

Similarities between RHEL, CentOS and Fedora strongly point towards selecting Fedora as a distribution of choice for the software development of the ROOT-based programs for scientific application.

## Installing CERN ROOT on personal computer

I highly encourage everyone to build ROOT on their personal computer from source. This way many potential issues can be eliminated: mismatch between compiler versions and Python versions installed on your local computer. When compiling ROOT from source, it is possible to turn on a few optional features that are not available in the pre-compiled executable version. Finally, any serious development requires debugging functionality which is only possible when compiling ROOT from source.

Scripts that install most recent CERN ROOT version on Fedora linux [can be found here](https://github.com/petrstepanov/fedora-scripts). Scripts builds ROOT with debug symbols and all options turned on. Geant4 is built with debug symbols and most common options. Installation instructions are following:
```
mkdir -p ~/Downloads && cd ~/Downloads
git clone https://github.com/petrstepanov/fedora-scripts && cd ~/Downloads
chmod +x install-root-latest.sh
sh ./install-root-latest.sh
```

Scripts locate the latest ROOT and Geant versions on the CERN servers, download the framework source code to corresponding `~/Source/` folder, satisfyes the dependencies, build the frameworks, installs binaries in `~/Applications/` folder and sets up the environment variables.

Feel free to reach out to [Petr Stepanov](https://github.com/petrstepanov/) with respect to the scripts issues. Find more information about compilation of CERN ROOT from source [here](https://root.cern/install/build_from_source/).

ROOT frameworkcan be installed on pretty much any OS. However, prerequisities -- required list of libraries and their names vary depending on the specific Linux distribution used. If you are using other distribution than Fedora, refer to the [list of the ROOT dependencies](https://root.cern/install/dependencies/) which I have updated and tested on the most popular recent Linux distributions.

## CMake approach
CMake option comes with benefits. CMake can automatically generate cross-platform makefiles, detect external libraries. In particular CMake can pcreate a project structure that can be later opened in IDE of your choice (Eclipse, Visual Studio,…). Then you can enjoy such features like code autocompletion, hilighting and debugging.

### Setting up Eclipse project

First, install Eclipse IDE. This process is documented in the [Chapter 6 of my dissertation](https://petrstepanov.com/static/petr-stepanov-dissertation-latest.pdf). Next, check out the Git repository into the desired location on your computer. I usually keep most of the Git repositories in `~/Development` folder. First we check out the Git repository.
```
mkdir -p ~/Development && cd ~/Development
git clone https://github.com/petrstepanov/root-eclipse
```
The out-of-source project generator build is initiated via following command:
```
mkdir root-eclipse-project && cd root-eclipse-project
cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../root-eclipse
```
Cmake will create an Eclipse project and will link ROOT (and Geant4) includes (header files) into the project. However **extensive debugging requires access to ROOT source files**. An effective way of linking ROOT and Geant4 framework source files to Eclipse project is following:
* Copy all ROOT source files in a same parent folder without nested directory structure. This greatly improves Eclipse indexing speed.
* Create a symlink to this folder in `eclipse` project folder.
* In Eclipse mark above symlink as *Source Folder*.

If ROOT was installed with the suggested scripts on Fedora, ROOT source files can be copied to a dedicated parent folder `~/Source/root-sources/` via a simple script:
```
cd ~/Downloads
git clone https://github.com/petrstepanov/fedora-scripts && cd ~/fedora-scripts
chmod +x copy-root-sources.sh
sh ./copy-root-sources.sh
```

Next, create a symlink in the Eclipse project tree: 
```
cd ~/Development/root-eclipse-project
ln -s ~/Source/root-geant-sources/
```

Open Eclipse and go to File → Open Projects from File System... Specify the project location in the modal dialog by clicking the "Directory..." button. Locate the `~/Development/root-eclipse-project/eclipse` project folder. Click "Finish". 

Next we need to tweak Eclipse project structure in order for the Eclipse Indexer to work correctly:

* Firstly, Cmake links program source folder to the project twice: as `[Source directory]` and `[Subprojects]`. Indexer is not working correctly when source code is opened in `[Source directory]`. Therefore we will filter out the `[Source Folder]` contents from the build and indexer. Right click the `[Source directory]` node, select "Properties". Go to "Resouce" → "Resource Filters". Click "Add Filter...". Select option buttons "Exclude All", "Files and Folders". In "Filter Details" specify "Name", "matches", "*". Click "Ok", "Apply and Close".
* Secondly the `root-sources` symlink with framework source code needs to be accessed by the indexer. We need mark it as *Source Folder*. Right click the top most project node. Select `New → Source Folder`. Specify the `root-sources` symlink.

Finally in Eclipse menu select `Prooject → C/C++ Index → Rebuild`. Depending on the speed of your hard drive and memory indexing will require from several minutes to about an hour.

### Setting up Eclipse debug and run configurations

Besides the Geant4 source files compiled into the `root-eclipse` executable, source code contains some ROOT scriupts under the `./draw/` folder. These scripts are designated for plotting and analyzing the output data. Debug configuration for the `root-eclipse` executable as well as the run configurations for the above scripts are needed to be set up in the Eclipse IDE.

We will start from setting up the main Debug configuration for Geant4 `root-eclipse` program.

1. Expand 🎯 Build Targets under the main project node in the Project Explorer window. Double-click "All" to compile the executable.
2. In menu "Run" open "Debug configurations". Right click "C/C++ Application" and select "New Configuration".
3. Click "Browse" next to the "Project" field. Select "root-eclipse-Debug@eclipse"
4. Click "Search Project" next to the "C/C++ Application" field. Specify the "root-eclipse" execuable built earlier.
5. Select "Enable Auto Build"
6. Go to the "Debugger" tab. Uncheck "Stop on startup at:".

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
