# SUNDIALS Testing

SUNDIALS utilizes CTest to build and run the example programs distributed with
SUNDIALS as a test suite. In order to run the tests SUNDIALS examples must be
enabled by setting the CMake options `EXAMPLES_ENABLE_<language>` to `ON`.

To run the standard C tests use the following commands:
```
mkdir <build directory>
cd <build directory>
cmake <sundials directory> -DCMAKE_INSTALL_PREFIX=<prefix> -DEXAMPLES_ENABLE_C=ON
make
make test
```
This will create an out of source build, build SUNDIALS and all the C examples,
and run the tests (which are the examples) that return a pass/fail status. All
output is saved to the file `<build directory>/Testing/Temporary/LastTest.log`.

### Development Test

Examples that do not return a pass/fail status are considered "development" only
tests and are excluded by default when running `make test`. To enable all
examples when testing set the CMake option `SUNDIALS_DEVTESTS` to `ON`. With
this option enabled tests are run using a Python test runner `test/testRunner`.
The test runner determines if a test passes or fails by comparing the test
output against saved output files that are deemed correct by the SUNDIALS team.

To run the development C tests use the following commands:
```
mkdir <build directory>
cd <build directory>
cmake <sundials directory> -DCMAKE_INSTALL_PREFIX=<prefix> -DEXAMPLES_ENABLE_C=ON -DSUNDIALS_DEVTESTS=ON
make
make test
```
This will create an out of source build, build SUNDIALS and all the C examples,
and run all the tests (which are the examples). The output from each test is
saved to `<build directory>/Testing/output`.

## Testing Scripts

Several shell scripts are provided for setting up and running SUNDIALS tests and
are used with continuous integration tools to test each push and pull-request to
the SUNDIALS repository. The primary driver script is `test_driver.sh` which
selects which test type to run (BRANCH, PR, or RELEASE) and calls one of three
driver scripts corresponding to the test type:
* BRANCH tests (`test_branch.sh`) are run on each push to a branch in the
SUNDIALS repository.
* PR tests (`test_pr.sh`) are run for each pull-request issued to the SUNDIALS
repository.
* RELEASE tests (`test_release.sh`) are run for each push to a release branch or
release pull-request.

The BRANCH, PR, and RELEASE scripts call one or more of the following test
scripts to configure, build, and test SUNDIALS:
* `suntest.sh` -- tests configured using standard options
* `suntest_xsdk.sh` -- tests configured using xSDK options
* `suntest_tarscript.sh` -- create tarballs and test with standard options

Any of the test scripts may be called independently of the driver scripts for
manual testing. See the comment block at the top of each file for more
information on running the scripts.

Note: At this time the testing scripts only run development tests when SUNDIALS
is configured with real type double (either index size can be used).

### Testing environment

To setup the testing environment the test scripts will source one of the
following environment scripts (listed in the order checked):

1. The script specified by the environment variable `SUNDIALS_ENV`
2. A user's local environment script: `<sunrepo>/test/env.sh`
3. A user's global environment script: `~/.sundials_config/env.sh`
4. The default SUNDIALS environment script: `<sunrepo>/test/env.default.sh`

Environment scripts must set the following environment variables that are used
when configuring SUNDIALS for testing.
```
CC  = C compiler
CXX = C++ compiler
FC  = Fortran compiler

BASE_CFLAGS   = C compiler flags
BASE_CXXFLAGS = C++ compiler flags
BASE_FFLAGS   = Fortran compiler flags
```
Note the `BASE_` prefix is used as the compiler flag variables as the test
scripts will append the C standard flag (`-std=c90` or `-std=c99`) and C++
standard flag (`-std=c++11`) to the compiler flags provided by the environment
variables.

An environment script may optionally set additional environment variables to
enable or disable third party libraries (TPLs) in the SUNDIALS configuration.
Variables of the form `<TPL>STATUS` enable or disable the corresponding TPL when
set to `ON` or `OFF` respectively. Note `<TPL>STATUS` variables default to `OFF`
if they are not set. Depending on the particular TPL, a variable of the from
`<TPL>DIR` or `<TPL>LIBS` must be set when `<TPL>STATUS = ON` to provide the
full path to the the TPL installation directory or the list of TPL libraries
respectively. To aid in setting these variables appropriately, the test scripts
pass the real type (`single`, `double`, or `extended`) and the index size
(`32` or `64`) SUNDIALS will be configured with as inputs to the environment
script.

The currently supported TPL environment variables are as follows:
```
CUDASTATUS = ON or OFF

MPISTATUS = ON or OFF
MPICC     = MPI C compiler
MPICXX    = MPI C++ compiler
MPIFC     = MPI Fortran
MPIEXEC   = executable for launching MPI runs

BLAUSSTATUS = ON or OFF
BLASLIBS    = full path to BLAS library

LAPACKSTATUS = ON or OFF
LAPACKLIBS   = full path to LAPACK library

KLUSTATUS = ON or OFF
KLUDIR    = full path to KLU installation

SLUMTSTATUS = ON or OFF
SLUMTDIR    = full path to SuperLU_MT installation

SLUDISTSTATUS = ON or OFF
SLUDISTDIR    = full path to SuperLU_DIST installation

HYPRESTATUS = ON or OFF
HYPREDIR    = full path to hypre installation

PETSCSTATUS = ON or OFF
PETSCDIR    = full path to PETSc installation
```

## Using Spack to install TPLs

The TPLs needed for a complete build of SUNDIALS can be easily installed with
spack and the spack environment included in the SUNDIALS repository. Simply
navigate to `test/spack` and run `spack install`. For more information on Spack
environments is the [Spack tutorial](https://spack.readthedocs.io/en/latest/tutorial_environments.html).
