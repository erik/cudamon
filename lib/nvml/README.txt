--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
NVIDIA Management Library (NVML)
README
Version v4.304
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
January 2013
Copyright (C) 2013 NVIDIA Corp.


Description
--------------------------
The NVIDIA Management Library (NVML) is a C-based API for
monitoring and managing various states of the NVIDIA GPU devices.
It provides a direct access to the queries and commands exposed
via nvidia-smi. The runtime version of NVML ships with the NVIDIA
display driver, and the SDK provides the appropriate header, stub
libraries and sample applications. Each new version of NVML is
backwards compatible and is intended to be a platform for building
3rd party applications.

For more information see the docs in the doc/ directory.

Important files
--------------------------
README.txt: This file

COPYING.txt: A copy of the License under which TDK is released

INSTALL.txt: An explanation of how the tools should be installed

doc/
  nvml.pdf: User manual for the NVML API
  man/: Man pages for the NVML API

example/
  example.c: A sample C program written to link and use NVML
  example.sln, example.vcproj: Visual Studio solutions to compile the example.c
  Makefile: A standard Linux Makefile to compile the example.c

include/
  nvml.h: A header for the NVML API

lib/, lib64/: Set of stub NVML libraries to link against.  Note that these
              files do not contain an implementation.  These are needed for
              linking when the build machine may not have a compatible
              version of the NVIDIA display Driver installed.


Supported NVIDIA Hardware
--------------------------
All NVIDIA(R) Tesla(TM) GPUs running NVIDIA Display Driver v304 and up are supported


Supported Software Platform
--------------------------
32-bit operating systems
- Linux

64-bit operating systems
- Windows 7
- Windows Server 2008 R2
- Linux


Build instructions
--------------------------
In order to compile the example application on Windows, use the Visual Studio
solution provided under the example/ directory. In order to compile the example
application on Linux, run "make" under the example/ directory.


More information
--------------------------
For more information, please visit http://developer.nvidia.com/nvidia-management-library-nvml

