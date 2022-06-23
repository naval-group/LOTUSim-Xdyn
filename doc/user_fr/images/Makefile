# This makefile generates for each svg image the corresponding png image

PNGS := $(patsubst %.svg,%.png,$(wildcard *.svg))

SPECTRA := \
	spectrumBretschneider.svg \
	spectrumGaussian.svg \
	spectrumITTC.svg \
	spectrumJonswap.svg \
	spectrumOchi.svg \
	spectrumPiersonMoskowitz.svg \
	spectrumVignatBovis.svg \
	spectrumComparison1.svg \
	spectrumComparison2.svg \
	waveMonochromatique.svg \
	waveBichromatique.svg

SPECTRA_PNGS := $(patsubst %.svg,%.png,$(SPECTRA))


all: build all_images

spectra_images: $(SPECTRA) $(SPECTRA_PNGS)
all_images: $(PNGS) $(SPECTRA) $(SPECTRA_PNGS)

%.svg: spectrum.py
	python3 spectrum.py --names $@

# We use inkscape inside a docker container to convert a svg to a PNG
# ./svg2png.sh $<
%.png: %.svg
	@echo Generating PNG: $@
	@docker run --rm -t \
	    -u $(shell id -u):$(shell id -g) \
	    -v $(shell pwd):/work:rw \
	    -w /work \
	    inkscape \
	    /usr/bin/inkscape --without-gui --export-type="png" \
		-d 150 \
		$< > /dev/null 2> /dev/null

clean:
	rm -rf $(SPECTRA) $(PNGS) $(SPECTRA_PNGS)

build:
	docker build . -t inkscape
	# docker run -it inkscape /usr/bin/inkscape --help
	# docker run -it inkscape /usr/bin/inkscape --version
