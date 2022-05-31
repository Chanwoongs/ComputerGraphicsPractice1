////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_Light = 0;

	m_ambient = true;
	m_diffuse = true;
	m_specular = true;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::InitializeModels(HWND hwnd)
{
	bool result;

	// Plane
	m_models.push_back(m_Plane);
	if (!m_models.at(0).model)
	{
		return false;
	}
	result = m_models.at(0).model->Initialize(m_D3D->GetDevice(), L"./data/cube.obj", L"./data/Plane.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Iron Man
	m_models.push_back(m_IronMan);
	if (!m_models.at(1).model)
	{
		return false;
	}
	result = m_models.at(1).model->Initialize(m_D3D->GetDevice(), L"./data/ironMan.obj", L"./data/ironMan.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Temp
	m_models.push_back(temp);
	if (!m_models.at(2).model)
	{
		return false;
	}
	result = m_models.at(2).model->Initialize(m_D3D->GetDevice(), L"./data/Hulk.obj", L"./data/Hulk.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	return true;
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
	m_Camera->SetPosition(0.0f, 2.0f, -5.0f);	// for cube
//	m_Camera->SetPosition(0.0f, 0.5f, -3.0f);	// for chair

	SetModelsInfo();
	InitializeModels(hwnd);

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}


	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -0.5f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	return true;
}

void GraphicsClass::toggleAmbient()
{
	if (m_Light->GetAmbientToggle() == 0.0f)
	{
		m_Light->SetToggleAmbient(1.0f);
	}
	else
	{
		m_Light->SetToggleAmbient(0.0f);
	}
}

void GraphicsClass::toggleDiffuse()
{
	if (m_Light->GetDiffuseToggle() == 0.0f)
	{
		m_Light->SetToggleDiffuse(1.0f);
	}
	else
	{
		m_Light->SetToggleDiffuse(0.0f);
	}
}

void GraphicsClass::toggleSpecular()
{
	if (m_Light->GetSpecularToggle() == 0.0f)
	{
		m_Light->SetToggleSpecular(1.0f);
	}
	else
	{
		m_Light->SetToggleSpecular(0.0f);
	}
}

CameraClass* GraphicsClass::GetCamera()
{
	return m_Camera;
}

void GraphicsClass::Shutdown()
{
	// Release the model object.
	for (auto object : m_models)
	{
		if (object.model)
		{
			object.model->Shutdown();
			delete object.model;
			object.model = 0;

			delete[] object.positions;
			object.positions = 0;
		}
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

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	
	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += XM_PI * 0.005f;
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

	for (auto object : m_models)
	{
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		object.model->Render(m_D3D->GetDeviceContext());
		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), object.model->GetVertexCount(), object.model->GetInstanceCount(),
			worldMatrix * XMMatrixScaling(object.scale.x, object.scale.y, object.scale.z), viewMatrix, projectionMatrix,
			object.model->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
			m_Light->GetAmbientToggle(), m_Light->GetDiffuseToggle(), m_Light->GetSpecularToggle());
		if (!result)
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::SetModelsInfo()
{
	m_Plane.instanceCount = 1;
	m_Plane.positions = new XMFLOAT3[m_Plane.instanceCount];
	m_Plane.positions[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Plane.scale = XMFLOAT3(400.0f, 0.1f, 1000.0f);
	m_Plane.model = new ModelClass(m_Plane.positions, m_Plane.instanceCount);

	m_IronMan.instanceCount = 1;
	m_IronMan.positions = new XMFLOAT3[m_IronMan.instanceCount];
	m_IronMan.positions[0] = XMFLOAT3(0.0f, 0.0f, 2.0f);
	m_IronMan.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_IronMan.model = new ModelClass(m_IronMan.positions, m_IronMan.instanceCount);

	temp.instanceCount = 1;
	temp.positions = new XMFLOAT3[temp.instanceCount];
	temp.positions[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	temp.scale = XMFLOAT3(0.5f, 0.5f, 0.5f);
	temp.model = new ModelClass(temp.positions, temp.instanceCount);
}