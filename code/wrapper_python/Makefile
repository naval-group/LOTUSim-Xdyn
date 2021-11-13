
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
	    "python3 -c 'import numpy as np; \
		 import xdyn; \
		 import xdyn.ssc; \
		 from xdyn.ssc.kinematics import KinematicTree as SscKinematicTree; \
		 from xdyn.ssc.kinematics import coriolis_and_centripetal; \
		 help(coriolis_and_centripetal); \
		 ssckt = SscKinematicTree(); \
		 ssckt.add(\"NED\", \"BODY\"); \
		 ssckt.add(\"BODY\", \"CRANE\"); \
		 print(ssckt.get_path(\"NED\", \"CRANE\")); \
		 from xdyn.ssc.kinematics import EulerAngles as SscEulerAngles; \
		 sscea1 = SscEulerAngles(); \
		 print(sscea1); \
		 print(sscea1.phi); \
		 sscea1.phi=666; \
		 print(sscea1.phi); \
		 from xdyn.ssc.kinematics import Point as SscPoint; \
		 sscpoint1 = SscPoint(); \
		 sscpoint2 = SscPoint(\"toto\"); \
		 sscpoint3 = SscPoint(\"toto\",1,2,3); \
		 print(sscpoint3.get_frame()); \
		 print(sscpoint3.v); \
		 sscpoint4 = SscPoint(\"toto\", [7,8,9]); \
		 print(sscpoint4.v); \
		 from xdyn.ssc.kinematics import Wrench as SscWrench; \
		 sscwrench = SscWrench(); \
		 print(sscwrench); \
		 print(\"Uninitialized\"); \
		 print(sscwrench.force); \
		 print(\"Uninitialized\"); \
		 print(sscwrench.torque); \
		 sscwrench.torque = [2,5,6]; \
		 print(sscwrench.torque); \
		 from xdyn.ssc.kinematics import RotationMatrix as SscRotationMatrix; \
		 from xdyn.ssc.kinematics import Transform as SscTransform; \
		 ssctransform1 = SscTransform(); \
		 ssctransform2 = SscTransform(SscPoint(\"frameA\", 1,2,3),\"frameB\"); \
		 ssctransform3 = SscTransform(SscPoint(\"frame1\",4,5,6),np.array([[1.0,0.0,0.0],[0.0,-1.0,0.0],[0.0,0.0,-1.0]]), \"frameB\"); \
		 print(ssctransform1.get_from_frame()); \
		 print(ssctransform2.get_from_frame()); \
		 print(ssctransform3.get_from_frame()); \
		 print(xdyn.add(1, 2)); \
		 print(xdyn.subtract(1, 2)); \
		 from xdyn import EnvironmentAndFrames; \
		 env=EnvironmentAndFrames(); \
		 print(env);\
		 print(dir(env));\
		 env.g=9.81;\
		 from xdyn import BodyStates; \
		 bs = BodyStates(0.0); \
		 print(bs); \
		 print(bs.name); \
		 bs.name=\"gh\"; \
		 print(bs.name); \
		 print(dir(bs)); \
		 from xdyn import Wrench; \
		 wrench = Wrench(sscwrench); \
		 print(wrench); \
		 print(dir(wrench)); \
		 print(wrench.force); \
		 print(wrench.torque); \
		 from xdyn import GravityForceModel; \
		 gfm = GravityForceModel(\"g\",env); \
		 print(gfm);\
		 # res=gfm.get_force(bs,0.0,env, {}); \
		 # print(res);\
		 from xdyn import call_go; \
		 from xdyn import Dog; \
		 d=Dog(); result = call_go(d); print(result); \
		 from xdyn.hdbinterpolators import History; \
		 h1 = History(); \
		 h2 = History(0.2); \
		 from xdyn.hdbinterpolators import RAOData; \
		 raodata = RAOData(); \
		 print(dir(xdyn))'"

clean:
	@rm -rf build
	@rm -rf core
	@rm -rf dist
	@rm -rf *.egg-info
	@rm -rf *.whl
