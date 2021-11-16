
DIR := $(notdir $(shell pwd))
DOCKER_AS_ROOT:=docker run -t --rm -w /opt/share/${DIR} -v $(shell pwd)/..:/opt/share
DOCKER_AS_USER:=$(DOCKER_AS_ROOT) -u $(shell id -u):$(shell id -g)

build_docker_build_container:
	docker build . -f Dockerfile_pybind -t pybind

demo_package: demo_package_build demo_package_test_build demo_package_test_run

demo_package_build:
	$(DOCKER_AS_USER) pybind /bin/bash -c \
	    "python3 setup.py bdist_wheel &&\
		mv dist/*.whl ."

demo_package_test_build:
	docker build . -f Dockerfile_test -t pybindtest

demo_package_test_run:
	$(DOCKER_AS_USER) pybindtest /bin/bash -c "cd unit_tests && python3 test_xdyn.py"
	$(DOCKER_AS_USER) pybindtest /bin/bash -c "cd unit_tests && python3 test_mng.py"
	$(DOCKER_AS_USER) pybindtest /bin/bash -c "cd unit_tests && python3 test_hydropolarforcemodel.py"

clean:
	@rm -rf build
	@rm -rf core
	@rm -rf dist
	@rm -rf *.egg-info
	@rm -rf *.whl
