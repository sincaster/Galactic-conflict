with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "buildenv";
  buildInputs = [ allegro5 ];
}
