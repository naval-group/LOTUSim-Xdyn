.PHONY: all_docker_images cmake-debian debian docker cmake-windows windows doc update-submodules

all: update-submodules generate_proto windows debian debug doc all_docker_images

DOCKER_AS_ROOT:=docker run -t --rm -w /opt/share -v $(shell pwd):/opt/share
DOCKER_AS_USER:=$(DOCKER_AS_ROOT) -u $(shell id -u):$(shell id -g)

HEADERS=code/ssc/ssc/check_ssc_version.hpp\
        code/ssc/ssc/csv_file_reader.hpp\
        code/ssc/ssc/data_source.hpp\
        code/ssc/ssc/decode_unit.hpp\
        code/ssc/ssc/exception_handling.hpp\
        code/ssc/ssc/geometry.hpp\
        code/ssc/ssc/integrate.hpp\
        code/ssc/ssc/interpolation.hpp\
        code/ssc/ssc/ipopt_interface.hpp\
        code/ssc/ssc/json.hpp\
        code/ssc/ssc/kinematics.hpp\
        code/ssc/ssc/macros.hpp\
        code/ssc/ssc/numeric.hpp\
        code/ssc/ssc/random_data_generator.hpp\
        code/ssc/ssc/solver.hpp\
        code/ssc/ssc/text_file_reader.hpp\
        code/ssc/ssc/websocket.hpp\
        code/ssc/ssc/yaml_parser.hpp

headers: ${HEADERS}
windows: ${HEADERS} windows_gccx_posix
debian11: ${HEADERS} debian_11_release_gcc_10
debian: ${HEADERS} debian_11_release_gcc_10
debian11_clang: ${HEADERS} debian_11_release_clang_14
debug: ${HEADERS} debian_11_debug_gcc_10
ubuntu1804-intel: ${HEADERS} ubuntu_18_04_release_intel_compiler

update-submodules:
	@echo "Updating Git submodules..."
	@git submodule sync --recursive
	@git submodule update --init --recursive
	@git submodule foreach --recursive 'git fetch --tags'

${HEADERS}:
	@git submodule sync --recursive
	@git submodule update --init --recursive
	@cd code/ssc/ssc && sh generate_module_header.sh && cd ../../..

generate_proto:
	make -C interfaces build

cmake-debian: BUILD_TYPE = Release
cmake-debian: BUILD_DIR = build_deb11
cmake-debian: CPACK_GENERATOR = DEB
cmake-debian: DOCKER_IMAGE = sirehna/base-image-debian11-gcc10:2021-12-12
cmake-debian: BOOST_ROOT = /opt/boost
cmake-debian: cmake-debian-target

cmake-windows: BUILD_TYPE=Release
cmake-windows: BUILD_DIR=build_win
cmake-windows: CPACK_GENERATOR=ZIP
cmake-windows: DOCKER_IMAGE=sirehna/base-image-win64-gcc540-win32threads-ssc-xdyn
cmake-windows: BOOST_ROOT=/usr/src/mxe/usr/x86_64-w64-mingw32.static
cmake-windows: HDF5_DIR=/opt/HDF5_1_8_20/cmake
cmake-windows: cmake-windows-target

ubuntu_18_04_release_intel_compiler: BUILD_TYPE = Release
ubuntu_18_04_release_intel_compiler: BUILD_DIR = build_ubuntu1804_intel
ubuntu_18_04_release_intel_compiler: CPACK_GENERATOR = DEB
ubuntu_18_04_release_intel_compiler: DOCKER_IMAGE = sirehna/base-image-ubuntu18-04-intel-oneapi-2022-02:2022-04-19
ubuntu_18_04_release_intel_compiler: BOOST_ROOT = /opt/boost
ubuntu_18_04_release_intel_compiler: HDF5_DIR = /usr/local/hdf5/share/cmake
ubuntu_18_04_release_intel_compiler: BUILD_PYTHON_WRAPPER = False
ubuntu_18_04_release_intel_compiler: cmake-ubuntu-intel-target build-ubuntu-intel test-ubuntu-intel

