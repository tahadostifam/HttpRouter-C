{
  description = "HttpRouter C";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      overlays = [ ];
      pkgs = import nixpkgs {
        inherit system overlays;
      };
    in
  {
      devShells.${system}.default = pkgs.mkShell {
        name = "cyrus-dev-shell";

        buildInputs = with pkgs; [
          gcc
          libgcc
          glibc
          libgccjit
          libffi
          libffi.dev
          isl
        ];
        
        shellHook = ''
          export LIBRARY_PATH="${pkgs.glibc}/lib"
        '';
      };
    };
}