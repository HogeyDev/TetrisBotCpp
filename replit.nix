{ pkgs }: {
	deps = [
		pkgs.libsForQt5.kcachegrind
  pkgs.valgrind
  pkgs.bear
  pkgs.gh
  pkgs.gcc
		pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}