ubuntu_22_04_release_gcc_11: BUILD_TYPE = Release
ubuntu_22_04_release_gcc_11: BUILD_DIR = build_ubuntu2204
ubuntu_22_04_release_gcc_11: CPACK_GENERATOR = DEB
ubuntu_22_04_release_gcc_11: DOCKER_IMAGE = sirehna/base-image-ubuntu2204-gcc11:2022-05-23
ubuntu_22_04_release_gcc_11: BOOST_ROOT = /opt/boost
ubuntu_22_04_release_gcc_11: HDF5_DIR = /usr/local/hdf5/share/cmake
ubuntu_22_04_release_gcc_11: BUILD_PYTHON_WRAPPER = False
ubuntu_22_04_release_gcc_11: cmake-debian-target build-debian test-debian

debian_9_release_gcc_6: BUILD_TYPE = Release
debian_9_release_gcc_6: BUILD_DIR = build_deb9
debian_9_release_gcc_6: CPACK_GENERATOR = DEB
debian_9_release_gcc_6: DOCKER_IMAGE = sirehna/base-image-debian9-gcc6-xdyn
debian_9_release_gcc_6: BOOST_ROOT = /opt/boost
debian_9_release_gcc_6: BUILD_PYTHON_WRAPPER = False
debian_9_release_gcc_6: cmake-debian-target build-debian test-debian

debian_10_release_gcc_8: BUILD_TYPE = Release
debian_10_release_gcc_8: BUILD_DIR = build_deb10
debian_10_release_gcc_8: CPACK_GENERATOR = DEB
debian_10_release_gcc_8: DOCKER_IMAGE = sirehna/base-image-debian10-gcc8-xdyn:2021-04-16
debian_10_release_gcc_8: BOOST_ROOT = /opt/boost
debian_10_release_gcc_8: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_10_release_gcc_8: BUILD_PYTHON_WRAPPER = False
debian_10_release_gcc_8: cmake-debian-target build-debian test-debian

debian_10_coverage_gcc_8: BUILD_TYPE = Coverage
debian_10_coverage_gcc_8: BUILD_DIR = build_deb10
debian_10_coverage_gcc_8: CPACK_GENERATOR = DEB
debian_10_coverage_gcc_8: DOCKER_IMAGE = sirehna/base-image-debian10-gcc8-xdyn:2021-04-16
debian_10_coverage_gcc_8: BOOST_ROOT = /opt/boost
debian_10_coverage_gcc_8: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_10_coverage_gcc_8: BUILD_PYTHON_WRAPPER = False
debian_10_coverage_gcc_8: cmake-debian-target build-debian test-debian

debian_10_profile_gcc_8: BUILD_TYPE = Profile
debian_10_profile_gcc_8: BUILD_DIR = build_deb10_profile
debian_10_profile_gcc_8: CPACK_GENERATOR = DEB
debian_10_profile_gcc_8: DOCKER_IMAGE = sirehna/base-image-debian10-gcc8-xdyn:2021-04-16
debian_10_profile_gcc_8: BOOST_ROOT = /opt/boost
debian_10_profile_gcc_8: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_10_profile_gcc_8: BUILD_PYTHON_WRAPPER = False
debian_10_profile_gcc_8: cmake-debian-target build-debian test-debian

debian_10_debug_gcc_8: BUILD_TYPE = Debug
debian_10_debug_gcc_8: BUILD_DIR = build_deb10_dbg
debian_10_debug_gcc_8: CPACK_GENERATOR = DEB
debian_10_debug_gcc_8: DOCKER_IMAGE = sirehna/base-image-debian10-gcc8-xdyn:2021-04-16
debian_10_debug_gcc_8: BOOST_ROOT = /opt/boost
debian_10_debug_gcc_8: BUILD_PYTHON_WRAPPER = False
debian_10_debug_gcc_8: cmake-debian-target build-debian

