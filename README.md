# OpenGL Project

Welcome! This repo is dedicated to documenting and showcasing my progress as I work through the LearnOpenGL text. My goal is to build a strong foundation in graphics programming via OpenGL.

## About
This repository serves as a collection of projects and experimentation from the LearnOpenGL text. It's a work-in-progress as I progress through each chapter.

## Progress
Completed Part 1: Basic OpenGL, which covers foundational graphics concepts.
* Rendering a triangle
* Shaders (texture and fragment)
* Textures
* Transformations
* Simulated camera

Completed Part 2: Basic Lights, which covers phong-lighting, diffuse/specular maps, and combining multiple types of lights

Completed Part 3: Loading in complex 3D models with Assimp

Currently working on Part 4: Advanced OpenGL
* Stencil buffers
* Depth testing
* Blending
* Face culling
* Framebuffers

Here are some themes I created in the basic lighting chapter:

<img width="480" alt="factory" src="https://github.com/tbbui-732/OpenGL-Project/blob/main/public/Basic%20Lighting/factory.png" /> (factory)
<img width="480" alt="horror" src="https://github.com/tbbui-732/OpenGL-Project/blob/main/public/Basic%20Lighting/horror.png" /> (horror)
<img width="480" alt="dimmed" src="https://github.com/tbbui-732/OpenGL-Project/blob/main/public/Basic%20Lighting/dimmed.png" /> (dimmed)

Here's the transparent windows (blending subchapter) with different preprocessing effects using kernel matrices and framebuffers + textures. 

<img width="480" alt="identity" src="https://github.com/tbbui-732/OpenGL-Project/blob/main/public/Kernel%20Matrix%20Preprocessing/identity.png" /> (Identity kernel)
<img width="480" alt="ridge" src="https://github.com/tbbui-732/OpenGL-Project/blob/main/public/Kernel%20Matrix%20Preprocessing/ridge.png" /> (Ridge kernel)
<img width="480" alt="sharpen" src="https://github.com/tbbui-732/OpenGL-Project/blob/main/public/Kernel%20Matrix%20Preprocessing/sharpen.png" /> (Sharpen kernel)
<img width="480" alt="gaussian blur" src="https://github.com/tbbui-732/OpenGL-Project/blob/main/public/Kernel%20Matrix%20Preprocessing/gaussian-blur.png" /> (Gaussian blur)

## Features
* Writen in C++
* Utilized OpenGL
* GLSL for shaders

## Building the Project
To run the project, you will need clang installed and please make sure you're running on a UNIX system! I have not tested this on Windows. The binaries for GLFW and GLAD are included in this repository, but they were built on my Macbook.

### How to Run
1. Clone this repository
2. Navigate to the project directory
3. Update the makefile to target file you want to build. You'll need to change SRCPATH and SRCNAME!
5. Build the project with make

## Acknowledgement
Thanks so much to Joey de Vries for creating this amazing piece of resource!
