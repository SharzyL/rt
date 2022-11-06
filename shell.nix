{ pkgs ? import <nixpkgs> {}}:

let
  llvm14 = pkgs.llvmPackages_14;
in llvm14.stdenv.mkDerivation {
    name = "rt";
    buildInputs = with pkgs; [
      llvm14.openmp

      protobuf ninja cmake

      git cacert # make cmake fetchContent happy
    ];
    shellHook = ''
      # due to https://github.com/NixOS/nixpkgs/pull/192943
      export NIX_CC=" "
    '';
  }
