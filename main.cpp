#include "PlaneMesh.hpp"
GLFWwindow* window;

//////////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[])
{

	///////////////////////////////////////////////////////
	float screenW = 1500;
	float screenH = 1500;
	float stepsize = 1.0f;

	float xmin = -5;
	float xmax = 5;

	if (argc > 1 ) {
		screenW = atoi(argv[1]);
	}
	if (argc > 2) {
		screenH = atoi(argv[2]);
	}
	if (argc > 3) {
		stepsize = atof(argv[3]);
	}
	if (argc > 4) {
		xmin = atof(argv[4]);
	}
	if (argc > 5) {
		xmax = atof(argv[5]);
	}


	///////////////////////////////////////////////////////

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( screenW, screenH, "Phong", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}


	PlaneMesh plane(xmin, xmax, stepsize);
	
	//TextureMesh boat("Assets/boat.ply", "Assets/boat.bmp", 1);
	//TextureMesh head("Assets/head.ply", "Assets/head.bmp", 1);
	//TextureMesh eyes("Assets/eyes.ply", "Assets/eyes.bmp", 1);


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.2f, 0.2f, 0.3f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glDisable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), screenW/screenH, 0.001f, 1000.0f);

	glm::mat4 V;

	glm::vec3 lightpos(5.0f, 30.0f, 5.0f);
	glm::vec4 color1(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glm::vec3 cameraPos(10.0f, 10.0f, 10.0f);	// Set up initial camera when loaded into the house.
	glm::vec3 camDirect(0.0f, 0.0f, 0.0f);	// Initialize new variable to be used to update new camera direction.
	glm::vec3 up(0.0f, 1.0f, 0.0f);			// Normalized vector in the direction that is up.
	double mouseDownX;
    double mouseDownY;
    bool firstPress = true;
    int state;
    float radius = magnitudeVec(cameraPos);
	float phi = findPhi(radius, cameraPos);
	float theta = findTheta(radius, cameraPos);

	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if (firstPress) {
                glfwGetCursorPos(window, &mouseDownX, &mouseDownY);
				firstPress = false;
            }

            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            theta += glm::radians((xpos - mouseDownX));
			if (theta < 0.0f) {
				theta += glm::radians(360.0f);
			}
			else if (theta > glm::radians(360.0f)) {
				theta -= glm::radians(360.0f);
			}

			phi -= glm::radians((ypos - mouseDownY));
			if (phi < 0.0f) {
				phi += glm::radians(180.0f);
			}
			else if (phi > glm::radians(180.0f)) {
				phi -= glm::radians(180.0f);
			}

            mouseDownX = xpos;
            mouseDownY = ypos;
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
            firstPress = true;
        }

		float cameraMove = 0.5f;
		//Check if up arrow was pressed.
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && radius > 1 && (radius - cameraMove) > 1) {
			radius -= cameraMove;
		}
		//Check if down arrow was pressed.
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			radius += cameraMove;
		}
		
		cameraPos = sphericalToCartesian(glm::vec3(radius, theta, phi));

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), screenW/screenH, 0.001f, 1000.0f); // Set up projection matrix with vertical field of view of 45 degrees
		glLoadMatrixf(glm::value_ptr(Projection));

		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();

		glm::mat4 V = glm::lookAt(cameraPos, camDirect, up); // Update where the camera is looking.

		glm::mat4 M = glm::mat4(1.0f);

		glLoadMatrixf(glm::value_ptr(V));
		
		plane.draw(lightpos, V, Projection);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

