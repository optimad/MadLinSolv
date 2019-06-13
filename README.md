MadLinSolv is a small application based on [bitpit](https://github.com/optimad/bitpit) and aimed at reading matrix and right-hand side and solving the linear system by using PETSc. Providing an user-edited XML dictionary, the user can control the application. 

At the moment, no other PETSc options but the debug ones can be passed to PETSc. Future develpements will expose to the user a way to use command line options of PETSc, customizing the linear solver, the preconditioner and the parameters involved in these two PETSc entities.