debian_11_debug_gcc_10: BUILD_TYPE = Debug
debian_11_debug_gcc_10: BUILD_DIR = build_deb11_dbg
debian_11_debug_gcc_10: CPACK_GENERATOR = DEB
debian_11_debug_gcc_10: DOCKER_IMAGE = sirehna/base-image-debian11-gcc10:2021-12-12
debian_11_debug_gcc_10: BOOST_ROOT = /opt/boost
debian_11_debug_gcc_10: BUILD_PYTHON_WRAPPER = False
debian_11_debug_gcc_10: cmake-debian-target build-debian

debian_11_profile_gcc_10: BUILD_TYPE = Profile
debian_11_profile_gcc_10: BUILD_DIR = build_deb11_profile
debian_11_profile_gcc_10: CPACK_GENERATOR = DEB
debian_11_profile_gcc_10: DOCKER_IMAGE = sirehna/base-image-debian11-gcc10:2021-12-12
debian_11_profile_gcc_10: BOOST_ROOT = /opt/boost
debian_11_profile_gcc_10: BUILD_PYTHON_WRAPPER = False
debian_11_profile_gcc_10: cmake-debian-target build-debian

debian_11_release_gcc_10: BUILD_TYPE = Release
debian_11_release_gcc_10: BUILD_DIR = build_deb11
debian_11_release_gcc_10: CPACK_GENERATOR = DEB
debian_11_release_gcc_10: DOCKER_IMAGE = sirehna/base-image-debian11-gcc10:2021-12-12
debian_11_release_gcc_10: BOOST_ROOT = /opt/boost
debian_11_release_gcc_10: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_11_release_gcc_10: BUILD_PYTHON_WRAPPER = False
debian_11_release_gcc_10: cmake-debian-target build-debian test-debian

debian_11_release_gcc_10_wrapper: BUILD_TYPE = Release
debian_11_release_gcc_10_wrapper: BUILD_DIR = build_deb11_pywrapper
debian_11_release_gcc_10_wrapper: CPACK_GENERATOR = DEB
debian_11_release_gcc_10_wrapper: DOCKER_IMAGE = xdyn-python-deb11-py309-build
debian_11_release_gcc_10_wrapper: BOOST_ROOT = /opt/boost
debian_11_release_gcc_10_wrapper: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_11_release_gcc_10_wrapper: BUILD_PYTHON_WRAPPER = True
debian_11_release_gcc_10_wrapper: PYTHON_VERSION=3.9
debian_11_release_gcc_10_wrapper: PYTHON_TEST_TARGET=debian_11_py309_package
debian_11_release_gcc_10_wrapper: build-docker-python-image cmake-debian-target build-debian test-debian package-debian-python-with-doc

debian_11_release_gcc_10_wrapper_py307: BUILD_TYPE = Release
debian_11_release_gcc_10_wrapper_py307: BUILD_DIR = build_deb11_pywrapper
debian_11_release_gcc_10_wrapper_py307: CPACK_GENERATOR = DEB
debian_11_release_gcc_10_wrapper_py307: DOCKER_IMAGE = xdyn-python-deb11-py307-build
debian_11_release_gcc_10_wrapper_py307: BOOST_ROOT = /opt/boost
debian_11_release_gcc_10_wrapper_py307: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_11_release_gcc_10_wrapper_py307: BUILD_PYTHON_WRAPPER = True
debian_11_release_gcc_10_wrapper_py307: PYTHON_VERSION=3.7
debian_11_release_gcc_10_wrapper_py307: PYTHON_TEST_TARGET=debian_11_py307_package
debian_11_release_gcc_10_wrapper_py307: ADDITIONAL_CMAKE_PARAMETERS = "-DPython_EXECUTABLE=/usr/local/bin/python3"
debian_11_release_gcc_10_wrapper_py307: build-docker-python-image cmake-debian-target build-debian test-debian package-debian-python

