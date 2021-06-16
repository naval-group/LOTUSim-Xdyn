# xdyn

xdyn is a lightweight time-domain ship simulator modelling the dynamic behaviour of a ship at sea, with its actuators, including some non-linear aspects of that behaviour and featuring a customizable maneuvring model.
It simulates the mechanical behaviour of a solid body in a fluid environment by
solving Newton's second law of motion, taking hydrodynamic forces into account.

It was developed by SIREHNA through both self-funded projects and various collaborative projects, including the IRT Jules Verne's ["Bassin Numérique"
project](https://www.irt-jules-verne.fr/wp-content/uploads/bassin-numerique.pdf).

(c) 2014-2015, [IRT Jules Verne](https://www.irt-jules-verne.fr/), [SIREHNA](http://www.sirehna.com/), [Naval Group](https://www.naval-group.com/en/), [Bureau Veritas](https://www.bureauveritas.fr/), [Hydrocean](https://marine-offshore.bureauveritas.com/bvsolutions), [STX France](http://chantiers-atlantique.com/en/), [LHEEA](https://lheea.ec-nantes.fr/) for the initial version.

(c) 2015-2020 [SIREHNA](http://www.sirehna.com/) & [Naval Group](https://www.naval-group.com/en/) for all subsequent versions.

**Disclaimer**: [External documentation](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/tree/master/doc_user) and [internal documentation](https://sirehna.gitlab-pages.sirehna.com/xdyn/)
was written for a French project, with
French participants, therefore it is in French. It will be translated
eventually. Also, please note that it is still a work-in-progress and, as such,
can be incomplete. Despite our best efforts, inaccuracies may remain. However,
the documentation will continue to evolve as new developments on xdyn are
on-going.

## Getting Started

The easiest way to run xdyn is to use [Docker](https://www.docker.com/):

```bash
docker run sirehna/xdyn
```

This does not require installing or downloading anything except Docker itself.

Pre-built binaries of xdyn are also available:

- [for Debian 10](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/jobs/artifacts/master/download?job=build%3Adebian)
- [for Windows](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/jobs/artifacts/master/download?job=build%3Awindows)

There are many other ways of using xdyn, all of which are described
in [the documentation](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/jobs/artifacts/master/download?job=doc).

The **environment models** implemented inside xdyn are described in detail [here](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/modeles_environnementaux.md)

The **force models** implemented inside xdyn are described in detail [here](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/modeles_efforts_commandes_et_non_commandes.md)

You can also learn how to use xdyn using the tutorials:

- [Falling ball](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/tutorial_01.md)
- [Hydrostatic](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/tutorial_02.md)
- [Waves](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/tutorial_03.md)
- [Propulsion](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/tutorial_06.md)
- [gRPC wave model](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/tutorial_09.md)
- [gRPC force model](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/tutorial_10.md)
- [gRPC controller](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/tutorial_11.md)

## Building xdyn from source

This section describes what you need to do if you wish to compile xdyn
yourself.
These instructions will get you a copy of the project up and running on your
local machine for development and testing purposes. See deployment for notes on
how to deploy the project on a live system.

### Prerequisites

To build xdyn, the easiest is to use [Docker](https://www.docker.com/).

Depending on your platform, please refer to:

- [Get Docker CE for CentOS](https://docs.docker.com/install/linux/docker-ce/centos/)
- [Get Docker CE for Debian](https://docs.docker.com/install/linux/docker-ce/debian/)
- [Get Docker CE for Fedora](https://docs.docker.com/install/linux/docker-ce/fedora/)
- [Get Docker CE for Mac](https://docs.docker.com/docker-for-mac/install/)
- [Install Docker for Windows](https://docs.docker.com/docker-for-windows/install/)

You also need [make](https://en.wikipedia.org/wiki/Make_(software)).

### Installing

Once Docker is installed, use:

```bash
make
```

to build both the Windows & the Debian versions.

If you only want one of the two versions, you can use:

```bash
make debian
```

or

```bash
make windows
```

which performs a cross-compilation for Windows using GCC.

The binaries can then be found in `build_windows` or `build_debian`,
respectively.

If you wish to build only part of the project, use:

```bash
./ninja_windows.sh package
```

for example, to build the Windows package (ZIP file containing the xdyn executable)

or

```bash
./ninja_debian.sh run_all_tests
```

to rebuild the Debian tests.

Once the build has finished, you can run the tests.

## Running the tests

To run the tests for Debian use:

```bash
./run_all_tests_debian.sh
```

To run the Windows tests (using Wine in a Docker container) enter:

```bash
./run_all_tests_windows.sh
```

The tests are written using Google test. These are both end-to-end tests and
unit tests. The end-to-end tests can be a bit long so you can disable them
using Google tests filters:

```bash
./run_all_tests_debian.sh --gtest_filter=-'*LONG*'
```

All arguments after the script name are passed to the GTest executable. Please
refer to [the Google Test documentation for details and other available
options](https://github.com/google/googletest/blob/master/googletest/docs/advanced.md#running-a-subset-of-the-tests).

## Running xdyn

### Running xdyn on Windows

Compile xdyn (`make windows`), install the xdyn executable, then run:

```bash
./xdyn <yaml file> [xdyn options]
```

All options can be found in [the documentation](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/interfaces_utilisateur.m4.m).

For example, to run the first [tutorial](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/tutorial_01.md),
from the executables/demos folder, you can run:

```bash
./xdyn tutorial_01_falling_ball.yml --dt 0.1 --tend 1
```

### Running xdyn on Debian

Compile xdyn (`make debian`) and install the xdyn executable
(`sudo dpkg -i xdyn.deb`).
You'll also need to install libgfortran3 (for debian9) or
libgfortran5 (for debian10).

You can then run:

```bash
xdyn <yaml file> [xdyn options]
```

All options can be found in [the documentation](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/interfaces_utilisateur.m4.m).

For example, to run the first [tutorial](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/tutorial_01.md),

```bash
xdyn tutorial_01_falling_ball.yml --dt 0.1 --tend 1
```

### Running xdyn on Debian with Docker

To create a Docker image containing xdyn, run:

```bash
make debian
make docker
```

To run the xdyn Docker container, use:

```bash
docker run -it --rm -u $(id -u):$(id -g) -v $(pwd):/build -w /build/path_to_yaml_file xdyn <yaml file> [xdyn options]
```

- Flag `--rm` deletes the container (not the image) after exit
- Flag `-u $(id -u):$(id -g)` launches the container with the permissions
of the current user, which ensures the files generated by xdyn (if any) are
owned by the current user
- Flag `-v $(pwd):/build` mounts the current directory to the container's
`/build` directory
- Flag `-w /build/path_to_yaml_file` sets the container's working directory and
`path_to_yaml_file` should be replaced by a sub-path of the current directory

More details can be found in
[Docker's official documentation](https://docs.docker.com/engine/reference/commandline/run/).

For example, to run the first [tutorial](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/doc_user/tutorial_01.md) and display the results in the terminal, assuming we are in the project's root directory:

```bash
docker run -it --rm -w /usr/demos sirehna/xdyn tutorial_01_falling_ball.yml --dt 0.1 --tend 1 -o tsv
```

## Debugging

### Valgrind

The memory analyzer [Valgrind](https://valgrind.org/) can be used during
development to check for memory leaks and use of uninitialized values.
To use it, first build a debug version (if you haven't already):

```bash
./ninja_debug.sh run_all_tests
```

then run:

```bash
./run_all_tests_valgrind.sh
```

This script accepts any [flag `run_all_tests` does](https://gitlab.inria.fr/Phylophile/Treerecs/blob/f6551e06797b52819ba3e630b92315254a944da5/tests/gtest/googletest/docs/AdvancedGuide.md#running-test-programs-advanced-options), in particular filtering:

```bash
./run_all_tests_valgrind.sh --gtest_filter='Gravity*'
```

### GDB

A script called `gdb.sh` can be used during development to launch `xdyn` or the
unit tests under [GDB](https://www.gnu.org/software/gdb/).

To debug xdyn, first build a debug version (if you haven't already):

```bash
./ninja_debug.sh xdyn
```

Then run:

```bash
./gdb.sh /build/executables/xdyn
```

This will open a GDB prompt. To close it, press Ctrl+D. For more details on how
to use GDB, refer to [the official GDB
documentation](https://www.gnu.org/software/gdb/).

To launch the debugger on the unit tests, first build a debug version (if you
haven't already):

```bash
./ninja_debug.sh run_all_tests
```

Then run:

```bash
./gdb.sh run_all_tests
```


## Deployment

Add additional notes about how to deploy this on a live system.

## Built with

* [CMake](https://cmake.org/) - Used to compile C++ code for various platforms.
* [Make](https://www.gnu.org/software/make/) - Used for the one-step build described above.
* [GCC](http://gcc.gnu.org/) - Compiler used for both the Windows & Linux: Visual Studio is currently **not** supported.
* [Boost](https://www.boost.org/) - For command-line options, regular expressions, filesystem library.
* [yaml-cpp](https://github.com/jbeder/yaml-cpp) - To parse the input files.
* [HDF5](https://support.hdfgroup.org/products/hdf5_tools/index.html) - To store the outputs.
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) - For matrix manipulations.
* [Pandoc](https://pandoc.org/) - To generate the documentation.
* [Pweave](http://mpastell.com/pweave/) - To generate the tutorials.
* [SSC](https://gitlab.com/sirehna_naval_group/sirehna/ssc) - For websockets, units decoding, interpolations, kinematics, CSV file reading and exception handling.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on how to submit
issues & pull requests to xdyn.
Our code of conduct is the [Contributor Covenant](CODE_OF_CONDUCT.md) (original
version available
[here](https://www.contributor-covenant.org/version/1/4/code-of-conduct) ).

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/tags).

## Authors

The main contributors to this projects are:

* [Charles-Edouard CADY](https://gitlab.com/CharlesEdouardCady_SIREHNA)
* [Guillaume JACQUENOT](https://gitlab.com/GuillaumeJacquenot)
* [Léa LINCKER](https://gitlab.com/llincker)
* [Moran CHARLOU](https://gitlab.com/mcharlou)


See also the [full list of contributors](https://gitlab.com/sirehna_naval_group/sirehna/xdyn/-/blob/master/contributors) who took part in this project.

## License

This project is licensed under the Eclipse Public License (version 2) - see the [LICENSE.md](LICENSE.md) file for details.
