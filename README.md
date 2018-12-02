# CatalystCxxInterface

This Project provides a pure C++ Interface to Paraviews CoProcessing capabilities (Catalyst) (www.paraview.org).

The Code generates a dynamic library. Currently the code is only tested on Windows and will not work under Linux.

At the current state the code is quite messy and I hope I find some time to clean it up.

However I think some of you may be interested in the C++ part for the communication with Paraview. The specific methods can be found in:

management.cpp:  
  CoProcess(), which sets up the link to Paraview and Update(), which transfers the data to Paraview
  
  
