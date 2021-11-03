#
# Build stage
#
FROM debian:buster as build-env

RUN apt-get update && apt-get install -y python3-pip wget
RUN pip3 install pandoc-plantuml-filter

# make slim-jdk
RUN wget https://download.java.net/java/GA/jdk11/9/GPL/openjdk-11.0.2_linux-x64_bin.tar.gz -P /tmp/ \
 && tar xfvz /tmp/openjdk-11.0.2_linux-x64_bin.tar.gz -C /tmp/ \
 && rm /tmp/openjdk-11.0.2_linux-x64_bin.tar.gz \
 && /tmp/jdk-11.0.2/bin/jlink --output /opt/openjdk-11-slim \
    --add-modules java.base,java.datatransfer,java.desktop,java.logging,java.prefs,java.scripting,java.xml \
 && rm -rf /tmp

# make plant UML
RUN mkdir -p /opt/plantuml/  \
 && wget https://github.com/plantuml/plantuml/releases/download/v1.2021.13/plantuml-1.2021.13.jar -O /opt/plantuml/plantuml.jar \
 && echo '#!/bin/bash\n\
    /opt/openjdk-11-slim/bin/java -jar /opt/plantuml/plantuml.jar $@' > /usr/bin/plantuml \
 && chmod a+x /usr/bin/plantuml

#
# Run stage
#
FROM debian:buster as setup-env

RUN apt-get update && apt-get install -y python3 pandoc graphviz libfreetype6 fontconfig git \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN ln -s /usr/bin/python3 /usr/bin/python

COPY --from=build-env /usr/local/lib/python3.7/dist-packages/ /usr/local/lib/python3.7/dist-packages/
COPY --from=build-env /usr/bin/plantuml /usr/bin/plantuml
COPY --from=build-env /usr/local/bin/pandoc-plantuml /usr/local/bin/pandoc-plantuml
COPY --from=build-env /opt/plantuml/ /opt/plantuml/
COPY --from=build-env /opt/openjdk-11-slim/ /opt/openjdk-11-slim/

WORKDIR /var/docs/
ENTRYPOINT ["pandoc", "--filter", "pandoc-plantuml"]
