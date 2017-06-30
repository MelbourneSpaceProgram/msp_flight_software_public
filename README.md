# MSP CS-1 Software

All code that will run on-board the satellite. The project is written in C++ and C, using the Code Composer Studio (CCS) compiler, provided by Texas Instruments for the MSP432.

## Getting Started

The default branch for this project is `satellite`, not `master`. Please ensure the `satellite` branch is used as the source branch for all work, and that all work is eventually merged back here.

### Prerequisites


```
Code Composer Studio
Doxygen (only if documentation needs to be generated
```
When using CCS, be sure to change the Build Configuration to `TIRTOS Build`. The project will not build otherwise.

### Tests

The project currently includes a test runner and some basic verification tests. These need to be fleshed out further, and expanded to test new parts of the code. The test framework used is Unity (test framework, not the game engine).

### Automatic builds

We also have a Jenkins server running. This server builds a copy of the code and runs the automated tests outlined above whenever new code is pushed. The server also runs some code quality tools such as CppCheck. CppCheck provides insights into unused functions/variables, and other possible improvements.

## Contributing code

Please ensure all code is created under a separate branch to `satellite`, to make testing and review easier. Squash non-meaningful commits (if unsure, don't worry about it, will be caught in review). Ensure the code builds locally, and also on the Jenkins build server. All automated tests must pass. When ready, please submit a merge request from the new branch onto `satellite`. The code will then be reviewed and merged when ready.

## Useful Links

[Git repository](https://git.anthonykremor.com)

[Jenkins build server](https://ci.anthonykremor.com)

[Documentation](http://doxygen.anthonykremor.com) (Username/password: `msp`)

