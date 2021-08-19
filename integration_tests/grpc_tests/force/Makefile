.PHONY: all clean cosim sim

all: cosim sim hdb clean

tutorial_10_gRPC_force_model.yml:
	@echo "Copying $@ from xdyn's Docker container..."
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/tutorial_10_gRPC_force_model.yml /xdyn_demos/"

tutorial_10_gRPC_force_model_commands.yml:
	@echo "Copying $@ from xdyn's Docker container..."
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/tutorial_10_gRPC_force_model_commands.yml /xdyn_demos/"

tutorial_13_hdb_force_model.yml:
	@echo "Copying $@ from xdyn's Docker container..."
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/$@ /xdyn_demos/"

hdb: tutorial_13_hdb_force_model.yml
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f docker-compose-hdb.yml rm -f
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f docker-compose-hdb.yml up -t 0 --exit-code-from xdyn --abort-on-container-exit --build --remove-orphans


cosim: tutorial_10_gRPC_force_model.yml tutorial_10_gRPC_force_model_commands.yml
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f docker-compose-cosim.yml rm -f
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f docker-compose-cosim.yml up -t 0 --exit-code-from test --abort-on-container-exit --build --remove-orphans

sim: tutorial_10_gRPC_force_model.yml tutorial_10_gRPC_force_model_commands.yml
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f docker-compose-sim.yml rm -f
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f docker-compose-sim.yml up -t 0 --exit-code-from xdyn --abort-on-container-exit --build --remove-orphans
	@echo "Checking the generated CSV file has 4 lines (including the header line)..."
	@if [ `cat out.csv | wc -l ` -eq 4 ]; then echo "...OK!"; else echo "***error: CSV file has `cat out.csv | wc -l ` line(s)" && exit 1; fi

clean:
	rm -f out.csv tutorial_10_gRPC_force_model.yml tutorial_10_gRPC_force_model_commands.yml hdb_output.csv
