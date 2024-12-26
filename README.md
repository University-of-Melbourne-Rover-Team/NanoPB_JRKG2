# This repository contains a Raspberry Pi Pico project using the Pico SDK and Nanopb.

To use this template, the following are required:

1. Install Docker
2. nstall Visual Studio Code (VS Code) and the Dev Container Extension

## Accessing Dev Container

To use Dev Container:

1. Open VS Code and navigate to current repo directory.
2. Open VS Code Shell Command and select "Open Folder in Container"

**Note**: To open VS Code Shell Command you use `control+shift+p` on Windows and `cmd+shift+p` on Mac OS.


## Building the project

Before you can build the template project you need to generate the protobuff file which is done via:

```
python3 $NANOPB_PATH/generator/nanopb_generator.py uart.proto 
```

Once the protobuff file is build then you can apply the following step to build the project:
1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make`

If you be able to build a sucessful project.