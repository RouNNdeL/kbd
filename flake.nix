{
  description = "Keyboard config for Lemokey P1 Pro";

  inputs = {
    nixpkgs.url = "github:chivay/nixpkgs?ref=qmk-fix";
  };

  outputs = { self, nixpkgs }: let 
        pkgs = import nixpkgs { system = "x86_64-linux"; };
        keychron_src = pkgs.fetchFromGitHub {
          owner = "Keychron";
          repo = "qmk_firmware";
          rev = "b507ea2216cfa51bf43a8be35d180870c97ec433";
          sha256 = "sha256-Y05wyrDT4SBiuDQLfaJvguNz8OmbfzZqR8X1HP0RAEw=";
          fetchSubmodules = true;
        }; 
  in {
    packages.x86_64-linux.flash = pkgs.writeShellScriptBin "flash_script" ''
        export QMK_HOME=${keychron_src}
        ${pkgs.qmk}/bin/qmk flash ${self.packages.x86_64-linux.firmware}/fw.bin
        ${pkgs.wb32-dfu-updater}/bin/wb32-dfu-updater_cli -R
      '';

    packages.x86_64-linux.firmware = pkgs.stdenv.mkDerivation rec {
      pname = "lemokey-p1-firmware";
      version = "0.1";
      buildInputs = [ pkgs.qmk pkgs.git ];

      src = keychron_src;

      SKIP_GIT=1;

      patchPhase = ''
        mkdir -p keyboards/lemokey/p1_pro/ansi_encoder/keymaps/rounndel/
        cp -r ${./rounndel}/* keyboards/lemokey/p1_pro/ansi_encoder/keymaps/rounndel/
      '';

      buildPhase = ''
        qmk compile -kb lemokey/p1_pro/ansi_encoder -km rounndel
      '';

      installPhase = ''
        mkdir -p $out
        cp .build/lemokey_p1_pro_ansi_encoder_rounndel.bin $out/fw.bin
      '';
    };
  };
}
