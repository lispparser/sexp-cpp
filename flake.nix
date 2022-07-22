{
  description = "A S-Expression Parser for C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-22.05";
    flake-utils.url = "github:numtide/flake-utils";

    tinycmmc.url = "github:grumbel/tinycmmc";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
    tinycmmc.inputs.flake-utils.follows = "flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, tinycmmc }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in rec {
        packages = flake-utils.lib.flattenTree rec {
          sexp-cpp = pkgs.stdenv.mkDerivation {
            pname = "sexp-cpp";
            version = "0.1.0";
            src = nixpkgs.lib.cleanSource ./.;
            cmakeFlags = [
              "-DBUILD_TESTS=ON"
              "-DUSE_CXX17=ON"
            ];
            doCheck = true;
            nativeBuildInputs = [
              pkgs.cmake
            ];
            buildInputs = [
              tinycmmc.packages.${system}.default

              pkgs.gtest
            ];
          };
          default = sexp-cpp;
        };
      }
    );
}
