# About
This repository concerns the generation of minecraft bedrock dedicated server header files through the PDB file provided with the release of each BDS version.

## Compilation
To build this, you need to have llvm on your machine. Refer to the documentation for more information.

Here's how you can download it on Ubuntu/Debian.
```shell
sudo apt-get update
sudo apt-get install llvm clang
```

Then, to compile the project:
```shell
mkdir -p build && cd build
cmake ..
make
```