////////////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_


/////////////
// LINKING //
/////////////
// �ܺ� Library ���� �ε�, SDK�� ����, ���� ����� �ƴ�
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

//////////////
// INCLUDES //
//////////////
// �ʼ� DX ����
// DirectX Grapics interface
#include <dxgi.h>
// DirectX ���� ���� �Լ�
#include <d3dcommon.h>
// DirectX 11
#include <d3d11.h>
// DirectX ���� ����
#include <directxmath.h>

#include "AlignedAllocationPolicy.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
// D3D�ȿ� �ִ� �����Ҵ��� ��� 16��Ʈ�� ����ض�. D3D data�� 16��Ʈ�� ��� GPU�� ���� ����� �Ѵ�. GPU�� �޸𸮸� �Ժη� ���� �� ����.
// ū �����͸� ������ ������ ó���ϸ� ������ ���� �� �ִ�. �װ� 16��Ʈ�̴�.
class D3DClass : public AlignedAllocationPolicy<16>
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	// �޸� ����
	void Shutdown();
	
	// ������ ����
	void BeginScene(float, float, float, float);
	// ��
	void EndScene();

	// GPU�ʿ� �ִ� ���ҽ��� CPU������ �����´�.
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	// ��ǥ�� ��ȯ
	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

private:
	// �⺻���� ������
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	// IDX, ID3D�� D3D�� ���� ����ü, I = interface, DX = DirectX
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;

	// XM = DirectX Math
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
};

#endif