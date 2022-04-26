with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "term-do";

  buildInputs = with pkgs; [
    gnumake
    gcc
    boost
  ];
}
