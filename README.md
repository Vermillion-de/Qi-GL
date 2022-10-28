## This is Qi's reimplementation of GLUT for visualization of 3d Datasets, our functions mainly about mimic( :) ).
### currently we support functions like
1. mouse move object (left / right)
2. orientation(rotation), scroll to max/minize
3. vim (and aswd) keybinding for rotate / move
4. basic key binding, e.g. esc->quit
5. using texture with stb_image
### features
1. can be initilized with functors (no warry if use eigen or vector for data).
### TODO:
1. make things into objects 
2. add ImGUI
3. add light (commonly good for light)
4. reimplement with modern opengl (mainly with the "draw" function)
5. user interfare for chose point with mouse casting.
6. Integral algorithms like meshlab.