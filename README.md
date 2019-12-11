# ResourceManagerMultithreaded
This project is a project I made at ISART Digital Montreal in order to learn multithreading. This project is able tot load .obj files and render them in an OpenGL context (OpenGL 4.5). This implementation of my obj loader is very simple and has issues. It cannot be used on complex models.

Issue : There is an issue with the normals which render lighting incorrectly, this is because vertices and normals have to be linked together especially when the obj file is made of quad entirely or even partially. The fact that I ignored subobjects and made it a single object introduced that issue. I won't make a fix for that for the moment, you should just use a 3D loader like assimp if you want to load 3D models in your project.
