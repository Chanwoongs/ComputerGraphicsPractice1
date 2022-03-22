////////////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_


/////////////
// LINKING //
/////////////
// 외부 Library 파일 로딩, SDK에 포함, 좋은 방법은 아님
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

//////////////
// INCLUDES //
//////////////
// 필수 DX 파일
// DirectX Grapics interface
#include <dxgi.h>
// DirectX 흔히 쓰는 함수
#include <d3dcommon.h>
// DirectX 11
#include <d3d11.h>
// DirectX 수학 관련
#include <directxmath.h>

#include "AlignedAllocationPolicy.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
// D3D안에 있는 동적할당은 모두 16비트를 사용해라. D3D data를 16비트로 끊어서 GPU에 전달 해줘야 한다. GPU는 메모리를 함부로 만질 수 없다.
// 큰 데이터를 일정한 단위로 처리하면 가속을 받을 수 있다. 그게 16비트이다.
class D3DClass : public AlignedAllocationPolicy<16>
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	// 메모리 해제
	void Shutdown();
	
	// 렌더링 시작
	void BeginScene(float, float, float, float);
	// 끝
	void EndScene();

	// GPU쪽에 있는 리소스를 CPU쪽으로 가져온다.
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	// 좌표계 변환
	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

private:
	// 기본적인 변수들
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	// IDX, ID3D는 D3D에 대한 구조체, I = interface, DX = DirectX
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