{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
    tinycmmc.url = "git+file:///home/ingo/projects/lispparser/sexp-cpp/external/cmake-modules/";
  };

  description = "sexp-cpp flake";

  outputs = { self, nix, nixpkgs, flake-utils, tinycmmc }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        inherit (nixpkgs) lib;
      in rec {
        packages = flake-utils.lib.flattenTree {
          sexp-cpp = pkgs.stdenv.mkDerivation {
            pname = "sexp-cpp";
            version = "0.0.0";
            src = lib.cleanSource ./.;
            nativeBuildInputs = [ pkgs.cmake pkgs.ninja pkgs.gcc tinycmmc.defaultPackage.${system} ];
           };
        };
        defaultPackage = packages.sexp-cpp;
      });
}
