.PHONY: all clean

all: remote_models.pptx

remote_models.pptx: remote_models.md
	@docker run -t --rm -u $(shell id -u):$(shell id -g) -v $(shell pwd):/shared -w /shared pandoc/core:2.14.0.3 $< --toc -o $@

clean:
	rm -rf remote_models.pptx
