////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

enum class Shape {
	HEXAGON,
	STAR,
	HOLETRIANGLE,
};

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass 
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
	    XMFLOAT4 color;
		// 정보는 얼마든지 더 추가될 수 있다.
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, Shape name);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*, Shape name);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	// vertexBuffer -> vertex list, indexBuffer -> order of vertex for triangle
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

};

#endif