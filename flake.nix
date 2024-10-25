# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    flake.nix                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/24 10:51:54 by adjoly            #+#    #+#              #
#    Updated: 2024/10/25 14:17:30 by babonnet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

{
	description = "A Nix-flake-based C/C++ development environment";
	inputs = {
		nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
		flake-parts.url = "github:hercules-ci/flake-parts";
		pogit = {
			url = "github:y-syo/pogit";
			inputs.nixpkgs.follows = "nixpkgs"; # to use your nixpkgs instance instead of the provided one
		};
	};

	outputs = inputs:
		inputs.flake-parts.lib.mkFlake { inherit inputs; } {
			systems = [ "x86_64-linux" "aarch64-linux" ];

			perSystem = { pkgs, ... }:
			{
				devShells.default = pkgs.mkShell {
					buildInputs = with pkgs; [
						gcc
						clang
					];

					packages = with pkgs; [
						clang
						gcc
						gdb
						norminette
						valgrind
						hexchat
						inputs.pogit.packages.${system}.default
						git
					];
				};
			};
		};
}
