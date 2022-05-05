////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Giant = 0;
	m_Warrior = 0;
	m_Man = 0;
	m_Plane = 0;
	m_TextureShader = 0;
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
	m_Camera->SetPosition(0.0f, 10.0f, -100.0f);	
	
	// Create the model object.
	m_Giant = new ModelClass;
	if(!m_Giant)
	{
		return false;
	}
	m_Warrior = new ModelClass;
	if (!m_Warrior)
	{
		return false;
	}
	m_Man = new ModelClass;
	if (!m_Man)
	{
		return false;
	}
	m_Plane = new ModelClass;
	if (!m_Plane)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Giant->Initialize(m_D3D->GetDevice(), L"./data/GiantAlien.obj", L"./data/GiantBaseColor.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	result = m_Warrior->Initialize(m_D3D->GetDevice(), L"./data/AlienWarrior.obj", L"./data/WarriorBaseColor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	result = m_Man->Initialize(m_D3D->GetDevice(), L"./data/Man.obj", L"./data/ManBaseColor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	result = m_Plane->Initialize(m_D3D->GetDevice(), L"./data/Cube.obj", L"./data/Plane.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model object.
	if(m_Giant)
	{
		m_Giant->Shutdown();
		delete m_Giant;
		m_Giant = 0;
	}
	if (m_Warrior)
	{
		m_Warrior->Shutdown();
		delete m_Warrior;
		m_Warrior = 0;
	}
	if (m_Man)
	{
		m_Man->Shutdown();
		delete m_Man;
		m_Man = 0;
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

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.001f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}

void GraphicsClass::UpdateFilter(int modeNum)
{
	m_TextureShader->UpdateFilter(m_D3D->GetDevice(), modeNum);
}

bool GraphicsClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	worldMatrix = XMMatrixRotationY(rotation);

	m_TextureShader->SetNumOfTextureTiles(1);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Giant->Render(m_D3D->GetDeviceContext());
	// Render the model using the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Giant->GetIndexCount(),
		worldMatrix * XMMatrixScaling(1.0f, 1.0f, 1.0f) * XMMatrixTranslation(0, -10.0f, 0), viewMatrix, projectionMatrix, m_Giant->GetTexture());
	if (!result)
	{
		return false;
	}

	m_Warrior->Render(m_D3D->GetDeviceContext());
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Warrior->GetIndexCount(),
		worldMatrix * XMMatrixScaling(0.05f, 0.05f, 0.05f) * XMMatrixTranslation(-50.0f, 10.0f, 20.0f), viewMatrix, projectionMatrix, m_Warrior->GetTexture());
	if (!result)
	{
		return false;
	}

	m_Man->Render(m_D3D->GetDeviceContext());	
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Man->GetIndexCount(),
		worldMatrix * XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixTranslation(50.0f, -10.0f, 20.0f), viewMatrix, projectionMatrix, m_Man->GetTexture());
	if (!result)
	{
		return false;
	}

	worldMatrix = XMMatrixRotationY(0);
	m_TextureShader->SetNumOfTextureTiles(15);
	m_Plane->Render(m_D3D->GetDeviceContext());
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Plane->GetIndexCount(),
		worldMatrix * XMMatrixScaling(800.0f, 1000.0f, 1.0f) * XMMatrixRotationAxis(XMVectorSet(1, 0, 0, 0), -90.0f) * XMMatrixTranslation(0, 0, 50.0f), viewMatrix, projectionMatrix, m_Plane->GetTexture());
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}