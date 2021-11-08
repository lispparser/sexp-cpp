{
  description = "A S-Expression Parser for C++";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
    tinycmmc.url = "gitlab:grumbel/cmake-modules";
  };

  outputs = { self, nix, nixpkgs, flake-utils, tinycmmc }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in rec {
        packages = flake-utils.lib.flattenTree {
          sexp-cpp = pkgs.stdenv.mkDerivation {
            pname = "sexp-cpp";
            version = "0.1.0";
            src = nixpkgs.lib.cleanSource ./.;
            nativeBuildInputs = [ pkgs.cmake pkgs.ninja pkgs.gcc tinycmmc.defaultPackage.${system} ];
           };
        };
        defaultPackage = packages.sexp-cpp;
      });
}
