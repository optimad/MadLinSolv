# MADLINSOLV installation

MadLinSolv runs on Linux and Mac OSX platforms.

## Dependencies
MadLinSolv depends on
* c++ compiler supporting `-std=c++11`. It has been tested with g++ >= 4.7.3
* cmake >= 2.8
* (optionally) MPI implementation. It has been tested with OpenMPI >= 1.6.5.
* [bitpit 1.6](https://github.com/optimad/bitpit)
* PETSc. It has been tested with PETSc >= 3.7.5.
* libxml2. This dependency will be removed in future, but bitpit depends on it.

## Confguring MadLinSolv
MadLinSolv uses ccmake as building tool.
In the MadLinSolv's root folder make a building folder, e.g. build
```bash
    bitpit$ mkdir build
```
Enter the `build` folder
```bash
    bitpit$ cd build
```
 In order to configure it, run:
```bash
    bitpit/build$ ccmake ../
```
 By this way, MadLinSolv can be configured for production and installation.
Setting some variable in ccmake interface you can customize a bit your configuration.

The `CMAKE_BUILD_TYPE` variable has to be used to set the type of build. The possible options are : `None`, the environment compiler flags are used; `Release`, using compiler optimization flag `-O2`; `Debug`, related to compiler flags `-O0 -fmessage-length=0`, `RelWithDebInfo`, that uses compilation flags `-O2 -g` and `MinSizeRel` to have the smallest binary size.

In addition the `ENABLE_PROFILING` variable can be set to `ON` in order to add profiling flag `-pg` during the compilation.

The `ENABLE_MPI` variable can be used to compile the parallel implementation of MadLinSolv and to allow the dependency on MPI libraries. Both bitpit and PETSc should be compiled in parellel mode.

Be sure that your PETSc installaton works and set `PETSC_ARCH` and `PETSC_DIR` according to the right PETSc installation.

Finally, you can choose the installation folder setting the cmake variable `CMAKE_INSTALL_PREFIX`. The default installation folder is `/usr/local/`.

Remember that if you choose the default installation path or another path without write permission you will need administration privileges to install MadLinSolv in.

## Building and Installing
Once cmake has configured,just do
```bash
    bitpit/build$ make   
```
to build and
```bash
    bitpit/build$ make install   
```
to install.

If you have just built MadLinSolv, the executable will be available at `build/src` folder.

If you have also installed MadLinSolv, the executable will be available at `/my/installation/folder/bin` folder and possibly the documentation will be available at `/my/installation/folder/doc`

## Building Documentation
In order to build properly the documentation Doxygen (>=1.8.6) and Graphviz (>=2.20.2) are needed.

In the ccmake interface the variable `BUILD_DOCUMENTATION` can be set to `ON` in order to build the documentation during the application compilation. 
If turned on the new variable `DOC_EXTRACT_PRIVATE` can be used to include all the private class members in the documentation.
  
After the `make` or `make install` the doxygen documentation will be built. You can chose to compile only the documentation with command 
```bash
    bitpit/build$ make doc   
```
You can now browse the html documentation with your favorite browser by opening 'html/index.html'.

## Help
For any problem, please contact <a href="http://www.optimad.it">Optimad engineering srl</a> at info@optimad.it. 
