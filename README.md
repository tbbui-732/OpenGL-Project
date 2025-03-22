# OpenGL Project

Welcome! This repo is dedicated to documenting and showcasing my progress as I work through the LearnOpenGL text. My goal is to build a strong foundation in graphics programming. 

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

Here are some screenshots from the basic-lights chapter

<img width="580" alt="THEME=FACTORY" src="https://github.com/user-attachments/assets/0bd9cceb-abe9-4e37-8292-cfce38069ca3" />
<img width="580" alt="THEME=HORROR" src="https://github.com/user-attachments/assets/42de1253-473a-47fe-8934-93603d1d2ddd" />
<img width="580" alt="THEME=NORMALDIMMED" src="https://github.com/user-attachments/assets/4412806e-2c26-493b-aa1a-d1145f94b568" />

Here's the transparent windows from the advanced-opengl section
<img width="720" alt="transparent-window" src="https://github.com/user-attachments/assets/76031bc0-6819-439c-85a0-80b5b64647fb" />

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
