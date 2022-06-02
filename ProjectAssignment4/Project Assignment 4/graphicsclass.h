////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <vector>
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "fogshaderclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
struct Model {
	ModelClass* model;
	XMFLOAT3* positions;
	XMFLOAT3 scale;
	int instanceCount;
	float rotY;
};

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

	void toggleAmbient();
	void toggleDiffuse();
	void toggleSpecular();

	CameraClass* GetCamera();

private:
	bool Render(float);
	void SetModelsInfo();
	bool InitializeModels(HWND);
	XMFLOAT3 divideXMF3(XMFLOAT3, XMFLOAT3);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;

	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	FogShaderClass* m_FogShader;

	LightClass* m_Light1;
	
	bool m_ambient;
	bool m_diffuse;
	bool m_specular;

	vector<Model> m_models;

	// Plane
	Model m_Plane;

	// Single Hero
	Model m_IronMan;
	Model m_Katarina;
	Model m_Deadpool;
	Model m_Spiderman;
	Model m_Spongebob;
	Model m_Patrick;
	Model m_Robocop;
	Model m_Hulk;

	// Multiple Heroes
	Model m_Tank;
	Model m_GiantRobot;
	Model m_HulkBuster;
	Model m_RifleSoldier;

	// Single Villain
	Model m_Diablo;
	Model m_Smoker;
	Model m_AbominationHulk;
	Model m_Venom;
	Model m_Xuchilbara;

	// Multiple Villains
	Model m_HeavyRobot;
	Model m_Librarian;
	Model m_GiantAlien;
	Model m_AlienWarrior;

	// Environments
	Model m_OldHouseN;
	Model m_OldHouseS;
	Model m_OldHouseE;
	Model m_OldHouseW;
	Model m_TrafficSignN;
	Model m_TrafficSignE;
	Model m_DamagedDockN;
	Model m_DamagedDockE;
	Model m_DestroyedTank;
	Model m_DestroyedHouse;
	Model m_Spaceship;
	Model m_Hospital;
	Model m_Building;

	Model m_cube;
};

#endif