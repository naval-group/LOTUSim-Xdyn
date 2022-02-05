FROM debian:bullseye-slim
RUN apt-get update && apt-get install --no-install-recommends -y \
  git
ADD changelog.sh /usr/bin/

ENTRYPOINT ["/bin/sh", "/usr/bin/changelog.sh"]
