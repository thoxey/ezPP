ezPP
----------
The basic concept of the library is to make the process of using OpenGL frame buffers to create post processing effects "ez".   
To do this the library obscures all of the binding, initialising etc. from the user and allows them to use the simpler to understand command chain illustrated [here](http://tomhoxey.co.uk/wp-content/uploads/2017/03/Screen-Shot-2017-03-18-at-00.13.57.png).   
Essentially the user can add as many effects as they want to using ezAdd and then each effect class generates a compiled gl shader program that is added to a vector in an ezPostProcessor. Then before the user runs thier draw code they must call ezCapture, run thier draw code as usual, then call ezRender.
For example the following code with turn you image to greyscale:
```

//Before your draw loop
ezPostProcessor myezPPer;
myezPPer.ezInit(SCREENSIZE, SCREENSIZE);
myezPPer.ezAddEffect(new ezGreyscale); 

......

//In your draw loop
myezPPer.ezCapture();
//Draw Something
myezPPer.ezRender();
```

----------

## How it works, Core flow ##
## ezInit ##
To begin using the library you must call **ezInit()**, the first thing this will do is check that you have a valid OpenGL context runnning on at least [version 4.0](https://en.wikipedia.org/wiki/OpenGL#OpenGL_4.0). This is to ensure that all of the features I use will work properly. If we arent running on Mac we next need to init Glew.   
Next we generate two [FBOs](https://www.khronos.org/opengl/wiki/Framebuffer_Object) and attatch a colour buffer that will store our screen texture as well as a renderbuffer that will contain our Depth/Stencil buffer, this is ran in a for loop that executes twice to generate 2 identical buffers.   
Next we need somewhere to draw these textures, and that place is our screen space quad. Thus we declare the verticies for it bind them into a buffer and a Vertex Array, we will need these later for our draw calls.   
The last two things we do in this function is push back a blank effect to the effect list so we get an output on the screen and then we set m_inited to true to allow the next stage to begin.
## ezCapture ##
This is a small function that simply binds to the first of our two custom FBOs then sets the size of the texture, this step is neccesary to ensure that the user is draws thier geometry to our custom buffer.
## ezRender ##
The backbone of the library is the idea of '**double buffering**' or '**ping ponging'** (The term I prefer and will use). This entails initialising two buffers and using the output of one into the other in a loop and finally pushing to the screen.    
This is a more efficient way to do this since it will only ever generate and use 2 Framebuffers saving on costly operations such as glGenFramebuffers and glGenTextures since there are only called once at launch.    
The effects themselves are C++ classes that contain the source code for GLSL shaders that get compiled when they are constructed. Each shader then stores the handle to a gl program that gets used in the ezRender call. Where the following routine is executed:
```
for each effect in effectList
  bind activeFramebuffer
  use effect.shader
  activeFramebuffer.draw
  activeFramebuffer.swap

bind screenFramebuffer
draw to screen
```
This is a standardised routine that uses the previous effect as the input for the next effect. This allows the 'stacking' of effects to create new effects.    
The effects can be added in realtime, or before you run, making this a flexible library for use in realtime projects. For example if you wanted to blur the image when a character got hit in a game you can apply the blur for a few frames and then clear it.    
The library is suited for easy expansion since you only need to add a new class and write in your frag shader in the constructor.    
If your desired new effect needs to take values that determine the output of the shader then look at the ezBrigtness class for how this can be achieved using string stream. In theory anything can be insterted into the shader, for example the ez3x3Kernel class shows insterting a string into the class.
