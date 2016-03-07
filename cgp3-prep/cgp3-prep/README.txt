Requirements:
-------------
These are requirements for Ubuntu. In each case, I've listed where to get the software 
for Ubuntu 12.04. For newer versions of Ubuntu you might not need all the PPAs.

CMake 2.8.7+, make, automake 1.9, pkg-config, doxygen, GLUT:

sudo apt-get install cmake make automake1.9 pkg-config doxygen freeglut3-dev libglm-dev 


GCC 4.8:

sudo apt-add-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install g++-4.8

Boost, at least 1.49 (earlier versions don't play nice with C++11):

sudo add-apt-repository ppa:apokluda/boost1.53 //this will fail to download so the update will probably complain about it
sudo apt-get update
sudo apt-get install libboost1.55-all-dev


Qt5:

sudo apt-add-repository ppa:ubuntu-sdk-team/ppa
sudo apt-get update
sudo apt-get install qtbase5-dev


ADDED PACKAGES:

sudo apt-get install libgl1-mesa-dev qtcreator libglew-dev libqtcore4 libqtgui4 libqt4-opengl libqt4-opengl-dev //line used in 3rd year to fix opengl stuff 
sudo apt-get install build-essential
sudo apt-get install libxmu-dev libxi-dev
sudo apt-get install libcppunit-dev //added in to get testing framework

Client-side git config
----------------------
In the top level of the repository, run the following commands:

git config core.whitespace tab-in-indent
cp .git/hooks/pre-commit.sample .git/hooks/pre-commit

This will prevent checkins with tab indentation or with trailing whitespace.


Compiling
---------
Once all the requirements are running, create a subdirectory called build (or
anything starting with build - you can have multiple build directories), switch
into it, and run

cmake <options> ..

Some useful options
-DCMAKE_CXX_COMPILER=g++-4.8          (to force a specific compiler)
-DCMAKE_BUILD_TYPE=Debug|Release|RelWithDebInfo  (to select build type, only choose one option here!!!)
-DCMAKE_CXX_FLAGS_DEBUG="-gdwarf-3"   (helps older versions of GDB interpret debug symbols)
-DCMAKE_CXX_FLAGS_RELWITHDEBINFO="-O3 -DNDEBUG -g -gdwarf-3" (similar)
-DCMAKE_CXX_FLAGS="-fPIC" (if the compilation complains about position independent code)


in GLHeaders.h

NOTE: the gl3.h file is missing and you need to copy it over manually. If you installed all packages the file should be somewhere on the machine. Navigate to root and search for gl3.h and copy it to this directory in the project: cgp1-prep/khronos_headers/GL

change :

#include <OpenGL/gl3.h> 
#include <OpenGL/glu.h> 

To:

#include <GL/gl3.h> 
#include <GL/glu.h> 

Then run make to build. cmake options are sticky. The executable is ./tesselate/tessviewer


GUI
---
Once a model has been loaded, it can be rotated by holding down the right mouse button and moving the cursor. Zooming is with the mouse wheel. Some sample meshes can be found in ./meshes