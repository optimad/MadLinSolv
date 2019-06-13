MadLinSolv is a small application based on [bitpit](https://github.com/optimad/bitpit) and aimed at reading matrix and right-hand side and solving the linear system by using PETSc. Providing an user-edited XML dictionary, the user can control the application. 

At the moment, no other PETSc options but the debug ones can be passed to PETSc. Future develpements will expose to the user a way to use command line options of PETSc, customizing the linear solver, the preconditioner and the parameters involved in these two PETSc entities.

Please, see [INSTALL.md](INSTALL.md) for build and install instructions. Installation is optional.

Please, see the Doxygen documentation for the use of the XML dictionary user interface.

To use the application:

- (optionally) create a folder with your dictionary.xml file and move into it
- edit the dictionary.xml file according to your inputs ()
- from this folder just launch /path/to/madlinsolv/executable or mpirun -n # /path/to/madlinsolv/executable
- logger, matrix, right-hand side and solution files will be in this folder

Tests will come as soon as possible. At the moment, the data folder contains a very small example of matrix and right-hand side 
