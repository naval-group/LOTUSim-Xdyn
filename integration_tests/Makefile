.PHONY: all

all:
	@echo "Building pre-requisite images..."
	@make -C ../interfaces/controllers build
	@make -C xdyn
	@make -C grpc_tests
