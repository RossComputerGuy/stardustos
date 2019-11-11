/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */

extern void _init();
extern void _fini();

void __libc_init() {
  // TODO: open stdin, stdout, and stderr
  _init();
}

void __libc_fini(int code) {
  _fini();
  // TODO: close stdin, stdout, and stderr
  while (1);
}
