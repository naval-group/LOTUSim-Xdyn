# This Makefile:
# - builds the xdyn wheel from xdyn python shared library
# - tests the generated wheel in a fresh docker container

help:
	echo "Run higher target to build wheel! It can not be built directly from this Makefile"

debian_10_build_docker_build_container: DOCKER_IMAGE_BUILD=xdyn-python-deb10-build
debian_10_build_docker_build_container: DOCKER_IMAGE_TEST=xdyn-python-deb10-test
debian_10_build_docker_build_container: DOCKER_FILE_BUILD=Dockerfile_deb10_pybind
debian_10_build_docker_build_container: DOCKER_FILE_TEST=Dockerfile_deb10_test
debian_10_build_docker_build_container: build_docker_build_container

debian_11_build_docker_build_container: DOCKER_IMAGE_BUILD=xdyn-python-deb11-build
debian_11_build_docker_build_container: DOCKER_IMAGE_TEST=xdyn-python-deb11-test
debian_11_build_docker_build_container: DOCKER_FILE_BUILD=Dockerfile_deb11_py309_pybind
debian_11_build_docker_build_container: DOCKER_FILE_TEST=Dockerfile_deb11_py309_test
debian_11_build_docker_build_container: build_docker_build_container

debian_11_py308_build_docker_build_container: DOCKER_IMAGE_BUILD=xdyn-python-deb11-py308-build
debian_11_py308_build_docker_build_container: DOCKER_IMAGE_TEST=xdyn-python-deb11-py308-test
debian_11_py308_build_docker_build_container: DOCKER_FILE_BUILD=Dockerfile_deb11_py308_pybind
debian_11_py308_build_docker_build_container: DOCKER_FILE_TEST=Dockerfile_deb11_py308_test
debian_11_py308_build_docker_build_container: build_docker_build_container

debian_11_py310_build_docker_build_container: DOCKER_IMAGE_BUILD=xdyn-python-deb11-py310-build
debian_11_py310_build_docker_build_container: DOCKER_IMAGE_TEST=xdyn-python-deb11-py310-test
debian_11_py310_build_docker_build_container: DOCKER_FILE_BUILD=Dockerfile_deb11_py310_pybind
debian_11_py310_build_docker_build_container: DOCKER_FILE_TEST=Dockerfile_deb11_py310_test
debian_11_py310_build_docker_build_container: build_docker_build_container

debian_10_demo_package: DOCKER_IMAGE_BUILD=xdyn-python-deb10-build
debian_10_demo_package: DOCKER_IMAGE_TEST=xdyn-python-deb10-test
debian_10_demo_package: DOCKER_FILE_BUILD=Dockerfile_deb10_pybind
debian_10_demo_package: DOCKER_FILE_TEST=Dockerfile_deb10_test
debian_10_demo_package: demo_package

debian_11_demo_package: DOCKER_IMAGE_BUILD=xdyn-python-deb11-build
debian_11_demo_package: DOCKER_IMAGE_TEST=xdyn-python-deb11-test
debian_11_demo_package: DOCKER_FILE_BUILD=Dockerfile_deb11_py309_pybind
debian_11_demo_package: DOCKER_FILE_TEST=Dockerfile_deb11_py309_test
debian_11_demo_package: demo_package

debian_11_py307_demo_package: DOCKER_IMAGE_BUILD=xdyn-python-deb11-py307-build
debian_11_py307_demo_package: DOCKER_IMAGE_TEST=xdyn-python-deb11-py307-test
debian_11_py307_demo_package: DOCKER_FILE_BUILD=Dockerfile_deb11_py3yz_pybind
debian_11_py307_demo_package: DOCKER_FILE_TEST=Dockerfile_deb11_py3yz_test
debian_11_py307_demo_package: demo_package

debian_11_py308_demo_package: DOCKER_IMAGE_BUILD=xdyn-python-deb11-py308-build
debian_11_py308_demo_package: DOCKER_IMAGE_TEST=xdyn-python-deb11-py308-test
debian_11_py308_demo_package: DOCKER_FILE_BUILD=Dockerfile_deb11_py308_pybind
debian_11_py308_demo_package: DOCKER_FILE_TEST=Dockerfile_deb11_py308_test
debian_11_py308_demo_package: demo_package

