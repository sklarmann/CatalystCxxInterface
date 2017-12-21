# CatalystCxxInterface

This Project provides a pure C++ Interface to Paraviews CoProcessing capabilities (Catalyst).

The Code is written for our finite element program and generates a dynamic library. Currently the code is only tested on Windows and will not work under Linux.

At the current state the code is quite messy and I hope a find some time for clean ups.

However I think some of you may be interested in the C++ part for the communication with Paraview. The specific methods can be found in:

management.cpp:  
  -CoProcess(), which sets up the link to Paraview
  -Update(), which transfers the data to Paraview
  
  
