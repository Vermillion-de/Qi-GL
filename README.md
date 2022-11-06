## This is Qi's reimplementation of GLFW for visualization of 3d Datasets, our functions mainly about mimic( :) ).
### currently we support functions like
1. mouse/key move/rotate object(vim for rotate, wasd for move), scroll to max/minize 
2. basic key bindings, e.g. esc->quit
3. using texture with stb_image
4. use imgui for interaction
5. work with cmake
6. Updating into OpenGL3 with GLSL.
7. Allowing many interaction based on ImGui(for graphics study).
### TODO:
1. add light (commonly good for light) settings, summarize those rendering things into imgui. 
2. Allow more man-graphics interaction (mouse rasting)
3. Integral algorithms like meshlab.
4. Add more things I know: multy threads, argparser, json file for rendering, ply loader(![happly](happly))
5. CMake Tests and run time config(and loader)
6. More intuitive rotation.(4-dim numbers).
7. github workflows (and submodules for cross platform)
8. LICENSE(finally)
### Main reference
Mainly learnt from [polyscope](), [libigl](), [learnopengl], thanks to all of them! without who I will never be able to play on such an amazing graphics playground.