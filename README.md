## This is Qi's reimplementation of GLFW for visualization of 3d Datasets, our functions mainly about mimic( :) ).
### currently we support functions like
1. mouse/key move/rotate object(vim for rotate, wasd for move), scroll to max/minize 
2. basic key bindings, e.g. esc->quit
3. using texture with stb_image
4. use imgui for interaction
5. work with cmake (submodule, external depedencies)
6. Updating into OpenGL3 with GLSL, with GLFW
7. Basic algorithms: Float param, Halfedge structure(still caution with boundary)

### TODO:
1. Package many things(light, material, shader, texture) into one `GL` object to manage, and allow more graphics lab.
2. Allow more human-graphics interaction (mouse rasting => select vertices, faces, change shapes, rotate => Hamilton number)
3. Using more fancy libs: argparser, json loader(for scenerio); make more concept about config.
4. More fancy algorithms: (marching cubes, ray tracing).
5. More modern things (tiny-cuda-nn).
### Main reference
Mainly learnt from [polyscope](), [libigl](), [learnopengl], thanks to all of them! without who I will never be able to play on such an amazing graphics playground.