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

Scripts that install build CERN ROOT with debug symbols and all options turned on can be found below. ROOT framework can be installed on pretty much any OS. However, prerequisities -- required list of libraries and their names vary depending on the specific Linux distribution used. I created two versions of scripts that will work for two common personal linux distribution families: Ubuntu-based linux systems and Fedora distributions. Installation instructions are following:

```
mkdir -p ~/Downloads && cd ~/Downloads

# For Ubuntu-based systems use:
wget -O install-root-latest.sh https://raw.githubusercontent.com/petrstepanov/ubuntu-scripts/main/install-root-latest.sh

# For Fedora linux use:
wget -O install-root-latest.sh https://raw.githubusercontent.com/petrstepanov/fedora-scripts/main/install-root-latest.sh

# Execute lines below:
chmod +x ./install-root-latest.sh
./install-root-latest.sh
```

If using other linux distribution, refer to the [list of the ROOT dependencies](https://root.cern/install/dependencies/) which I updated and tested on the most popular recent Linux distributions.

Above scripts locate the latest ROOT version on the CERN servers, download the framework source code to corresponding `~/Source/` folder, satisfy the dependencies, build the ROOT framework with debug symbols and most common options turned on, install ROOT binaries under `~/Applications/` folder and set up necessary environment variables.

Feel free to [reach out to me](https://petrstepanov.com/) with respect to any issues with the script (or open an Issue in corresponding GitHub repo). Find more information about compilation of CERN ROOT from source [here](https://root.cern/install/build_from_source/).

## CMake approach
CMake option comes with benefits. CMake can automatically generate cross-platform makefiles, detect external libraries. In particular CMake can pcreate a project structure that can be later opened in IDE of your choice (Eclipse, Visual Studio,…). Then you can enjoy such features like code autocompletion, hilighting and debugging.

### Setting up Eclipse project

First, install Eclipse IDE. This process is documented in the [Chapter 6 of my dissertation, pp. 87](https://petrstepanov.com/static/petr-stepanov-dissertation-latest.pdf). Make sure to increase Eclipse heap and indexer cache limits because CERN ROOT is a rather resourceful framework. 

Next, check out the Git repository into the desired location on your computer. I usually keep most of the Git repositories in `~/Development` folder. First we check out the Git repository.
```
mkdir -p ~/Development && cd ~/Development
git clone https://github.com/petrstepanov/root-eclipse
```

**Extensive debugging requires accessing ROOT source files in Eclipse project**. In order to do it we create a symlink under project's `src` folder pointing to the CERN ROOT source files Tip: it is important to do it [before building project with CMake](https://stackoverflow.com/questions/48509911/cmake-add-subdirectory-vs-include). I outlined a good approach of consolidating CERN ROOT source files from the original ROOT archive [here](https://github.com/petrstepanov/fedora-scripts/blob/main/copy-root-sources.sh). This will work out of the box if you installed ROOT with my scripts above:
```
cd ~/Downloads
wget -O copy-root-sources.sh https://raw.githubusercontent.com/petrstepanov/fedora-scripts/main/copy-root-sources.sh
chmod +x ./copy-root-sources.sh
./copy-root-sources.sh
```

Above code will copy all ROOT sources under `~/Source/root-sources`. Next we symlink them under our the project's `src` folder:

```
ln -s ~/Source/root-sources/ ~/Development/root-eclipse/src/root-sources
```

The out-of-source project generator build is carried out in separate folder located outside of the actual project Git tree. For instance, we will create a `root-eclipse-project` folder. Build is initinitiated via following commands:
```
cd ~/Development
mkdir root-eclipse-project && cd root-eclipse-project
cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../root-eclipse
```

Now project is set up and ready to be loaded. Open Eclipse and go to File → Open Projects from File System... Specify the project location in the modal dialog by clicking the "Directory..." button. Locate the `~/Development/root-eclipse-project` project folder. Click "Finish". Eclipse should start to index the project automatically. Depending on the speed of your hard drive and memory indexing will require from several minutes to about an hour.

**Optional step**. Cmake links program source folder to the project twice: as `[Source directory]` and `[Subprojects]`. For some reaso, indexer is not working correctly when source code is opened in `[Source directory]`. Therefore we will filter out the `[Source Folder]` contents from the build and indexer. Right click the `[Source directory]` node, select "Properties". Go to "Resouce" → "Resource Filters". Click "Add Filter...". Select option buttons "Exclude All", "Files and Folders". In "Filter Details" specify "Name", "matches", "*". Click "Ok", "Apply and Close".

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
This option might seem rudimentary. However it gives a good understanding of building a stand-alone CERN ROOT project. Also this option is useful for running the code if user does not have root permisions on computer. For instance, if working on the remote computer.

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
Finally, an executable with debug symbols can be produced. Please note that it is better to compile ROOT framework with debug symbols as well.
