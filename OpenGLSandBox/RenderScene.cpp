#include "common_header.h"

#include "win_OpenGLApp.h"

#include "shaders.h"

float fSea[12];
float fTriangle[9]; // Data to render triangle (3 vertices, each has 3 floats)
float fQuad[12]; // Data to render quad using triangle strips (4 vertices, each has 3 floats)
float fBoat[12];
float fSun[24];
float fSeaColor[12];
float fTriangleColor[9];
float fQuadColor[12];
float fBoatColor[12];
float fSunColor[24];

UINT uiVBO[10];
UINT uiVAO[5];

CShader shVertex, shFragment;
CShaderProgram spMain;

// Initializes OpenGL features that will be used.
// lpParam - Pointer to anything you want.
void InitScene(LPVOID lpParam)
{
	glClearColor(0.8f, 0.9f, 1.0f, 1.0f);

	// setup sea
	fSea[0] = 1.0f; fSea[1] = -0.75f; fSea[2] = 0.0f;
	fSea[3] = 1.0f; fSea[4] = -1.0f; fSea[5] = 0.0f;
	fSea[6] = -1.0f; fSea[7] = -0.75f; fSea[8] = 0.0f;
	fSea[9] = -1.0f; fSea[10] =-1.0f; fSea[11] = 0.0f;

	// setup sea color
	fSeaColor[0] = 0.0f; fSeaColor[1]	= 0.65f; fSeaColor[2] = 0.8f;
	fSeaColor[3] = 0.0f; fSeaColor[4]	= 0.65f; fSeaColor[8] = 0.8f;
	fSeaColor[6] = 0.0f; fSeaColor[7]	= 0.65f; fSeaColor[5] = 0.8f;
	fSeaColor[9] = 0.0f; fSeaColor[10] =  0.65f; fSeaColor[11] =0.8f;

	// Setup triangle vertices
	fTriangle[0] = -0.005f; fTriangle[1] = -0.15f; fTriangle[2] = 0.0f;
	fTriangle[3] = -0.005f; fTriangle[4] = -0.6f; fTriangle[5] = 0.0f;
	fTriangle[6] = -0.2f; fTriangle[7] = -0.6f; fTriangle[8] = 0.0f;

	// Setup triangle color

	fTriangleColor[0] = 1.0f; fTriangleColor[1] = 0.0f; fTriangleColor[2] = 0.0f;
	fTriangleColor[3] = 0.0f; fTriangleColor[4] = 1.0f; fTriangleColor[5] = 0.0f;
	fTriangleColor[6] = 0.0f; fTriangleColor[7] = 0.0f; fTriangleColor[8] = 1.0f;
 
	// Setup quad vertices
 
	fQuad[0] = -0.005f; fQuad[1] = -0.1f; fQuad[2] = 0.0f;
	fQuad[3] = -0.005f; fQuad[4] = -0.7f; fQuad[5] = 0.0f;
	fQuad[6] = 0.005f; fQuad[7] = -0.1f; fQuad[8] = 0.0f;
	fQuad[9] = 0.005f; fQuad[10] = -0.7f; fQuad[11] = 0.0f;

	// Setup quad color

	fQuadColor[0] = 1.0f; fQuadColor[1] = 0.0f; fQuadColor[2] = 0.0f;
	fQuadColor[3] = 0.0f; fQuadColor[4] = 1.0f; fQuadColor[8] = 0.0f;
	fQuadColor[6] = 0.0f; fQuadColor[7] = 0.0f; fQuadColor[5] = 1.0f;
	fQuadColor[9] = 1.0f; fQuadColor[10] = 1.0f; fQuadColor[11] = 0.0f;

	// Setup boat vertices

	fBoat[0] = -0.2f; fBoat[1] = -0.8f; fBoat[2] = 0.0f;
	fBoat[3] = -0.3f; fBoat[4] = -0.7f; fBoat[5] = 0.0f;
	fBoat[6] = 0.2f; fBoat[7] = -0.8f; fBoat[8] = 0.0f;
	fBoat[9] = 0.3f; fBoat[10] = -0.7f; fBoat[11] = 0.0f;

	// Setup boat color

	fBoatColor[0] = 1.0f; fBoatColor[1] = 0.0f; fBoatColor[2] = 0.0f;
	fBoatColor[3] = 0.0f; fBoatColor[4] = 1.0f; fBoatColor[8] = 0.0f;
	fBoatColor[6] = 0.0f; fBoatColor[7] = 0.0f; fBoatColor[5] = 1.0f;
	fBoatColor[9] = 1.0f; fBoatColor[10] = 1.0f; fBoatColor[11] = 0.0f;

	// Setup sun vertices center = 0.5, 0.5
	fSun[0] = 0.54f; fSun[1] = 0.5f; fSun[2] = 0.0f;
	fSun[3] = 0.53f; fSun[4] = 0.53f; fSun[5] = 0.0f;
	fSun[6] = 0.5f; fSun[7] =  0.54f; fSun[8] = 0.0f;
	fSun[9] = 0.47f; fSun[10] = 0.53f; fSun[11] = 0.0f;
	fSun[12] = 0.46f; fSun[13] = 0.5f; fSun[14] = 0.0f;
	fSun[15] = 0.47f; fSun[16] = 0.47f; fSun[17] = 0.0f;
	fSun[18] = 0.5f; fSun[19] = 0.46f; fSun[20] = 0.0f;
	fSun[21] = 0.53f; fSun[22] = 0.47f; fSun[23] = 0.0f;

	// Setup sun color
	fSunColor[0]  = 1.0;   fSunColor[1]  = 0.0f; fSunColor[2] =  0.0f;
	fSunColor[3]  = 1.0f;  fSunColor[4]  = 0.0f; fSunColor[5] =  0.0f;
	fSunColor[6]  = 1.0f;  fSunColor[7]  = 0.0f; fSunColor[8] =  0.0f;
	fSunColor[9]  = 1.0f;  fSunColor[10] = 0.0f; fSunColor[11] = 0.0f;
	fSunColor[12] = 1.0f; fSunColor[13]  = 0.0f; fSunColor[14] = 0.0f;
	fSunColor[15] = 1.0f; fSunColor[16]  = 0.0f; fSunColor[17] = 0.0f;
	fSunColor[18] = 1.0f; fSunColor[19]  = 0.0f; fSunColor[20] = 0.0f;
	fSunColor[21] = 1.0f; fSunColor[22]  = 0.0f; fSunColor[23] = 0.0f;

	glGenVertexArrays(5, uiVAO); 
	glGenBuffers(10, uiVBO); 


	// Setup whole sea
	glBindVertexArray(uiVAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), fSea, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), fSeaColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Setup whole triangle
	glBindVertexArray(uiVAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangleColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Setup whole quad
	glBindVertexArray(uiVAO[2]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[4]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[5]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuadColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Setup whole boat
	glBindVertexArray(uiVAO[3]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[6]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), fBoat, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[7]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), fBoatColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Setup sun
	glBindVertexArray(uiVAO[4]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[8]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), fSun, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[9]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), fSunColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Load shaders and create shader program

	shVertex.LoadShader("data\\shaders\\shader.vert", GL_VERTEX_SHADER);
	shFragment.LoadShader("data\\shaders\\shader.frag", GL_FRAGMENT_SHADER);

	spMain.CreateProgram();
	spMain.AddShaderToProgram(&shVertex);
	spMain.AddShaderToProgram(&shFragment);

	spMain.LinkProgram();
	spMain.UseProgram();
}

// Renders whole scene.
// lpParam - Pointer to anything you want.
void RenderScene(LPVOID lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	// We just clear color
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(uiVAO[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(uiVAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(uiVAO[2]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(uiVAO[3]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(uiVAO[4]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

	oglControl->SwapBuffersM();
}

// Releases OpenGL scene.
// lpParam - Pointer to anything you want.
void ReleaseScene(LPVOID lpParam)
{
	spMain.DeleteProgram();

	shVertex.DeleteShader();
	shFragment.DeleteShader();
}