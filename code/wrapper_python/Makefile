
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
	$(DOCKER_AS_USER) pybindtest /bin/bash -c \
	    "python3 -c 'import pyxdyn; \
		 import pyxdyn.ssc; \
		 from pyxdyn.ssc.kinematics import Point as SscPoint; \
		 sscpoint1 = SscPoint(); \
		 sscpoint2 = SscPoint(\"toto\"); \
		 sscpoint3 = SscPoint(\"toto\",1,2,3); \
		 print(sscpoint3.get_frame()); \
		 print(sscpoint3.v); \
		 sscpoint4 = SscPoint(\"toto\", [7,8,9]); \
		 print(sscpoint4.v); \
		 from pyxdyn.ssc.kinematics import Wrench as SscWrench; \
		 sscwrench = SscWrench(); \
		 print(sscwrench); \
		 print(\"Uninitialized\"); \
		 print(sscwrench.force); \
		 print(\"Uninitialized\"); \
		 print(sscwrench.torque); \
		 sscwrench.torque = [2,5,6]; \
		 print(sscwrench.torque); \
		 print(pyxdyn.add(1, 2)); \
		 print(pyxdyn.subtract(1, 2)); \
		 from pyxdyn import EnvironmentAndFrames; \
		 env=EnvironmentAndFrames(); \
		 print(env);\
		 print(dir(env));\
		 env.g=9.81;\
		 from pyxdyn import BodyStates; \
		 bs = BodyStates(0.0); \
		 print(bs); \
		 print(bs.name); \
		 bs.name=\"gh\"; \
		 print(bs.name); \
		 print(dir(bs)); \
		 from pyxdyn import Wrench; \
		 wrench = Wrench(sscwrench); \
		 print(wrench); \
		 print(dir(wrench)); \
		 print(wrench.force); \
		 print(wrench.torque); \
		 from pyxdyn import GravityForceModel; \
		 gfm = GravityForceModel(\"g\",env); \
		 print(gfm);\
		 # res=gfm.get_force(bs,0.0,env, {}); \
		 # print(res);\
		 from pyxdyn import call_go; \
		 from pyxdyn import Dog; \
		 d=Dog(); result = call_go(d); print(result); \
		 print(dir(pyxdyn))'"

clean:
	@rm -rf build
	@rm -rf core
	@rm -rf dist
	@rm -rf *.egg-info
	@rm -rf *.whl
