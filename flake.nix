{
  description = "A S-Expression Parser for C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-21.11";
    flake-utils.url = "github:numtide/flake-utils";

    tinycmmc.url = "gitlab:grumbel/cmake-modules";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
    tinycmmc.inputs.flake-utils.follows = "flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, tinycmmc }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in rec {
        packages = flake-utils.lib.flattenTree {
          sexp-cpp = pkgs.stdenv.mkDerivation {
            pname = "sexp-cpp";
            version = "0.1.0";
            src = nixpkgs.lib.cleanSource ./.;
            cmakeFlags = [ "-DBUILD_TESTS=ON" ];
            doCheck = true;
            nativeBuildInputs = [
              pkgs.cmake
              pkgs.ninja
              pkgs.gcc
            ];
            buildInputs = [
              tinycmmc.defaultPackage.${system}

              pkgs.gtest
            ];
           };
        };
        defaultPackage = packages.sexp-cpp;
      });
}
