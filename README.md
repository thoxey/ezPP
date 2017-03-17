## EZ Post Processing ##
An OpenGL post processing library that facilitates adding multiple post processing effects to any Open GL scene. The library is written in C++11 making heavy use of new C++11 features.

**ezPP**     
This folder contains the static library and and source code for it, I recommend rebuilding the library since I develop on Mac and Linux and the .dylib or the .so may be out of date.

----------
**Demo:**   
There is a functional SDL demo that shows a basic RGB triangle that should allow you to get an idea of what each effect does.

----------
**Tests:**   

The test folder contains a google test project for doing unit testing, as well as a very basic shader debugger I wrote to check for compilation erros in shaders.

## Requirements/Dependencies ##   
The core library itself is only dependent on OpenGL, so ensuring you have a modern machine it should be able to run perfectly fine. Since the Library requires at least OpenGL 3.2.    
**QT Creator**   
I wrote all my code in QT Creator and therefore the project files are written for that editor. While it is obvious essential to use QT it will make you life easier if you do!    
**SDL**    
The demos are written with [SDL](https://www.libsdl.org/)  which you will need to include yourself, I recommend using [Brew](https://brew.sh/) on Mac to install it. Then just use:

    brew install sdl
For Linux (And probably Windows?) there is a tutorial here: https://wiki.libsdl.org/Installation .    
**Google Test**    
The Unit testing is all done using the Google Test, which is great since it integrates right into QT creator.
Google test can be found on their [Github](https://github.com/google/googletest) with all the information you could possibly need on how to run it!    
**C++11**    
As mentioned before this library makes heavy use of C++11 features, such as:

 - Range Based Fors
 - std::find
 - The auto keyword
 - Raw String literals
 
And some more as well, however since C++11 is increasingly becoming the norm now I figured I might as well let it make my life easier.

## Want More Documentation?! ##
Each project directory also has its own readme explaining in further detail its contents!
