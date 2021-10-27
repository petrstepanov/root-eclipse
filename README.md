<img src="https://raw.githubusercontent.com/petrstepanov/root-eclipse/main/resources/setup-eclipse-ide-with-cern-root.jpg" width="100%" alt="How to set up and debug CERN ROOT project in Eclipse IDE">

# Template project for developing and building a standalone CERN ROOT based application in Eclipse IDE
This is a template repository for a CERN ROOT based C++ project. Project supports CMake and GNU Make builds. Additionaly, with the help of CMake generators project can be easily set up in Eclipse IDE for enchanced debugging and development.

When writing your own ROOT program, place your sources and headers under the `src` folder. Also make sure you add corresponding class names to the `LinkDef.h` file for proper dictionary generation.

## Recommended OS for software development

Red Hat Enterprise Linux (RHEL) is a gold standard in linux family operating systems for commercial market. RHEL is open-source distribution that comes with paid support. Within the past decade scinetific community (organizations like CERN or Fermilab and JLab) were utilizing a free RHEL clone named CentOS. CentOS is originally a RHEL fork, a community driven project that established good relationships with Red Hat headquaters and even received funding and support since 2014. 

CentOS is currently installed on majority of scientific computing environments. Computing environment nodes are usually rich with CPU and memory resources and meant to be used for running final calculations that process large amounts of data. However, initial program development and debugging often needs to be carried out on a personal computer.

Red Hat provides a free operating system named Fedora that is based on RHEL but oriented more for personal computers rather than enterprise use. Red Hat utilizes Fedora to test the bleeding edge technologies that later might be merged into RHEL. Fedora has the same package structure (RPM) as the RHEL and CentOS. Also Fedora uses the same package manager (YUM, later DNF).

In my point of view, similarities between RHEL, CentOS and Fedora strongly lead towards selecting Fedora as a distribution of choice for the software development of the ROOT-based programs for scientific application. However, any linux distribution should work just fine for development of a C++ ROOT project in Eclipse.

I consider development of ROOT-based software on macOS more complicated compared to Linux. Apple introduced code signing procedures that I was never able to figure out. From that point in time I am developing C++ programs on Linux.

## Installing CERN ROOT on personal computer

I highly encourage everyone to build ROOT on their personal computer from source. This way many potential issues can be eliminated: mismatch between compiler versions and Python versions installed on your local computer. When compiling ROOT from source, it is possible to turn on a few optional features that are not available in the pre-compiled executable version. Finally, any serious development requires debugging functionality which is only possible when compiling ROOT from source.

Scripts that install build CERN ROOT with debug symbols and all options turned on can be found below. ROOT framework can be installed on pretty much any OS. However, prerequisities - required list of libraries and their names vary depending on the specific Linux distribution used. I created two versions of scripts that will work for common personal linux distribution families: Ubuntu-based linux systems and Fedora distributions. Installation instructions are following:

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

