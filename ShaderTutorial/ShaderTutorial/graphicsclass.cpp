////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Hexagon = 0;
	m_Star = 0;
	m_HoleTriangle = 0;
	m_ColorShader = 0;

	m_angle = 0.0f;
	m_fillMode = 0;
	m_brightnessNum = 1.0f;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	// 그릴 도화지 만듦
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Create the model object.
	m_Hexagon = new ModelClass;
	if (!m_Hexagon)
	{
		return false;
	}
	m_Star = new ModelClass;
	if (!m_Star)
	{
		return false;
	}
	m_HoleTriangle = new ModelClass;
	if (!m_HoleTriangle)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Hexagon->Initialize(m_D3D->GetDevice(), Shape::HEXAGON);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	result = m_Star->Initialize(m_D3D->GetDevice(), Shape::STAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	result = m_HoleTriangle->Initialize(m_D3D->GetDevice(), Shape::HOLETRIANGLE);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass; // 쉐이더 파일을 읽어들이고 컴파일해서 모델 클래스에서 준 정보를 변형
	if(!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the color shader object.
	if(m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if(m_Hexagon)
	{
		m_Hexagon->Shutdown();
		delete m_Hexagon;
		m_Hexagon = 0;
	}
	if (m_Star)
	{
		m_Star->Shutdown();
		delete m_Star;
		m_Star = 0;
	}
	if (m_HoleTriangle)
	{
		m_HoleTriangle->Shutdown();
		delete m_HoleTriangle;
		m_HoleTriangle = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;
	UpdateAngle();

	result = UpdateFillMode();
	if (!result)
	{
		return false;
	}

	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

void GraphicsClass::UpdateAngle()
{
	m_angle += 0.01f;
}

int GraphicsClass::GetFillMode()
{
	return m_fillMode;
}

void GraphicsClass::SetFillMode(int fillMode)
{
	this->m_fillMode = fillMode;
}

bool GraphicsClass::UpdateFillMode()
{
	HRESULT result;
	D3D11_RASTERIZER_DESC rasterDesc;

	if (m_fillMode == 0)
	{
		// Setup the raster description which will determine how and what polygons will be drawn.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = m_D3D->GetDevice()->CreateRasterizerState(&rasterDesc, &m_solidModeState);
		if (FAILED(result))
		{
			return false;
		}

		// Now set the rasterizer state.
		m_D3D->GetDeviceContext()->RSSetState(m_solidModeState);
	}
	else if (m_fillMode == 1)
	{
		// Setup the raster description which will determine how and what polygons will be drawn.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = m_D3D->GetDevice()->CreateRasterizerState(&rasterDesc, &m_wireFrameModeState);
		if (FAILED(result))
		{
			return false;
		}

		// Now set the rasterizer state.
		m_D3D->GetDeviceContext()->RSSetState(m_wireFrameModeState);
	}

	return true;
}

void GraphicsClass::SetBrightnessNum(float brightnessNum)
{
	this->m_brightnessNum = brightnessNum;
}


void GraphicsClass::SetBgColor(float red, float green, float blue, float alpha)
{
	this->m_bgColor[0] = red;
	this->m_bgColor[1] = green;
	this->m_bgColor[2] = blue;
	this->m_bgColor[3] = alpha;
}


bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Clear the buffers to begin the scene. Background color
	m_D3D->BeginScene(m_bgColor[0], m_bgColor[1], m_bgColor[2], m_bgColor[3]);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Hexagon->Render(m_D3D->GetDeviceContext());
	// Render the model using the color shader.
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Hexagon->GetIndexCount(),
		worldMatrix * XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), m_angle) * XMMatrixTranslation(-3.0f, 0.0f, 0.0f), viewMatrix, projectionMatrix, m_brightnessNum);
	if (!result)
	{
		return false;
	}

	m_Star->Render(m_D3D->GetDeviceContext());
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Star->GetIndexCount(),
		worldMatrix * XMMatrixRotationAxis(XMVectorSet(1, 0, 0, 0), m_angle), viewMatrix, projectionMatrix, m_brightnessNum);
	if (!result)
	{
		return false;
	}

	m_HoleTriangle->Render(m_D3D->GetDeviceContext());
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_HoleTriangle->GetIndexCount(),
		worldMatrix * XMMatrixRotationAxis(XMVectorSet(0, 0, 1, 0), m_angle) * XMMatrixTranslation(3.0f, 0.0f, 0.0f), viewMatrix, projectionMatrix, m_brightnessNum);
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}