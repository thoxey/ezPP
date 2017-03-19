**Shader Debugger**
---------------
A quick tool I made to help test shaders.


----------


**Instructions**
Simply replace the two relevant GLSL files with the shaders you want to debug and it will make a GL context and debug them for you.

It will output one of three things:

 1. "[Frag/Vert] Shader Compiled", meaning the shader compiled
 2. "[Frag/Vert] Shader Blank, Ignored", If one of the GLSL files is blank it will just ignore it
 3. Otherwise it will push the error log to std::cerr


----------


**Requirements**
The tool relies on SDL to set up the context but could be rewritten to use your preferred method of creating a GL context. It is also mac/linux only, but it shouldn't be too hard to change it for windows.