If using other linux distribution, refer to the [list of the ROOT dependencies](https://root.cern/install/dependencies/). I updated and tested these instructions the most popular recent Linux distributions in 2021.

Above scripts locate latest ROOT version on the CERN servers, download the framework source code to corresponding `~/Source/` folder, satisfy the dependencies, build the ROOT framework with debug symbols and most common options turned on, install ROOT binaries under `~/Applications/` folder and set up necessary environment variables.

Feel free to [reach out](https://petrstepanov.com/) with respect to any issues with the script (or open an Issue in corresponding GitHub repo). Find more information about compilation of CERN ROOT from source [here](https://root.cern/install/build_from_source/).

## Why use CMake?
Generally speaking one can build a ROOT standalone executable using standard GNU Makefile approach. I used to do this before and this process is summarized in [Chapter 6 of my dissertation](https://petrstepanov.com/static/petr-stepanov-dissertation-latest.pdf). For a reference, standard cross-platform (works with Linux and macOS) GNU makefile is located in this repository and named `Makefile-GNU`.

However, CMake approach comes with benefits. CMake can automatically generate cross-platform makefiles and detect external libraries. In particular, CMake includes IDE project generators feature. Once generated, a project workspace that can be instantly opened in IDE of your choice (Eclipse, Visual Studio,…). This allows such features like code autocompletion, hilighting and debugging.

## Trivial build

If your goal is to simply build a standalone CERN ROOT based executable, you can check out the project, place your code in `main.cpp` file and carry out a basic CMake out-of-source build:
```
mkdir -p ~/Development && cd ~/Development
git clone https://github.com/petrstepanov/root-eclipse
mkdir root-eclipse-build && cd root-eclipse-build
cmake ../root-eclipse
make 
sudo make install
```
Depending on your program code, extra ROOT libraries may need to be specified in `CMakeLists.txt` with CMake `list(APPEND LIB_NAMES "<root-library-name>")` command. List of available extra ROOT libraries [can be found here](https://cliutils.gitlab.io/modern-cmake/chapters/packages/ROOT.html#the-right-way-targets). 

Next in this article we will discuss a more important aspect. Namely, how to set up a CERN ROOT based project for extensive development in Eclipse IDE.

## Installing and Tweaking the Eclipse IDE

Refer to [original documantation](https://wiki.eclipse.org/Eclipse/Installation) to install Eclipse IDE. On Fedora linux it is a one-liner `sudo dnf -y install eclipse`. After installation is complete, perform following steps:
* Install CDT plugin. In menu Help > Install New Software... select "All Available Sites". Under "Programming Languages" select "C/C++ Development Tools". Restart Eclipse.
* Activate "C/C++" perspective in Window > Perspective > Open.
* Set Eclipse environment variables. In Window > Preferences > C/C++ > Environment specify the `LD_LIBRARY_PATH` variable for shared library include path. Take variable value from Terminal's `echo $LD_LIBRARY_PATH` output.
* Increase Eclipse initial and maximum heap size. Run following commands in Terminal, but double check that your `/etc/eclipse.ini` location first:
```
sudo cp /etc/eclipse.ini /etc/eclipse.ini.bak
sudo sed -i -r "s;Xms[0-9]*m;Xms1024m;" /etc/eclipse.ini
sudo sed -i -r "s;Xmx[0-9]*m;Xmx4096m;" /etc/eclipse.ini  
```
* Increase Eclipse indexer cache limits. In Window > Preferences > C/C++ > Indexer set the "Cache limits" to 50% and 4096MB.
* Prevent workspace refreshes. In Window > Preferences > General > Workspace. Uncheck "Refresh on access". Otherwise Eclipse may randomly start refreshing the workspace. For external (non-CDT managed) build tools Eclipse does not provide folder specific Refresh Policy settings. Therefore, on a workspace refresh event Eclipse will re-index all project source files including ROOT sources. This takes quite a few time and CPU cycles. We are trying to avoid it.

## Generating Eclipse Project

Check out the Git repository into the desired location on your computer. I usually keep most of the Git repositories in `~/Development` folder.
```
mkdir -p ~/Development && cd ~/Development
git clone https://github.com/petrstepanov/root-eclipse
```

Next we need to set up an Eclipse project. Thankfully, CMake has an automated way of generating the Eclipse project. The CMake generator command is `cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../project/path`, where `../project/path` path must be relative or absolute path of the project source folder that contains CMake cache file `CMakeLists.txt`. Generally speaking there are two options:
1. **In-source build**. Eclipse project folder files are located the source program folder next to the `CMakeLists.txt` file. It is not favorable because project files will interfere with the Git tree and multiple excludes in `.gitignore` will be required. Also I've experienced Eclipse indexer issued using this method.
2. **Out-of-source build**. Eclipse project is located outside of the Git repository. This option is preferrable because Eclipse project, build and executable files are separated from the original Git source tree. We will use this option.

The out-of-source project generator build is carried out in separate folder located outside of the actual project Git tree. For instance, we will create a `root-eclipse-project` folder. Generally speaking, the project denerator build build is initinitiated via following command:
```
mkdir root-eclipse-project && cd root-eclipse-project
cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../root-eclipse
```

More information regarding the CMake Eclipse generator can be found: [on Mantid project page](https://www.mantidproject.org/Setting_up_Eclipse_projects_with_CMake), [on Javier V. Gómez website](https://jvgomez.github.io/pages/how-to-configure-a-cc-project-with-eclipse-and-cmake.html), [on CMake Wiki](https://gitlab.kitware.com/cmake/community/-/wikis/home), [again on CMake Wiki](https://gitlab.kitware.com/cmake/community/-/wikis/doc/editors/Eclipse-CDT4-Generator) and [again on CMake Wiki](https://gitlab.kitware.com/cmake/community/-/wikis/doc/editors/Eclipse-UNIX-Tutorial).

## Attaching ROOT Sources to the Project

By default, CMake project generator creates Eclipse project and links ROOT includes (header files) to the project. However, **extensive debugging requires access to Geant and ROOT source files**. An effective way of linking ROOT framework source files to Eclipse project is following:
* Download and extract ROOT source packages to local computer. I usually keep all sources under `~/Source/` folder. Tip: store ROOT sources on the RAMDISK (hard drive in memory) to improve indexing performance on older hardware (explained later).
* Symlink all ROOT and Geant4 folders under the project's source folder. Tip: it is better if sources are arranged without nested directory structure. This greatly improves Eclipse indexing speed.

I wrote a [special Makefile function](https://stackoverflow.com/a/69463832) that performs above functuionality. In order to make it work, pass locations of the ROOT sources to `CMakeLists.txt` as CMake variable:

```
cd ~/Development
mkdir root-eclipse-project && cd root-eclipse-project
cmake -G"Eclipse CDT4 - Unix Makefiles" \
      -DCMAKE_BUILD_TYPE=Debug \
      -DROOT_SOURCE_LOCATION="$HOME/ramdisk-persistent/current/root*/" \
      -DCMAKE_ECLIPSE_GENERATE_LINKED_RESOURCES=OFF \
      ../root-eclipse
```
In the command above the ROOT library path need to be replaced with one on your file system. Here `-DCMAKE_ECLIPSE_GENERATE_LINKED_RESOURCES=OFF` [disables creation](https://gitlab.kitware.com/cmake/cmake/-/issues/19107) of the `[Subprojects]` folder for out-of source Cmake build. Presence of this folder duplicates all project source files and slows down the indexer.

Finaly open Eclipse and go to File > Open Projects from File System... Specify the project location in the modal dialog by clicking the "Directory..." button. Locate the `~/Development/root-eclipse-project` project folder. Click "Finish". 

Tip: Eclipse will automatically start indexing the project. Please kill this process because Eclipse will refresh the workspace and re-index all project again after the first launch of the Debug configuration.

## Setting up Eclipse Debug and Run Configurations

We will start from setting up the main Debug configuration for Geant4 `root-eclipse` program.

1. Expand 🎯 Build Targets under the main project node in the Project Explorer window. Double-click "All" to compile the executable.
2. In menu "Run" open "Debug configurations". Right click "C/C++ Application" and select "New Configuration".
3. Click "Browse" next to the "Project" field. Select "root-eclipse-Debug@eclipse"
4. Click "Search Project" next to the "C/C++ Application" field. Specify the "root-eclipse" execuable built earlier.
5. Select "Enable Auto Build"
6. Go to the "Debugger" tab. Uncheck "Stop on startup at:".

Finally we can run the project in Debug mode. In Eclipse menu select `Run → Debug`. Eclipse will run the project and simultaneously start indexing all ROOT source files. Depending on the speed of your hard drive and memory indexing will require from several minutes to about an hour. 

## RAMDISK for Older Computers 

On modern computers with NVMe hard drives Eclipse indexer will crawl ROOT sources fairly quick. However, indexing process can take about one hour on older computers SATA3 interface or magnetic disk drives (HDD). 

Luckily there is a solution. If you have an older computer with decent amount of RAM (8 Gb+) I recommend setting up RAMDISK - a virtual drive partition in computer's RAM. Indexing speed will increase 2-4 times if you store Eclipse workspace folder and ROOT sources [on RAMDISK](https://github.com/patrikx3/ramdisk). 

It is reasonable to disable system swap when using RAMDISK. This prevents the RAMDISK from being moved back onto the hard drive. Disable system swap with `sudo swapoff -a` after startup.

## [Rudimentary] Compiling and installing with GNU Make
This option is fairly outdated. I mostly used it for Root v5 and early v6 versions. However it gives a good understanding of building a stand-alone CERN ROOT project. Also this option is useful for running the code if user does not have root permisions on computer. For instance, if working on the remote computer.

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
