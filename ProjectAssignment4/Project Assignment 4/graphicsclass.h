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

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;

	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	
	bool m_ambient;
	bool m_diffuse;
	bool m_specular;

	vector<ModelClass*> m_models;

	ModelClass* m_Plane;
	XMFLOAT3* m_planePosition;
	int m_planeCount;

	ModelClass* m_IronMan;
	XMFLOAT3* m_ironManPosition;
	int m_ironManCount;

	ModelClass* temp;
	XMFLOAT3* tempPos;
	int tempCount;

	
};

#endif