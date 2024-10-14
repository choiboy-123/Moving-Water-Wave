#include "LoadBMP.hpp"

class PlaneMesh {
	
	std::vector<float> verts;
	std::vector<float> normals;
	std::vector<int> indices;
	glm::vec4 modelColor;
	float min, max;
	int numVerts, numIndices;

	GLuint programID;

	GLuint MVPID;
	GLuint PID;
	GLuint VID;
	GLuint MID;
	GLuint LightPosID;
	GLuint WatTexID;
	GLuint DisplaceTexID;

	GLuint VAO, vertVBO, normVBO, EBO;


	void planeMeshQuads(float min, float max, float stepsize) {

		// The following coordinate system works as if (min, 0, min) is the origin
		// And then builds up the mesh from that origin down (in z)
		// and then to the right (in x).
		// So, one "row" of the mesh's vertices have a fixed x and increasing z

		//manually create a first column of vertices
		float x = min;
		float y = 0;
		for (float z = min; z <= max; z += stepsize) {
			verts.push_back(x);
			verts.push_back(y);
			verts.push_back(z);
			normals.push_back(0);
			normals.push_back(1);
			normals.push_back(0);
		}

		for (float x = min+stepsize; x <= max; x += stepsize) {
			for (float z = min; z <= max; z += stepsize) {
				verts.push_back(x);
				verts.push_back(y);
				verts.push_back(z);
				normals.push_back(0);
				normals.push_back(1);
				normals.push_back(0);
			}
		}

		int nCols = (max-min)/stepsize + 1;
		int i = 0, j = 0;
		for (float x = min; x < max; x += stepsize) {
			j = 0;
			for (float z = min; z < max; z += stepsize) {
				indices.push_back(i*nCols + j);
				indices.push_back(i*nCols + j + 1);
				indices.push_back((i+1)*nCols + j + 1);
				indices.push_back((i+1)*nCols + j);
				++j;
			}
			++i;
		}
	}

public:

	PlaneMesh(float min, float max, float stepsize) {
		this->min = min;
		this->max = max;
		modelColor = glm::vec4(0, 1.0f, 1.0f, 1.0f);

		planeMeshQuads(min, max, stepsize);
		numVerts = verts.size()/3;
		numIndices = indices.size();

		glPatchParameteri(GL_PATCH_VERTICES, 4);
		programID = LoadShaders("WaterShader.vertexshader", "WaterShader.geoshader", "WaterShader.fragmentshader");
		
		// Generate VAO ID
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);   

		// Generate Buffer for vertices
		glGenBuffers(1, &vertVBO);
		glBindBuffer(GL_ARRAY_BUFFER, vertVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*verts.size(), verts.data(), GL_STATIC_DRAW);
		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                      // attribute location in shader
			3,                      // size
			GL_FLOAT,               // type
			GL_FALSE,               // normalized?
			0,      				// stride
			(void*) 0               // buffer offset
		);

		// Generate Buffer for normal coordinates.
		glGenBuffers(1, &normVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.size(), normals.data(), GL_STATIC_DRAW);
		// 2nd attribute buffer : texture coordinates
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,                                  // attribute location in shader
			3,                                  // size
			GL_FLOAT,                           // type
			GL_FALSE,                           // normalized?
			0,                 					// stride
			(void *) 0     						// buffer offset
		);

		// Generate Buffer for faces.
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), indices.data(), GL_STATIC_DRAW);

		//shaders and uniforms
		unsigned char* dataWat;
		unsigned int widthWat, heightWat;
		loadBMP("Assets/water.bmp", &dataWat, &widthWat, &heightWat);

		glGenTextures(1, &WatTexID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, WatTexID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthWat, heightWat, 0, GL_BGR, GL_UNSIGNED_BYTE, dataWat);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		unsigned char* dataDis;
		unsigned int widthDis, heightDis;
		loadBMP("Assets/displacement-map1.bmp", &dataDis, &widthDis, &heightDis);

		glGenTextures(1, &DisplaceTexID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, DisplaceTexID);
		glTexImage2D(GL_TEXTURE_2D, 1, GL_RGB, widthDis, heightDis, 0, GL_BGR, GL_UNSIGNED_BYTE, dataDis);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		MVPID = glGetUniformLocation(programID, "MVP");
		PID = glGetUniformLocation(programID, "P");
		VID = glGetUniformLocation(programID, "V");
		MID = glGetUniformLocation(programID, "M");
		LightPosID = glGetUniformLocation(programID, "LightPosition_worldspace");
	}

	void draw(glm::vec3 lightPos, glm::mat4 V, glm::mat4 P) {
		glm::mat4 M = glm::mat4(1.0f);
		glm::mat4 MVP = P * V * M;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, WatTexID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, DisplaceTexID);

		glUseProgram(programID);

		glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(PID, 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(VID, 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(MID, 1, GL_FALSE, &M[0][0]);
		glUniform3f(LightPosID, lightPos.x, lightPos.y, lightPos.z);
		glUniform1i(glGetUniformLocation(programID, "waterTexture"), 0);
		glUniform1i(glGetUniformLocation(programID, "displacementTexture"), 1);
		glUniform1f(glGetUniformLocation(programID, "time"), glfwGetTime());

		glBindVertexArray(VAO);
		glDrawElements(GL_PATCHES, numIndices, GL_UNSIGNED_INT, (void*)0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);

	}


};
