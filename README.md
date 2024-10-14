# **Assignment 6 - CS 3388**

This is assignment 6 of the course CS 3388. The submission should include the following files:
* README.md
* Makefile
* main.cpp
* HelperMethods.hpp
* Libraries.hpp
* LoadBMP.hpp
* PLaneMesh.hpp
* shader.hpp
* WaterShader.fragmentshader
* WaterShader.geoshader
* WaterShader.tcs
* WaterShader.tes
* WaterShader.vertexshader

<br>

## **Description**

The goal of this assignment is to render in a water with waves.

<br>

## **Getting  Started**

### *Dependencies and Set-Up*

* Requires the glfw library, GLEW library, and glm library.
* Requires linux or windows that can run c++ and OpenGL.
    * This program is not supported on the macOS, as the shaders encoded into this program is not supported on macOS.
* Ensure all files mentioned at the top of this README are in one folder.

<br>

### *Executing program*

* To compile the program through your terminal use the following command line prompts inside the root directory that you created above:
```
make
```
The above command will compile the program using the Makefile that is provided to you. After that has compiled successfully, you can execute the program with the following command.
```
./main
```
**OPTIONAL**: You may also choose to clean your folder after you are done running it. To do so, run the following command:
```
make clean
```
This will clean up your folder and delete any .o and executable files.

<br>

### *Interacting with the program*
1. As described above, you can interact with a first person camera that moves in spherical coordinates. To move around do the following:
    * Hold the **up arrow** to move closer to the origin.
    * Hold the **down arrow** to move away from the origin.
    * Hold the **left button** of your mouse down as you move your mouse around to drag the camera around the origin. It will spin around the point at a fixed radius.
2. To exit the program, either exit it or press the **escape key** on your keyboard.

<br>

## **About the Code**
There are many different headers and cpp files included with this project. This was to allow a more organized layout, so it would make it easier to follow for those that wanted to look at the source code. 
<br><br>
**HelperMethods.hpp** is a header file with helpful methods used throughout the program.
<br><br>
**Libraries.hpp** is a header file that includes all the necessary libraries.
<br><br>
**LoadBMP.hpp** is a header file that reads in BMP files.
<br><br>
**PlaneMesh.hpp** is a header file that properly renders in the body of water with waves.
<br><br>
**shader.hpp** is a header file that contains a function that properly reads in a fragmentshader and vertexshader file.
<br><br>
**main.cpp** implements the main method, where the environment for the window and the background is set-up.
<br><br>
**Makefile** compiles the program for you. This makes it easier for the user to compile and run the program. How to call this function in the terminal can be found in "Executing program" in the previous section. 

<br>

## **Authors**

* Jake Choi
* Email: [jchoi492@uwo.ca](mailto:jchoi492@uwo.ca)

<br>

## **Acknowledgements**

* CS3388 Prof: Alex Brandt
    * He provided me the template and resources to make this project possible.