debian_11_release_gcc_10_wrapper_py308: BUILD_TYPE = Release
debian_11_release_gcc_10_wrapper_py308: BUILD_DIR = build_deb11_pywrapper
debian_11_release_gcc_10_wrapper_py308: CPACK_GENERATOR = DEB
debian_11_release_gcc_10_wrapper_py308: DOCKER_IMAGE = xdyn-python-deb11-py308-build
debian_11_release_gcc_10_wrapper_py308: BOOST_ROOT = /opt/boost
debian_11_release_gcc_10_wrapper_py308: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_11_release_gcc_10_wrapper_py308: BUILD_PYTHON_WRAPPER = True
debian_11_release_gcc_10_wrapper_py308: PYTHON_VERSION=3.8
debian_11_release_gcc_10_wrapper_py308: PYTHON_TEST_TARGET=debian_11_py308_package
debian_11_release_gcc_10_wrapper_py308: ADDITIONAL_CMAKE_PARAMETERS = "-DPython_EXECUTABLE=/usr/local/bin/python3"
debian_11_release_gcc_10_wrapper_py308: build-docker-python-image cmake-debian-target build-debian test-debian package-debian-python

debian_11_release_gcc_10_wrapper_py310: BUILD_TYPE = Release
debian_11_release_gcc_10_wrapper_py310: BUILD_DIR = build_deb11_pywrapper
debian_11_release_gcc_10_wrapper_py310: CPACK_GENERATOR = DEB
debian_11_release_gcc_10_wrapper_py310: DOCKER_IMAGE = xdyn-python-deb11-py310-build
debian_11_release_gcc_10_wrapper_py310: BOOST_ROOT = /opt/boost
debian_11_release_gcc_10_wrapper_py310: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_11_release_gcc_10_wrapper_py310: BUILD_PYTHON_WRAPPER = True
debian_11_release_gcc_10_wrapper_py310: PYTHON_VERSION=3.10
debian_11_release_gcc_10_wrapper_py310: PYTHON_TEST_TARGET=debian_11_py310_package
debian_11_release_gcc_10_wrapper_py310: ADDITIONAL_CMAKE_PARAMETERS = "-DPython_EXECUTABLE=/usr/local/bin/python3"
debian_11_release_gcc_10_wrapper_py310: build-docker-python-image cmake-debian-target build-debian test-debian package-debian-python

