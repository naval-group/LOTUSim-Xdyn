.PHONY: all

all:
	@echo "Building pre-requisite images..."
	@docker build -t "xdyn-force-grpc:python3" -f ../interfaces/forces/Dockerfile ../interfaces
	@make -C ../interfaces/controllers build
	@make -C xdyn
	@make -C grpc_tests
	@make -C json_tests
