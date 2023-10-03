{ pkgs }: {
	deps = [
		pkgs.gcc
		pkgs.clang_12
		pkgs.ccls
		# pkgs.clangd
		pkgs.gdb
		pkgs.gnumake
	];
}