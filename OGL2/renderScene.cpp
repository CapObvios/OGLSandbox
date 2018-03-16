#include "common_header.h"

#include "win_OpenGLApp.h"

#include "shaders.h"
#include "texture.h"
#include "vertexBufferObject.h"

#include "flyingCamera.h"

#include "skybox.h"

#include "dirLight.h"
#include "spotLight.h"
#include "pointLight.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define NUMTEXTURES 10

#define FOG_EQUATION_LINEAR		0
#define FOG_EQUATION_EXP		1
#define FOG_EQUATION_EXP2		2
#define FOG_EQUATION_ABSENCE	3

/* One VBO, where all static data are stored now,
in this tutorial vertex is stored as 3 floats for
position, 2 floats for texture coordinate and
3 floats for normal vector. */

CVertexBufferObject vboSceneObjects, vboCubeInd, vboCube, vboHouse,	vboHouseInd;
UINT uiVAOs[3]; // Only one VAO now

CTexture tTextures[NUMTEXTURES];
CFlyingCamera cCamera;

CSkybox sbMainSkybox;

CDirectionalLight dlSun;
CSpotLight slFlashLight[2];
CPointLight plLight[2];

CAssimpModel assimpModels[3];

CMaterial matShiny;

namespace FogParameters
{
	float fDensity = 0.04f;
	float fStart = 10.0f;
	float fEnd = 75.0f;
	glm::vec4 vFogColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	int iFogEquation = FOG_EQUATION_EXP; // 0 = linear, 1 = exp, 2 = exp2
};

#include "static_geometry.h"

// Initializes OpenGL features that will be used.
// lpParam - Pointer to anything you want.
void InitScene(LPVOID lpParam)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Prepare all scene objects

	
	glGenVertexArrays(3, uiVAOs); // Create one VAO

	//================================================================
	vboSceneObjects.CreateVBO();

	glBindVertexArray(uiVAOs[0]);

	vboSceneObjects.BindVBO();

	AddSceneObjects(vboSceneObjects);

	vboSceneObjects.UploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));

	//================================================================
	glBindVertexArray(uiVAOs[1]);

	vboCube.CreateVBO();

	vboCube.BindVBO();
	AddCube(vboCube);
	vboCube.UploadDataToGPU(GL_STATIC_DRAW);

	vboCubeInd.CreateVBO();
	// Bind indices
	vboCubeInd.BindVBO(GL_ELEMENT_ARRAY_BUFFER);
	vboCubeInd.AddData(&iCubeindices, sizeof(iCubeindices));
	vboCubeInd.UploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));


	//================================================================
	glBindVertexArray(uiVAOs[2]);

	vboHouse.CreateVBO();

	vboHouse.BindVBO();
	AddCube(vboHouse);
	vboHouse.UploadDataToGPU(GL_STATIC_DRAW);

	
	// Bind indices
	vboHouseInd.CreateVBO();
	vboHouseInd.BindVBO(GL_ELEMENT_ARRAY_BUFFER);
	vboHouseInd.AddData(&iCubeindices, sizeof(iCubeindices));
	vboHouseInd.UploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	//================================================================

	assimpModels[0].LoadModelFromFile("data\\models\\treasure_chest_obj\\treasure_chest.obj");
	assimpModels[1].LoadModelFromFile("data\\models\\house\\house.obj");
	assimpModels[2].LoadModelFromFile("data\\models\\teapot\\teapot.obj");

	CAssimpModel::FinalizeVBO();


	if(!PrepareShaderPrograms())
	{
		PostQuitMessage(0);
		return;
	}

	matShiny = CMaterial(1.0f, 32.0f);

	// Load textures

	string sTextureNames[] = {
		"grass.png",		"met_wall01a.jpg",	"tower.jpg",		"tire.jpg",		"ground.jpg",
		"CrabNebula.jpg",	"HelixNebula.jpg",	"grassyCliff.png",	"metal.jpg",	"rope.jpg"
	};

	FOR(i, NUMTEXTURES)
	{
		tTextures[i].LoadTexture2D("data\\textures\\"+sTextureNames[i], true);
		tTextures[i].SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	}

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	
	cCamera = CFlyingCamera(glm::vec3(0.0f, 10.0f, 120.0f), glm::vec3(0.0f, 10.0f, 119.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.001f);
	cCamera.SetMovingKeys('W', 'S', 'A', 'D');

	sbMainSkybox.LoadSkybox("data\\skyboxes\\mountains\\", "back.jpg", "front.jpg", "left.jpg", "right.jpg", "top.jpg", "bottom.jpg");

	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f) / 2, -sqrt(2.0f) / 2, 0), 1.0f);

	slFlashLight[0] = CSpotLight(glm::vec3(1.0f, 1.0f, 0.8f), glm::vec3(-5.0f, 25.0f, 40.0f), glm::vec3(0.0f, -1.0f, 0.0f), 1, 35.0f, 0.017f);
	slFlashLight[1] = CSpotLight(glm::vec3(0.6f, 0.6f, 0.2f), glm::vec3(-5.0f, 25.0f, 80.0f), glm::vec3(0.0f, -1.0f, 0.0f), 1, 35.0f, 0.017f);
	plLight[0] = CPointLight(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(50.0f, 1.0f, 25.0f), 0.15f, 0.3f, 0.007f, 0.00008f);
	plLight[1] = CPointLight(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(50.0f, 1.0f, 50.0f), 0.15f, 0.3f, 0.007f, 0.00008f);
}

