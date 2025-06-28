{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "CommandParser";
  nativeBuildInputs = with pkgs; [
    clang
    cmake
    tmux
    gdb
  ];

  shellHook = ''
    export PATH=$PATH;${pkgs.clang}/bin
  '';
}
