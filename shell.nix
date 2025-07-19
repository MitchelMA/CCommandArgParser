{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "CommandParser";
  nativeBuildInputs = with pkgs; [
    clang
    cmake
    tmux
    gdb

    doxygen
  ];

  shellHook = ''
    export PATH=$PATH;${pkgs.clang}/bin
  '';
}
