FROM archlinux/base

RUN pacman -Sy >/dev/null 2>&1 && \
	pacman -S --noconfirm base-devel grub xorriso gcc cmake zip unzip mtools cppcheck rubygems >/dev/null 2>&1 && \
	pacman -Scc --noconfirm >/dev/null 2>&1

RUN useradd builder -m -u 1000 && \
	passwd -d builder

USER builder
WORKDIR /usr/src

RUN gem install mdl

CMD ["./scripts/build.sh"]
