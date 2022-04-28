.PHONY: all

all:
	@docker build -t changelog .
	@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v5.1.0 v6.0.0 > CHANGELOG.md
	@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v5.0.1 v5.1.0 >> CHANGELOG.md
	@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v5.0.0 v5.0.1 >> CHANGELOG.md
	@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v4.3.0 v5.0.0 >> CHANGELOG.md
	@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v4.2.0 v4.3.0 >> CHANGELOG.md
	@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v4.1.0 v4.2.0 >> CHANGELOG.md
	@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v4.0.4 v4.1.0 >> CHANGELOG.md
	@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v4.0.3 v4.0.4 >> CHANGELOG.md
	@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v4.0.2 v4.0.3 >> CHANGELOG.md
	@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v4.0.1 v4.0.2 >> CHANGELOG.md
	@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v4.0.0 v4.0.1 >> CHANGELOG.md

# The following tags won't work as they predate the use of Gitlab issues & merge requests
#@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v3.1.1 v4.0.0 >> CHANGELOG.md
#@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v3.1.0 v3.1.1 >> CHANGELOG.md
#@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v3.0.3 v3.1.0 >> CHANGELOG.md
#@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v3.0.2 v3.0.3 >> CHANGELOG.md
#@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v3.0.1 v3.0.2 >> CHANGELOG.md
#@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v3.0.0 v3.0.1 >> CHANGELOG.md
#@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v2.0.0 v3.0.0 >> CHANGELOG.md
#@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog v1.0.0 v1.0.0 >> CHANGELOG.md
#@docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog 8fac6c21 v1.0.0 >> CHANGELOG.md

clean:
	@rm -f CHANGELOG.md
