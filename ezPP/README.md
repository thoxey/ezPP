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

## How it works ##
The backbone of the library is the idea of '**double buffering**' or '**ping ponging'** (The term I prefer and will use). This entails initialising two buffers and using the output of one into the other in a loop and finally pushing to the screen.   
This is a more efficient way to do this since it will only ever generate and use 2 Framebuffers saving on costly operations such as glGenFramebuffers and glGenTextures since there are only called once at launch.
The effects themselves are C++ classes that contain the source code for GLSL shaders that get added to a stack by the user. 
## To be Continued ##
