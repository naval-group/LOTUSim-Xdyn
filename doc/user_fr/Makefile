.PHONY: all build clean check images

ROOT_DIRECTORY=../..

all: check doc.html

check: build concatenated_doc.md
	@docker run -t --rm \
		-u $(shell id -u):$(shell id -g) \
		--entrypoint xdyn \
		-w /work \
		-v $(shell pwd)/tuto_execution:/work \
		xweave \
		tutorial_06_propulsion.yml --dt 0.1 --tend 1 > err
	@if grep -q "Warning" err ; then echo "There was a warning in one of the tutorials" && rm -f err && exit 1; else echo "Tutorials executed with no warnings or errors" && rm -f err && exit 0; fi

concatenated_doc.pmd: \
    metadata.yaml \
    init.md introduction.md \
    interfaces_utilisateur.md \
    reperes_et_conventions.md \
    modeles_environnementaux.md \
    calcul_de_GM.md \
    courbes_de_GZ.md \
    solver.md diffraction_radiation.md \
    modeles_efforts.md \
    controleurs.md \
    tutorial_00.md \
    tutorial_01.md \
    tutorial_02.md \
    tutorial_03.md \
    tutorial_06.md \
    tutorial_09.md \
    tutorial_10.md \
    tutorial_11.md \
    tutorial_12.md \
    tutorial_13.md \
    tutorial_14.md
	rm -f concatenated_doc.pmd
	for f in $^ ; do (cat $$f; echo "") >> concatenated_doc.pmd ; done

m4 :=  docker run -t --rm \
    -v $(shell pwd):/work \
    -v $(shell pwd)/$(ROOT_DIRECTORY)/interfaces:/interfaces \
    -v $(shell pwd)/$(ROOT_DIRECTORY)/integration_tests/grpc_tests/controller:/controller \
    -v $(shell pwd)/$(ROOT_DIRECTORY)/integration_tests/grpc_tests/force:/force \
    -w /work \
    -u $(shell id -u):$(shell id -g) \
    --entrypoint /usr/bin/m4 \
    sirehna/pandoc@sha256:cc4ec54f6e09cda3cc57c6a125764bc0bb12712511e3f0560a69de6f762761e7

tutorial_10.md: tutorial_10.m4.md
	$(m4) -I/interfaces $< > $@

tutorial_11.md: tutorial_11.m4.md
	$(m4) -I/controller $< > $@

tuto_execution/filtered_states.csv: $(ROOT_DIRECTORY)/integration_tests/grpc_tests/force/filtered_states.csv
	@mkdir -p tuto_execution
	cp $< $@

$(ROOT_DIRECTORY)/integration_tests/grpc_tests/force/filtered_states.csv:
	@make -C $(ROOT_DIRECTORY)/integration_tests/grpc_tests/force/ filtered_states.csv

tutorial_14.md: tutorial_14.m4.md tuto_execution/filtered_states.csv
	$(m4) -I/controller -I/force $< > $@

interfaces_utilisateur.md: \
        interfaces_utilisateur.m4.md \
        $(ROOT_DIRECTORY)/interfaces/proto/cosimulation.proto \
        $(ROOT_DIRECTORY)/interfaces/proto/model_exchange.proto
	$(m4) -I/interfaces $< > $@

controleurs.md: controleurs.m4.md $(ROOT_DIRECTORY)/interfaces/proto/controller.proto
	$(m4) -I/interfaces $< > $@

xdyn-pweave := docker run -t \
    -v $(shell pwd):/work \
    -w /work \
    -u $(shell id -u):$(shell id -g) \
    --entrypoint /usr/bin/xdyn-weave \
    xweave

xdyn-pandoc := docker run -t \
    -v $(shell pwd):/work \
    -w /work \
    -u $(shell id -u):$(shell id -g) \
    --entrypoint /usr/bin/pandoc \
    xweave

xdyn-python3 := docker run -t \
    -v $(shell pwd):/work \
    -w /work \
    -u $(shell id -u):$(shell id -g) \
    --entrypoint /usr/local/bin/python3 \
    xweave

images: images/spectrum.py
	cd images && \
	docker run --rm -t \
	    -u $(shell id -u):$(shell id -g) \
	    -v $(shell pwd)/images:/work_images:rw \
	    -w /work_images \
	    --entrypoint /usr/local/bin/python3 \
	    xweave \
	    spectrum.py && \
	cd ..

build: $(ROOT_DIRECTORY)/build_deb11/xdyn.deb
	@cp $< .
	@docker build --tag xweave .
	@rm xdyn.deb

concatenated_doc.md: concatenated_doc.pmd
	$(xdyn-pweave) -f markdown -k python3 $< -o $@

concatenated_doc_pandoc.md: concatenated_doc.md pandoc_layout.py
	$(xdyn-python3) pandoc_layout.py $< -o $@

concatenated_doc_pandoc_png.md: concatenated_doc_pandoc.md
	sed -e 's/svg/png/g' $< > $@

doc.html: concatenated_doc_pandoc.md images
	$(xdyn-pandoc) --title-prefix=XDYN \
	    -t html \
	    --standalone --self-contained \
	    --toc --toc-depth=3 \
	    --mathml \
	    --highlight-style pygments \
	    --filter pandoc-eqnos \
	    --filter pandoc-fignos \
	    --number-sections \
	    -f markdown -V lang=fr \
	    --css stylesheet.css \
	    concatenated_doc_pandoc.md \
	    -o doc.html


# Pandoc filter pandoc-eqnos and pandoc-fignos produce non readable docx file...
doc.docx: concatenated_doc_pandoc_png.md
	make -C images
	$(xdyn-pandoc) --title-prefix=XDYN \
	    -s \
	    --highlight-style pygments \
	    -t docx \
	    -o doc.docx \
	    concatenated_doc_pandoc_png.md

clean:
	rm -f \
	    concatenated_doc.pmd \
	    concatenated_doc.md \
	    concatenated_doc_pandoc.md \
	    concatenated_doc_pandoc_png.md \
	    doc.html \
	    doc.docx \
	    interfaces_utilisateur.md \
	    tutorial_10.md \
	    tutorial_11.md \
	    tutorial_14.md \
	    controleurs.md \
	    pict*.svg
	rm -rf figures
	rm -rf tuto_execution
	rm -rf *.aux *.docx *.html *.pdf *.log *.out *.tex
