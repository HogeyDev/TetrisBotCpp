{ pkgs }: {
	deps = [
		pkgs.bear
  pkgs.gh
  pkgs.gcc
		pkgs.clang_12
		pkgs.ccls
		# pkgs.clangd
		pkgs.gdb
		pkgs.gnumake
	];
}