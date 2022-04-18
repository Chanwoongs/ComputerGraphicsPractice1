////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"


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

	void SetBgColor(float red, float green, float blue, float alpha);
	void UpdateAngle();

	int GetFillMode();
	void SetFillMode(int fillMode);
	bool UpdateFillMode();

	void SetBrightnessNum(float brightnessNum);

	void CreateRS();
private:
	bool Render();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Hexagon;
	ModelClass* m_Star;
	ModelClass* m_HoleTriangle;
	ColorShaderClass* m_ColorShader;

	ID3D11RasterizerState* m_solidModeState;
	ID3D11RasterizerState* m_wireFrameModeState;

	float m_bgColor[4];
	float m_angle;
	float m_brightnessNum;

	int m_fillMode;



};

#endif