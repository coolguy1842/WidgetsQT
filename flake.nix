{
    description = "Developer shell flake for clang";

    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11"; 
    };

    outputs = { self, nixpkgs, systems }: let
        forEachSystem = nixpkgs.lib.genAttrs (import systems);

        debugLibrary = pkg: pkg.overrideAttrs (attrs: {
            separateDebugInfo = true;
        });
    in {
        devShells = forEachSystem (system: let
            pkgs = import nixpkgs {
                inherit system;

                config = {};
                overlays = [
                    (final: prev: {
                        kdePackages.layer-shell-qt = prev.kdePackages.layer-shell-qt.overrideAttrs { separateDebugInfo = true; };
                    })
                ];
            };

            # qt6Debug = debugLibrary pkgs.qt6.full;
            myDebugInfoDirs = pkgs.symlinkJoin {
                name = "myDebugInfoDirs";
                paths = with pkgs; [
                    kdePackages.layer-shell-qt.debug
                ];
            };
            
        in {
            default = pkgs.mkShell {
                NIX_DEBUG_INFO_DIRS = "${pkgs.lib.getLib myDebugInfoDirs}/lib/debug";

                hardeningDisable = [ "all" ];
                
                nativeBuildInputs = with pkgs; [
                    llvmPackages_latest.lldb
                    clang
                    pkgconf
                    ninja
                    cmake
                    meson
                    gdb
                ];

                buildInputs = with pkgs; [
                    qt6.full
                    kdePackages.layer-shell-qt
                    dart-sass
                ];
            };
        });
    };
}