float fGlobalAngle;
float fTextureContribution = 0.5f;
float fContributionClock = 0.5f;
// Renders whole scene.
// lpParam - Pointer to anything you want.
void RenderScene(LPVOID lpParam)
{

#if !USE_FREEGLUT
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spMain.UseProgram();

	glm::mat4 mModelMatrix, mView;

	
	glm::vec3 vCameraDir = glm::normalize(cCamera.vView - cCamera.vEye);
	
	slFlashLight[0].SetUniformData(&spMain, "spotLight[0]");
	slFlashLight[1].SetUniformData(&spMain, "spotLight[1]");

	plLight[0].SetUniformData(&spMain, "pointLight[0]");
	plLight[1].SetUniformData(&spMain, "pointLight[1]");

#if !USE_FREEGLUT
	oglControl->ResizeOpenGLViewportFull();
#endif	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spMain.UseProgram();

#if USE_FREEGLUT
	spMain.SetUniform("matrices.projMatrix", glm::perspective(45.0f, float(glutGet(GLUT_WINDOW_WIDTH)) / float(glutGet(GLUT_WINDOW_HEIGHT)), 0.5f, 1000.0f));
#else
	spMain.SetUniform("matrices.projMatrix", oglControl->GetProjectionMatrix());
#endif
	spMain.SetUniform("gSamplers[0]", 0);
	spMain.SetUniform("gSamplers[1]", 1);
	spMain.SetUniform("fTextureContributions[0]", 1.0f);
	spMain.SetUniform("fTextureContributions[1]", fTextureContribution);
	spMain.SetUniform("numTextures", 1);
	
	spMain.SetUniform("fogParams.iEquation", FogParameters::iFogEquation);
	spMain.SetUniform("fogParams.vFogColor", FogParameters::vFogColor);

	spMain.SetUniform("useShiny", false);

	mView = cCamera.Look();
	spMain.SetUniform("matrices.viewMatrix", &mView);

	mModelMatrix = glm::translate(glm::mat4(1.0f), cCamera.vEye);
	
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mView*mModelMatrix)));

	if (FogParameters::iFogEquation == FOG_EQUATION_LINEAR)
	{
		spMain.SetUniform("fogParams.fStart", FogParameters::fStart);
		spMain.SetUniform("fogParams.fEnd", FogParameters::fEnd);
	}
	else
		spMain.SetUniform("fogParams.fDensity", FogParameters::fDensity);

	CDirectionalLight dlSun2 = dlSun;

	// We set full ambient for skybox, so that its color isn't affected by directional light

	dlSun2.fAmbient = 1.0f;
	dlSun2.vColor = glm::vec3(1.0f, 1.0f, 1.0f);
	dlSun2.SetUniformData(&spMain, "sunLight");
	
	spMain.SetUniform("receiveLight", false);
	sbMainSkybox.RenderSkybox();

	spMain.SetUniform("receiveLight", true);

	dlSun.SetUniformData(&spMain, "sunLight");

	matShiny.SetUniformData(&spMain, "matActive");

	CAssimpModel::BindModelsVAO();

	glm::mat4 mModel = glm::translate(glm::mat4(1.0), glm::vec3(-30.0f, 0, 0));
	//mModel = glm::scale(mModel, glm::vec3(8, 8, 8));
	spMain.SetUniform("matrices.modelMatrix", mModel);
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModel)));

	assimpModels[0].RenderModel();

	CAssimpModel::BindModelsVAO();

	mModel = glm::translate(glm::mat4(1.0), glm::vec3(-30.0f, 0, -50.0f));
	mModel = glm::scale(mModel, glm::vec3(8, 8, 8));
	spMain.SetUniform("matrices.modelMatrix", mModel);
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModel)));

	assimpModels[1].RenderModel();

	CAssimpModel::BindModelsVAO();

	mModel = glm::translate(glm::mat4(1.0), glm::vec3(30.0f, 0, 15.0f));
	mModel = glm::scale(mModel, glm::vec3(0.1f, 0.1f, 0.1f));
	spMain.SetUniform("matrices.modelMatrix", mModel);
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModel)));

	assimpModels[2].RenderModel();

	mModel = glm::translate(glm::mat4(1.0), glm::vec3(30.0f, 0, 30.0f));
	mModel = glm::scale(mModel, glm::vec3(0.1f, 0.1f, 0.1f));
	spMain.SetUniform("matrices.modelMatrix", mModel);
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModel)));
	spMain.SetUniform("useShiny", true);

	assimpModels[2].RenderModel();

	spMain.SetUniform("useShiny", false);

	glBindVertexArray(uiVAOs[1]);
	// Render cubes
	glm::mat4 mModelToCamera;

	tTextures[5].BindTexture();
	tTextures[6].BindTexture(1);

	spMain.SetUniform("fTextureContributions[0]", max(0.0f, 1.0f - fTextureContribution));
	spMain.SetUniform("numTextures", 2);

	float PI = float(atan(1.0)*4.0);

	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW); //Done by default
	glm::vec3 vPos2 = glm::vec3(30.0f, 8.0f, 0.0f);
	mModelMatrix = glm::mat4(1.0f);
	mModelMatrix = glm::translate(mModelMatrix, vPos2);
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(16.0f, 16.0f, 16.0f));
	// We need to transform normals properly, it's done by transpose of inverse matrix of rotations and scales
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", mModelMatrix);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
	glDisable(GL_CULL_FACE);	

	spMain.SetUniform("fTextureContributions[0]", 1.0f);
	spMain.SetUniform("numTextures", 1);


	glBindVertexArray(uiVAOs[2]);
	tTextures[2].BindTexture();

	FOR(i, 6)
	{
		glm::vec3 vPos6 = glm::vec3(-10.0f, -7.8f, 0.0f + i * 16.0f);
		mModelMatrix = glm::mat4(1.0f);
		mModelMatrix = glm::translate(mModelMatrix, vPos6);
		mModelMatrix = glm::scale(mModelMatrix, glm::vec3(16.0f, 16.0f, 16.0f));
		// We need to transform normals properly, it's done by transpose of inverse matrix of rotations and scales
		spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
		spMain.SetUniform("matrices.modelMatrix", mModelMatrix);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

	}

	glDisable(GL_CULL_FACE);

	

	glBindVertexArray(uiVAOs[0]);

	

	spMain.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0f));
	spMain.SetUniform("matrices.normalMatrix", glm::mat4(1.0f));

	//==================================================================
	// Render ground
	tTextures[7].BindTexture();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// render tire
	tTextures[3].BindTexture();
	// Now it's gonna float in the air
	glm::vec3 vPos = glm::vec3(-5.0f, 5.0f, 0.0f);
	mModelMatrix = glm::translate(glm::mat4(1.0), vPos);
	//mModelMatrix = glm::rotate(mModelMatrix, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	glDrawArrays(GL_TRIANGLES, 6, iTorusFaces * 3);

	FOR(i, 3)
	{
		// vertical pipe
		glm::vec3 vPos3 = glm::vec3(0.0f , 0.0f , 0.0f + 40.0f*i);

		tTextures[8].BindTexture();
		mModelMatrix = glm::translate(glm::mat4(1.0), vPos3);
		//mModelMatrix = glm::rotate(mModelMatrix, fGlobalAngle, glm::vec3(1.0f, 0.0f, 0.0f));
		spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
		spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
		glDrawArrays(GL_TRIANGLES, 6 + iTorusFaces * 3, iCylinderFaces * 3);

		// horizontal pipe
		glm::vec3 vPos4 = glm::vec3(0.0f, 25.0f, 0.0f + 40.0f*i);

		mModelMatrix = glm::translate(glm::mat4(1.0), vPos4);
		mModelMatrix = glm::rotate(mModelMatrix, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
		spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
		glDrawArrays(GL_TRIANGLES, 6 + iTorusFaces * 3 + iCylinderFaces * 3, iCylinderFaces2 * 3);
	}


	// rope
	glm::vec3 vPos5 = glm::vec3(-5.0f, 8.0f, 0.0f);

	tTextures[9].BindTexture();

	mModelMatrix = glm::translate(glm::mat4(1.0), vPos5);
	//mModelMatrix = glm::rotate(mModelMatrix, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	glDrawArrays(GL_TRIANGLES, 6 + iTorusFaces * 3 + iCylinderFaces * 3 + iCylinderFaces2 * 3, iRopeFaces* 3);

	// light
	glm::vec3 vPos7 = glm::vec3(50.0f, 0.0f, 25.0f);

	tTextures[8].BindTexture();

	mModelMatrix = glm::translate(glm::mat4(1.0), vPos7);
	//mModelMatrix = glm::rotate(mModelMatrix, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	glDrawArrays(GL_TRIANGLES, 6 + iTorusFaces * 3 + iCylinderFaces * 3 + iCylinderFaces2 * 3 + iRopeFaces * 3, iLightFaces * 3);

	glm::vec3 vPos8 = glm::vec3(50.0f, 0.0f, 50.0f);

	mModelMatrix = glm::translate(glm::mat4(1.0), vPos8);
	//mModelMatrix = glm::rotate(mModelMatrix, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	glDrawArrays(GL_TRIANGLES, 6 + iTorusFaces * 3 + iCylinderFaces * 3 + iCylinderFaces2 * 3 + iRopeFaces * 3, iLightFaces * 3);

	

	cCamera.Update();

	glEnable(GL_DEPTH_TEST);

	/*if (Keys::Key('Q')) fTextureContribution += appMain.sof(-0.2f);
	if (Keys::Key('E')) fTextureContribution += appMain.sof(0.2f);*/

	if (Keys::Key('Q'))matShiny.fSpecularIntensity -= appMain.sof(0.2f);
	if (Keys::Key('E'))matShiny.fSpecularIntensity += appMain.sof(0.2f);

	if (Keys::Key('Z'))matShiny.fSpecularPower -= appMain.sof(8.0f);
	if (Keys::Key('C'))matShiny.fSpecularPower += appMain.sof(8.0f);

	fContributionClock += appMain.sof(0.2f);

	/*fTextureContribution = min(max(0.0f, fContributionClock), 1.0f);*/
	fTextureContribution = std::fmodf(fContributionClock, 1.0f);
	if ((int)fContributionClock % 2 == 0) 
		fTextureContribution = 1.0f - fTextureContribution;

	if (Keys::Onekey('L'))
		FOR(i, 2) slFlashLight[i].bOn = 1 - slFlashLight[i].bOn;

	if (Keys::Onekey('P'))
		FOR(i, 2) plLight[i].bOn = !plLight[i].bOn;


	if (FogParameters::iFogEquation == FOG_EQUATION_LINEAR)
	{
		if (Keys::Key(VK_ADD))
			FogParameters::fStart += appMain.sof(15.0f);
		if (Keys::Key(VK_SUBTRACT))
			FogParameters::fStart -= appMain.sof(15.0f);

		if (Keys::Key(VK_PRIOR))
			FogParameters::fEnd += appMain.sof(15.0f);
		if (Keys::Key(VK_NEXT))
			FogParameters::fEnd -= appMain.sof(15.0f);
	}
	else
	{
		if (Keys::Key(VK_ADD))
			FogParameters::fDensity += appMain.sof(0.01f);
		if (Keys::Key(VK_SUBTRACT))
			FogParameters::fDensity -= appMain.sof(0.01f);
	}

	if (Keys::Onekey('F'))
		FogParameters::iFogEquation = (FogParameters::iFogEquation + 1) % 4;



	if(Keys::Onekey(VK_ESCAPE))PostQuitMessage(0);
	fGlobalAngle += appMain.sof(1.0f);

#if USE_FREEGLUT
	glutSwapBuffers();
#else
	oglControl->SwapBuffers();

#endif
	
}


void InitSceneGLUT()
{
	InitScene(nullptr);
}

void RenderSceneGLUT()
{
	RenderScene(nullptr);
}

// Releases OpenGL scene.
// lpParam - Pointer to anything you want.
void ReleaseScene(LPVOID lpParam)
{
	FOR(i, NUMTEXTURES)tTextures[i].DeleteTexture();
	sbMainSkybox.DeleteSkybox();
	spMain.DeleteProgram();
	FOR(i, NUMSHADERS)shShaders[i].DeleteShader();

	glDeleteVertexArrays(2, uiVAOs);
	vboSceneObjects.DeleteVBO();
	vboCubeInd.DeleteVBO();
	vboCube.DeleteVBO();
}