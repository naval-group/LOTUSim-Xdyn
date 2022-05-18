.PHONY: all clean cosim hdb precal-r sim tutorial_14

DOCKER_IMAGE=xdyn
DOCKER_WORKING_DIRECTORY=/xdyn_demos
DOCKER_RUN:=docker run --rm \
    -u $(shell id -u):$(shell id -g) \
    -v $(shell pwd):$(DOCKER_WORKING_DIRECTORY) \
    -w $(DOCKER_WORKING_DIRECTORY)
DOCKER_RUN_BASH:=$(DOCKER_RUN) \
    --entrypoint /bin/bash \
    $(DOCKER_IMAGE) -c
DOCKER_RUN_PYTHON:=$(DOCKER_RUN) python:3.10-slim python3
DASH:=-
UNDERSCORE:=_
SLUG:=$(subst $(DASH),$(UNDERSCORE),$(CI_PROJECT_PATH_SLUG))
COMPOSE_PROJECT_NAME:=$(shell basename `pwd`)$(SLUG)$(CI_PIPELINE_ID)
DOCKER_COMPOSE:=COMPOSE_PROJECT_NAME=$(COMPOSE_PROJECT_NAME) CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose

all: cosim sim hdb precal-r tutorial_14 clean

tutorial_14: filtered_states.csv
	@$(DOCKER_RUN_PYTHON) test_filtered_states.py
	@echo "Tutorial 14's output file passed all test assertions!"

filtered_states.csv: docker-compose-filtered-states.yml filtered_force.py tutorial_14_filtered_states.yml
	@$(DOCKER_COMPOSE) -f docker-compose-filtered-states.yml up -t 0 --exit-code-from xdyn --abort-on-container-exit --build --remove-orphans

tutorial_10_gRPC_force_model.yml:
	@echo "Copying $@ from xdyn's Docker container..."
	@$(DOCKER_RUN_BASH) "cp /usr/demos/$@ /xdyn_demos/"

tutorial_14_filtered_states.yml:
	@echo "Copying $@ from xdyn's Docker container..."
	@$(DOCKER_RUN_BASH) "cp /usr/demos/$@ /xdyn_demos/"

tutorial_10_gRPC_force_model_commands.yml:
	@echo "Copying $@ from xdyn's Docker container..."
	@$(DOCKER_RUN_BASH) "cp /usr/demos/$@ /xdyn_demos/"

tutorial_13_hdb_force_model.yml:
	@echo "Copying $@ from xdyn's Docker container..."
	@$(DOCKER_RUN_BASH) "cp /usr/demos/$@ /xdyn_demos/"

tutorial_13_precal_r_force_model.yml:
	@echo "Copying $@ from xdyn's Docker container..."
	@$(DOCKER_RUN_BASH) "cp /usr/demos/$@ /xdyn_demos/"

ONRT_SIMMAN.raodb.ini:
	@echo "Copying $@ from xdyn's Docker container..."
	@$(DOCKER_RUN_BASH) "cp /usr/demos/$@ /xdyn_demos/"

big.hdb:
	@echo "Copying $@ from xdyn's Docker container..."
	@$(DOCKER_RUN_BASH) "cp /usr/demos/$@ /xdyn_demos/"

precal-r: precal_r_output.csv test_precal_r.py
	@$(DOCKER_RUN_PYTHON) test_precal_r.py
	@echo "All PRECAL_R data was properly passed to the gRPC force model! Test passed!"

hdb: hdb_output.csv test_hdb.py
	@$(DOCKER_RUN_PYTHON) test_hdb.py
	@echo "All HDB data was properly passed to the gRPC force model! Test passed!"

hdb_output.csv: tutorial_13_hdb_force_model.yml docker-compose-hdb.yml big.hdb
	@$(DOCKER_COMPOSE) -f docker-compose-hdb.yml up -t 0 --exit-code-from xdyn --abort-on-container-exit --build --remove-orphans

precal_r_output.csv: tutorial_13_precal_r_force_model.yml docker-compose-precal-r.yml ONRT_SIMMAN.raodb.ini
	@$(DOCKER_COMPOSE) -f docker-compose-precal-r.yml up -t 0 --exit-code-from xdyn --abort-on-container-exit --build --remove-orphans

cosim: tutorial_10_gRPC_force_model.yml tutorial_10_gRPC_force_model_commands.yml
	@$(DOCKER_COMPOSE) -f docker-compose-cosim.yml up -t 0 --exit-code-from test --abort-on-container-exit --build --remove-orphans

sim: tutorial_10_gRPC_force_model.yml tutorial_10_gRPC_force_model_commands.yml
	@$(DOCKER_COMPOSE) -f docker-compose-sim.yml up -t 0 --exit-code-from xdyn --abort-on-container-exit --build --remove-orphans
	@echo "Checking the generated CSV file has 4 lines (including the header line)..."
	@if [ `cat out.csv | wc -l ` -eq 4 ]; then echo "...OK!"; else echo "***error: CSV file has `cat out.csv | wc -l ` line(s)" && exit 1; fi

clean:
	rm -f out.csv\
		  tutorial_10_gRPC_force_model.yml\
		  tutorial_10_gRPC_force_model_commands.yml\
		  hdb_output.csv\
		  precal_r_output.csv\
		  tutorial_13_hdb_force_model.yml\
		  tutorial_13_precal_r_force_model.yml\
		  hdb_output.csv\
		  precal_r_output.csv\
		  big.hdb\
		  output_filtered_force.tsv\
		  tutorial_14_filtered_states.yml\
		  hash.md5\
		  filtered_states.csv
	@$(DOCKER_COMPOSE) -f docker-compose-filtered-states.yml down --remove-orphans
	@$(DOCKER_COMPOSE) -f docker-compose-hdb.yml down --remove-orphans
	@$(DOCKER_COMPOSE) -f docker-compose-precal-r.yml down --remove-orphans
	@$(DOCKER_COMPOSE) -f docker-compose-cosim.yml down --remove-orphans
	@$(DOCKER_COMPOSE) -f docker-compose-sim.yml down --remove-orphans