debian_11_py310_demo_package: DOCKER_IMAGE_BUILD=xdyn-python-deb11-py310-build
debian_11_py310_demo_package: DOCKER_IMAGE_TEST=xdyn-python-deb11-py310-test
debian_11_py310_demo_package: DOCKER_FILE_BUILD=Dockerfile_deb11_py310_pybind
debian_11_py310_demo_package: DOCKER_FILE_TEST=Dockerfile_deb11_py310_test
debian_11_py310_demo_package: demo_package

DIR := $(notdir $(shell pwd))
PYTHON=python3
DOCKER_AS_ROOT:=docker run -t --rm -w /opt/share/${DIR} -v $(shell pwd)/..:/opt/share
DOCKER_AS_USER:=$(DOCKER_AS_ROOT) -u $(shell id -u):$(shell id -g)
DOCKER_RUN_BUILD_BASH=$(DOCKER_AS_USER) $(DOCKER_IMAGE_BUILD) /bin/bash -c
DOCKER_RUN_TEST_BASH=$(DOCKER_AS_USER) $(DOCKER_IMAGE_TEST) /bin/bash -c

build_docker_build_container:
	docker build . -f ${DOCKER_FILE_BUILD} -t ${DOCKER_IMAGE_BUILD}

demo_package: demo_package_build_wheel demo_package_test_build demo_package_test_run

demo_package_build_wheel:
	$(DOCKER_RUN_BUILD_BASH) \
	    "${PYTHON} setup.py bdist_wheel &&\
		mv dist/*.whl ."

demo_package_test_build:
	docker build . -f ${DOCKER_FILE_TEST} -t $(DOCKER_IMAGE_TEST)

demo_package_test_run:
	$(DOCKER_RUN_TEST_BASH) "${PYTHON} -m unit_tests"

demo_package_test_run_for_a_specific_file:
	$(DOCKER_RUN_TEST_BASH) \
		"cd unit_tests/force && ${PYTHON} test_maneuveringforcemodel.py"

clean:
	@rm -rf build
	@rm -rf core
	@rm -rf dist
	@rm -rf *.egg-info
	@rm -rf *.whl
	@rm -f *.coverage
	@rm -f nosetests.xml
	@rm -rf unit_tests/__pycache__
	@rm -f unit_tests/*.coverage
	@rm -f unit_tests/core
	@rm -f unit_tests/nosetests.xml
	@find . -type d -name __pycache__ -exec rm -rf {} \; || true


.PHONY=xdyn-python-deb10-build
xdyn-python-deb10-build: Dockerfile_deb10_pybind
	docker build . -f $^ -t $@

.PHONY=xdyn-python-deb11-build
xdyn-python-deb11-build: Dockerfile_deb11_py309_pybind
	docker build . -f $^ -t $@

.PHONY=xdyn-python-deb11-py307-build
xdyn-python-deb11-py307-build:
	docker build . \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=7 \
		--build-arg PYTHON_PATCH_VERSION=12 \
		-f Dockerfile_deb11_py3yz_pybind -t $@

.PHONY=xdyn-python-deb11-py307-test
xdyn-python-deb11-py307-test:
	docker build . \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=7 \
		--build-arg PYTHON_PATCH_VERSION=12 \
		-f Dockerfile_deb11_py3yz_test -t $@

.PHONY=xdyn-python-deb11-py308-build
xdyn-python-deb11-py308-build:
	docker build . \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=8 \
		--build-arg PYTHON_PATCH_VERSION=12 \
		-f Dockerfile_deb11_py3yz_pybind -t $@

.PHONY=xdyn-python-deb11-py308-test
xdyn-python-deb11-py308-test:
	docker build . \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=8 \
		--build-arg PYTHON_PATCH_VERSION=12 \
		-f Dockerfile_deb11_py3yz_test -t $@

.PHONY=xdyn-python-deb11-py310-build
xdyn-python-deb11-py310-build:
	docker build . \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=10 \
		--build-arg PYTHON_PATCH_VERSION=1 \
		-f Dockerfile_deb11_py3yz_pybind -t $@

.PHONY=xdyn-python-deb11-py310-test
xdyn-python-deb11-py310-test:
	docker build . \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=10 \
		--build-arg PYTHON_PATCH_VERSION=1 \
		-f Dockerfile_deb11_py3yz_test -t $@