debian_11_release_gcc_10_wrapper_python_all:
	@echo "Create Python3 wheels for Python 3.7, 3.8, 3.9 and 3.10"
	@echo "The Python 3.08 wheel can be installed on Ubuntu 20.04"
	@echo "The Python 3.10 wheel can be installed on Ubuntu 22.04"
	@echo "A docker image based on Debian11, with xdyn, Python3, all proto files and the Python generated code is created"
	@rm -rf build_deb11_pywrapper/CMakeCache.txt build_deb11_pywrapper/xdyn_wrapper_python build_deb11_pywrapper/lib.linux-x86_64-3.7
	make debian_11_release_gcc_10_wrapper_py307
	@rm -rf build_deb11_pywrapper/CMakeCache.txt build_deb11_pywrapper/xdyn_wrapper_python build_deb11_pywrapper/lib.linux-x86_64-3.8
	make debian_11_release_gcc_10_wrapper_py308
	@rm -rf build_deb11_pywrapper/CMakeCache.txt build_deb11_pywrapper/xdyn_wrapper_python build_deb11_pywrapper/lib.linux-x86_64-3.9
	make debian_11_release_gcc_10_wrapper
	make -C code/xdyn_wrapper_python create_docker_image_debian11_py309
	@rm -rf build_deb11_pywrapper/CMakeCache.txt build_deb11_pywrapper/xdyn_wrapper_python build_deb11_pywrapper/lib.linux-x86_64-3.10
	make debian_11_release_gcc_10_wrapper_py310
	make -C code/xdyn_wrapper_python create_docker_image_ubuntu2204_py310
	ls code/xdyn_wrapper_python/*.whl
	make -C code/xdyn_wrapper_python test_additional_platforms_depending_on_debian
	@echo "Success for :"
	@echo " - Linux x86-64 Debian 11 with Python 3.7"
	@echo " - Linux x86-64 Debian 11 with Python 3.8"
	@echo " - Linux x86-64 Debian 11 with Python 3.9 (default Python version)"
	@echo " - Linux x86-64 Debian 11 with Python 3.10"
	@echo " - Linux x86-64 Ubuntu 20.04 with Python 3.8, using the Debian 11 generated wheel"

debian_11_release_clang_14: BUILD_TYPE = Release
debian_11_release_clang_14: BUILD_DIR = build_deb11_clang14
debian_11_release_clang_14: CPACK_GENERATOR = DEB
debian_11_release_clang_14: DOCKER_IMAGE = sirehna/base-image-debian11-clang14:2022-09-27
debian_11_release_clang_14: BOOST_ROOT = /opt/boost
debian_11_release_clang_14: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_11_release_clang_14: BUILD_PYTHON_WRAPPER = False
debian_11_release_clang_14: ADDITIONAL_CMAKE_PARAMETERS=-D CMAKE_CXX_COMPILER=clang++ -D CMAKE_C_COMPILER=clang -D BUILD_DOCUMENTATION=False
debian_11_release_clang_14: cmake-debian-target build-debian test-debian

debian_11_release_clang_14_docker:
	@make debian_11_release_clang_14
	@cp build_deb11_clang14/xdyn.deb .
	@echo "**" > .dockerignore
	@echo "!xdyn.deb" >> .dockerignore
	@docker build . --tag xdyn
	@rm -f .dockerignore

debian_11_release_clang_14_doxygen: BUILD_TYPE = Release
debian_11_release_clang_14_doxygen: BUILD_DIR = build_deb11_clang14
debian_11_release_clang_14_doxygen: CPACK_GENERATOR = DEB
debian_11_release_clang_14_doxygen: DOCKER_IMAGE = sirehna/base-image-debian11-clang14:2022-09-27
debian_11_release_clang_14_doxygen: BOOST_ROOT = /opt/boost
debian_11_release_clang_14_doxygen: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_11_release_clang_14_doxygen: BUILD_PYTHON_WRAPPER = False
debian_11_release_clang_14_doxygen: ADDITIONAL_CMAKE_PARAMETERS=-D CMAKE_CXX_COMPILER=clang++ -D CMAKE_C_COMPILER=clang -D BUILD_DOCUMENTATION=True
debian_11_release_clang_14_doxygen: cmake-debian-target doxygen-debian

build-docker-python-image:
	make -C code/xdyn_wrapper_python ${DOCKER_IMAGE}

package-debian-python:
	@mkdir -p code/xdyn_wrapper_python/build
	cp -rf ${BUILD_DIR}/lib.linux-x86_64-${PYTHON_VERSION} code/xdyn_wrapper_python/build/.
	make -C code/xdyn_wrapper_python ${PYTHON_TEST_TARGET}

package-debian-python-with-doc:
	@echo "Create the xdyn wheel file and generate the HTML sphinx associated documenation"
	@mkdir -p code/xdyn_wrapper_python/build
	cp -rf ${BUILD_DIR}/lib.linux-x86_64-${PYTHON_VERSION} code/xdyn_wrapper_python/build/.
	make -C code/xdyn_wrapper_python ${PYTHON_TEST_TARGET}
	make -C code/xdyn_wrapper_python doc
	@echo "xdyn python documentation is located in code/xdyn_wrapper_python/doc/src/build/html"

package-debian-python-specific-deb10-gcc8:
	@mkdir -p code/xdyn_wrapper_python/build/lib.linux-x86_64-${PYTHON_VERSION}
	cp -rf ${BUILD_DIR}/lib.linux-x86_64-${PYTHON_VERSION}/xdyn.so \
		code/xdyn_wrapper_python/build/lib.linux-x86_64-${PYTHON_VERSION}/xdyn.cpython-37m-x86_64-linux-gnu.so
	make -C code/xdyn_wrapper_python ${PYTHON_TEST_TARGET}

windows_gccx_posix: BUILD_TYPE=Release
windows_gccx_posix: BUILD_DIR=build_win_posix
windows_gccx_posix: CPACK_GENERATOR=ZIP
windows_gccx_posix: DOCKER_IMAGE=sirehna/base-image-win64-gccx-posixthreads-ssc-xdyn
windows_gccx_posix: BOOST_ROOT=/usr/src/mxe/usr/x86_64-w64-mingw32.static.posix
windows_gccx_posix: HDF5_DIR=/opt/HDF5_1_8_20/cmake
windows_gccx_posix: cmake-windows-target build-windows test-windows


code/yaml-cpp/CMakeLists.txt: yaml-cpp-CMakeLists.txt
	$(DOCKER_AS_USER) $(DOCKER_IMAGE) /bin/bash -c \
	    "rm -rf /opt/share/code/yaml-cpp && \
	     cp -rf /opt/yaml_cpp /opt/share/code/yaml-cpp && \
	     cp /opt/share/yaml-cpp-CMakeLists.txt /opt/share/code/yaml-cpp/CMakeLists.txt"


cmake-windows-target: code/yaml-cpp/CMakeLists.txt
	docker pull $(DOCKER_IMAGE) || true
	$(DOCKER_AS_USER) $(DOCKER_IMAGE) /bin/bash -c \
	   "cd /opt/share &&\
	    mkdir -p $(BUILD_DIR) &&\
	    cd $(BUILD_DIR) &&\
	    mkdir -p /opt/share/.wine;\
	    export WINEPREFIX=/opt/share/.wine;\
	    wine winecfg;\
	    cmake -Wno-dev\
	        -G Ninja \
	        -D THIRD_PARTY_DIRECTORY=/opt \
	        -D CPACK_GENERATOR=$(CPACK_GENERATOR) \
	        -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) \
	        -D CMAKE_INSTALL_PREFIX:PATH=/opt/xdyn \
	        -D HDF5_DIR=$(HDF5_DIR) \
	        -D Boost_DEBUG=0 \
	        -D BOOST_ROOT:PATH=$(BOOST_ROOT) \
	        -D BOOST_INCLUDEDIR:PATH=$(BOOST_ROOT)/include \
	        -D Boost_INCLUDE_DIR:PATH=$(BOOST_ROOT)/include \
	        -D BOOST_LIBRARYDIR:PATH=$(BOOST_ROOT)/lib \
	        -D Boost_NO_SYSTEM_PATHS:BOOL=OFF \
	        -D Boost_LIBRARY_DIR_RELEASE:PATH=$(BOOST_ROOT)/lib \
	        -D Boost_PROGRAM_OPTIONS_LIBRARY:PATH=$(BOOST_ROOT)/lib/libboost_program_options-mt.a \
	        -D Boost_FILESYSTEM_LIBRARY:PATH=$(BOOST_ROOT)/lib/libboost_filesystem-mt.a \
	        -D Boost_SYSTEM_LIBRARY:PATH=$(BOOST_ROOT)/lib/libboost_system-mt.a \
	        -D Boost_REGEX_LIBRARY:PATH=$(BOOST_ROOT)/lib/libboost_regex-mt.a \
	        -D CMAKE_SYSTEM_VERSION=7 \
	        -D BUILD_PYTHON_WRAPPER:BOOL=$(BUILD_PYTHON_WRAPPER) \
	        /opt/share/code"

build-windows:
	$(DOCKER_AS_USER) $(DOCKER_IMAGE) /bin/bash -c \
	   "cd /opt/share &&\
	    mkdir -p $(BUILD_DIR) &&\
	    cd $(BUILD_DIR) &&\
	    mkdir -p /opt/share/.wine;\
	    export WINEPREFIX=/opt/share/.wine;\
	    wine winecfg;\
	    ninja $(NB_OF_PARALLEL_BUILDS) package"

test-windows:
	$(DOCKER_AS_USER) $(DOCKER_IMAGE) /bin/bash -c \
	   "cd $(BUILD_DIR) &&\
	    mkdir -p /opt/share/.wine;\
	    export WINEPREFIX=/opt/share/.wine;\
	    wine winecfg;\
	    wine ./run_all_tests --gtest_filter=-*ocket*:*ot_throw_if_CSV_file_exists"


cmake-debian-target: SHELL:=/bin/bash
cmake-debian-target: code/yaml-cpp/CMakeLists.txt
	docker pull $(DOCKER_IMAGE) || true
	$(DOCKER_AS_USER) $(DOCKER_IMAGE) /bin/bash -c \
	   "cd /opt/share &&\
	    mkdir -p $(BUILD_DIR) &&\
	    cd $(BUILD_DIR) &&\
	    cmake -Wno-dev \
	     -G Ninja \
	     -D THIRD_PARTY_DIRECTORY=/opt/ \
	     -D CPACK_GENERATOR=$(CPACK_GENERATOR) \
	     -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) \
	     -D CMAKE_INSTALL_PREFIX:PATH=/opt/xdyn \
	     -D HDF5_DIR=$(HDF5_DIR) \
	     -D BOOST_ROOT:PATH=$(BOOST_ROOT) \
	     -D BUILD_PYTHON_WRAPPER:BOOL=$(BUILD_PYTHON_WRAPPER) \
	     $(ADDITIONAL_CMAKE_PARAMETERS) \
	    /opt/share/code"

build-debian: SHELL:=/bin/bash
build-debian:
	$(DOCKER_AS_USER) $(DOCKER_IMAGE) /bin/bash -c \
	   "cd /opt/share && \
	    mkdir -p $(BUILD_DIR) && \
	    cd $(BUILD_DIR) && \
	    ninja $(NB_OF_PARALLEL_BUILDS) package"

doxygen-debian: SHELL:=/bin/bash
doxygen-debian:
	$(DOCKER_AS_USER) $(DOCKER_IMAGE) /bin/bash -c \
	   "cd /opt/share && \
	    mkdir -p $(BUILD_DIR) && \
	    cd $(BUILD_DIR) && \
	    ninja $(NB_OF_PARALLEL_BUILDS) doc_dev_fr"

test-debian: SHELL:=/bin/bash
test-debian:
	$(DOCKER_AS_USER) $(DOCKER_IMAGE) /bin/bash -c \
	   "cp validation/codecov_bash.sh $(BUILD_DIR) && \
	    cd $(BUILD_DIR) &&\
	    ./run_all_tests &&\
	    if [[ $(BUILD_TYPE) == Coverage ]];\
	    then\
	    echo Coverage;\
	    gprof run_all_tests gmon.out > gprof_res.txt 2> gprof_res.err;\
	    bash codecov_bash.sh && \
	    rm codecov_bash.sh;\
	    fi && \
	    ldd run_all_tests"

cmake-ubuntu-intel-target: code/yaml-cpp/CMakeLists.txt
	docker pull $(DOCKER_IMAGE) || true
	$(DOCKER_AS_USER) $(DOCKER_IMAGE) /bin/bash -c \
	   "source /opt/intel/oneapi/setvars.sh && \
	    cd /opt/share &&\
	    mkdir -p $(BUILD_DIR) &&\
	    cd $(BUILD_DIR) &&\
	    cmake -Wno-dev \
	     -G Ninja \
	     -D CMAKE_C_COMPILER=icc \
	     -D CMAKE_CXX_COMPILER=icpc \
	     -D THIRD_PARTY_DIRECTORY=/opt/ \
	     -D CPACK_GENERATOR=$(CPACK_GENERATOR) \
	     -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) \
	     -D CMAKE_INSTALL_PREFIX:PATH=/opt/xdyn \
	     -D HDF5_DIR=$(HDF5_DIR) \
	     -D BOOST_ROOT:PATH=$(BOOST_ROOT) \
	     -D BUILD_PYTHON_WRAPPER:BOOL=$(BUILD_PYTHON_WRAPPER) \
	     $(ADDITIONAL_CMAKE_PARAMETERS) \
	    /opt/share/code"

build-ubuntu-intel:
	$(DOCKER_AS_USER) $(DOCKER_IMAGE) /bin/bash -c \
	   "source /opt/intel/oneapi/setvars.sh && \
	    cd /opt/share && \
	    mkdir -p $(BUILD_DIR) && \
	    cd $(BUILD_DIR) && \
	    ninja $(NB_OF_PARALLEL_BUILDS) package"

test-ubuntu-intel:
	$(DOCKER_AS_USER) $(DOCKER_IMAGE) /bin/bash -c \
	   "source /opt/intel/oneapi/setvars.sh && \
	    cd $(BUILD_DIR) &&\
	    ./run_all_tests"

docker-ci: xdyn.deb
	@docker build . --tag xdyn

docker: BUILD_TYPE = Release
docker: BUILD_DIR = build_deb11
docker: CPACK_GENERATOR = DEB
docker: DOCKER_IMAGE = sirehna/base-image-debian11-gcc10:2021-12-12
docker: BOOST_ROOT = /opt/boost
docker: HDF5_DIR = /usr/local/hdf5/share/cmake
docker: cmake-debian-target build-debian
	@cp build_deb11/xdyn.deb .
	@echo "**" > .dockerignore
	@echo "!xdyn.deb" >> .dockerignore
	@docker build . --tag xdyn
	@rm -f .dockerignore

xdyn.deb: build_deb11/xdyn.deb
	@cp $< $@

build_deb11/xdyn.deb:
	@echo "Run ./ninja_debian.sh package"

docker_xdyngrpc:
	make -C interfaces build

all_docker_images: docker-ci docker_xdyngrpc
	@echo "Built all docker images after having run 'make debian'"

doc: BUILD_TYPE = Release
doc: BUILD_DIR = build_deb11
doc: CPACK_GENERATOR = DEB
doc: DOCKER_IMAGE = sirehna/base-image-debian11-gcc10:2021-12-12
doc: BOOST_ROOT = /opt/boost
doc: build-debian
	make -C doc
	mv doc/user_fr/doc.html doc_fr.html
	mv doc/training/remote_models.pptx doc_training_remote_models.pptx

clean:
	@rm -rf build_*
	@rm -rf yaml-cpp
	@make -C doc clean
	@make -C code/xdyn_wrapper_python clean

GREP=$(DOCKER_AS_USER) --entrypoint /bin/grep bitnami/minideb@sha256:25c5d5677df967c85b1bc3a33505cb43225dd92de06fe35eda3042348995835f

lint:
	@cd code && \
	if $(GREP) --recursive --include={*.cpp,*.c,*.hpp,*.h,*.md,*.yml,*.cmake.*.xml,*.html,*.in,*.txt} \
	        --exclude-dir={eigen,eigen3-hdf5,gcovr,gtest,gmock,google-test,yaml-cpp,ssc,interfaces} -P "\t" . ; \
	then echo "Tabs found in the lines shown above."; false; \
	else echo "Repo passed no-tabs check."; fi && \
	cd ..

changelog: CHANGELOG.md
changelog: ## Generates CHANGELOG.md from git merge commits
CHANGELOG.md:
	@make -C changelog
	@cp changelog/$@ .
.PHONY: changelog CHANGELOG.md
