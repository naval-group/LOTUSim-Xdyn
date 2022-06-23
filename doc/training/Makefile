.PHONY: all clean

all: remote_models.pptx

remote_models.pptx: remote_models.md
	@docker build -t pandoc .
	@docker run -t --rm -u $(shell id -u):$(shell id -g) -v $(shell pwd):/shared -w /shared pandoc $< --toc -o $@
	@rm -rf plantuml-images/

clean:
	@rm -rf remote_models.pptx
	@rm -rf ./?
