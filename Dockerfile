FROM debian:sid

RUN apt-get update -y >/dev/null 2>&1 && \
	apt-get install -y build-essential grub xorriso gcc cmake zip unzip curl cppcheck rubygems cscope doxygen git >/dev/null 2>&1 

RUN useradd builder -m -u 1000 && \
	passwd -d builder

RUN gem install mdl

USER builder
WORKDIR /usr/src

CMD ["./scripts/build.sh"]
