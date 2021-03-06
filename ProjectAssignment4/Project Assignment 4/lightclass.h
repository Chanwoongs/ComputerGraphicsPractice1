////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetPosition(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);
	void SetToggleAmbient(float);
	void SetToggleDiffuse(float);
	void SetToggleSpecular(float);
	void SetToggleFog(float);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();
	XMFLOAT4 GetPosition();
	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();
	float GetAmbientToggle();
	float GetDiffuseToggle();
	float GetSpecularToggle();
	float GetFogToggle();

private:
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;
	XMFLOAT4 m_position;
	XMFLOAT4 m_specularColor;
	float m_specularPower;
	float m_ambientToggle;
	float m_diffuseToggle;
	float m_specularToggle;
	float m_fogToggle;
};

#endif