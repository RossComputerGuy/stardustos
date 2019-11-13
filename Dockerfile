FROM debian:sid

RUN apt-get upgrade -y >/dev/null 2>&1 && \
	apt-get install -y base-devel grub xorriso gcc cmake zip unzip mtools cppcheck rubygems >/dev/null 2>&1

RUN useradd builder -m -u 1000 && \
	passwd -d builder

USER builder
WORKDIR /usr/src

RUN gem install mdl

CMD ["./scripts/build.sh"]
