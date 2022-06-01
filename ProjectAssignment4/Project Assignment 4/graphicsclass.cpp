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
	m_Camera->SetPosition(0.0f, 2.0f, -5.0f);	

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

	auto tempWorldMatrix = worldMatrix;

	for (auto object : m_models)
	{
		tempWorldMatrix = worldMatrix;

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		object.model->Render(m_D3D->GetDeviceContext());
		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), object.model->GetVertexCount(), object.model->GetInstanceCount(),
			tempWorldMatrix * XMMatrixScaling(object.scale.x, object.scale.y, object.scale.z)
			, viewMatrix, projectionMatrix,
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

	// Single Heroes
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
	// Katarina
	m_models.push_back(m_Katarina);
	if (!m_models.at(2).model)
	{
		return false;
	}
	result = m_models.at(2).model->Initialize(m_D3D->GetDevice(), L"./data/Katarina.obj", L"./data/Katarina.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Deadpool
	m_models.push_back(m_Deadpool);
	if (!m_models.at(3).model)
	{
		return false;
	}
	result = m_models.at(3).model->Initialize(m_D3D->GetDevice(), L"./data/Deadpool.obj", L"./data/Deadpool.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Spiderman
	m_models.push_back(m_Spiderman);
	if (!m_models.at(4).model)
	{
		return false;
	}
	result = m_models.at(4).model->Initialize(m_D3D->GetDevice(), L"./data/Spiderman.obj", L"./data/Spiderman.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Spongebob
	m_models.push_back(m_Spongebob);
	if (!m_models.at(5).model)
	{
		return false;
	}
	result = m_models.at(5).model->Initialize(m_D3D->GetDevice(), L"./data/Spongebob.obj", L"./data/Spongebob.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Patrick
	m_models.push_back(m_Patrick);
	if (!m_models.at(6).model)
	{
		return false;
	}
	result = m_models.at(6).model->Initialize(m_D3D->GetDevice(), L"./data/Patrick.obj", L"./data/Patrick.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Robocop
	m_models.push_back(m_Robocop);
	if (!m_models.at(7).model)
	{
		return false;
	}
	result = m_models.at(7).model->Initialize(m_D3D->GetDevice(), L"./data/Robocop.obj", L"./data/Robocop.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Hulk
	m_models.push_back(m_Hulk);
	if (!m_models.at(8).model)
	{
		return false;
	}
	result = m_models.at(8).model->Initialize(m_D3D->GetDevice(), L"./data/Hulk.obj", L"./data/Hulk.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Multiple Heroes
	// Tank
	m_models.push_back(m_Tank);
	if (!m_models.at(9).model)
	{
		return false;
	}
	result = m_models.at(9).model->Initialize(m_D3D->GetDevice(), L"./data/Tank.obj", L"./data/TankSnow.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// GiantRobot
	m_models.push_back(m_GiantRobot);
	if (!m_models.at(10).model)
	{
		return false;
	}
	result = m_models.at(10).model->Initialize(m_D3D->GetDevice(), L"./data/GiantRobot.obj", L"./data/GiantRobot.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// HulkBuster
	m_models.push_back(m_HulkBuster);
	if (!m_models.at(11).model)
	{
		return false;
	}
	result = m_models.at(11).model->Initialize(m_D3D->GetDevice(), L"./data/HulkBuster.obj", L"./data/HulkBuster.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// RifleSoldier
	m_models.push_back(m_RifleSoldier);
	if (!m_models.at(12).model)
	{
		return false;
	}
	result = m_models.at(12).model->Initialize(m_D3D->GetDevice(), L"./data/RifleSoldier.obj", L"./data/RifleSoldier.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Single Villain
	// Diablo
	m_models.push_back(m_Diablo);
	if (!m_models.at(13).model)
	{
		return false;
	}
	result = m_models.at(13).model->Initialize(m_D3D->GetDevice(), L"./data/Diablo.obj", L"./data/Diablo.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Smoker
	m_models.push_back(m_Smoker);
	if (!m_models.at(14).model)
	{
		return false;
	}
	result = m_models.at(14).model->Initialize(m_D3D->GetDevice(), L"./data/Smoker.obj", L"./data/Smoker.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// AbominationHulk
	m_models.push_back(m_AbominationHulk);
	if (!m_models.at(15).model)
	{
		return false;
	}
	result = m_models.at(15).model->Initialize(m_D3D->GetDevice(), L"./data/AbominationHulk.obj", L"./data/AbominationHulk.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Venom
	m_models.push_back(m_Venom);
	if (!m_models.at(16).model)
	{
		return false;
	}
	result = m_models.at(16).model->Initialize(m_D3D->GetDevice(), L"./data/Venom.obj", L"./data/Venom.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Xuchilbara
	m_models.push_back(m_Xuchilbara);
	if (!m_models.at(17).model)
	{
		return false;
	}
	result = m_models.at(17).model->Initialize(m_D3D->GetDevice(), L"./data/Xuchilbara.obj", L"./data/Xuchilbara.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	return true;
}

XMFLOAT3 GraphicsClass::divideXMF3(XMFLOAT3 a, XMFLOAT3 b)
{
	XMFLOAT3 temp;
	temp.x = a.x / b.x;
	temp.y = a.y / b.y;
	temp.z = a.z / b.z;

	return temp;
}

void GraphicsClass::SetModelsInfo()
{
	m_Plane.instanceCount = 4000;
	m_Plane.scale = XMFLOAT3(10.0f, 0.1f, 10.0f);
	m_Plane.positions = new XMFLOAT3[m_Plane.instanceCount];
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			m_Plane.positions[100 * i + j] = divideXMF3(XMFLOAT3(-195.0f + (10.0f * i), 0.0f, 495.0f - (10.0f * j)), m_Plane.scale);
		}
	}
	m_Plane.model = new ModelClass(m_Plane.positions, m_Plane.instanceCount, 0.0f);

	// Single Hero
	m_IronMan.instanceCount = 1;
	m_IronMan.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_IronMan.positions = new XMFLOAT3[m_IronMan.instanceCount];
	m_IronMan.positions[0] = divideXMF3(XMFLOAT3(0.0f, 0.0f, 31.0f), m_IronMan.scale);
	m_IronMan.model = new ModelClass(m_IronMan.positions, m_IronMan.instanceCount, 0.0f);

	m_Katarina.instanceCount = 1;
	m_Katarina.scale = XMFLOAT3(0.3f, 0.3f, 0.3f);
	m_Katarina.positions = new XMFLOAT3[m_Katarina.instanceCount];
	m_Katarina.positions[0] = divideXMF3(XMFLOAT3(2.0f, 0.0f, 32.0f), m_Katarina.scale);
	m_Katarina.model = new ModelClass(m_Katarina.positions, m_Katarina.instanceCount, 0.0f);

	m_Deadpool.instanceCount = 1;
	m_Deadpool.scale = XMFLOAT3(0.01f, 0.01f, 0.01f);
	m_Deadpool.positions = new XMFLOAT3[m_Deadpool.instanceCount];
	m_Deadpool.positions[0] = divideXMF3(XMFLOAT3(-4.0f, 0.0f, 32.0f), m_Deadpool.scale);
	m_Deadpool.model = new ModelClass(m_Deadpool.positions, m_Deadpool.instanceCount, XM_PI / 2);

	m_Spiderman.instanceCount = 1;
	m_Spiderman.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Spiderman.positions = new XMFLOAT3[m_Spiderman.instanceCount];
	m_Spiderman.positions[0] = divideXMF3(XMFLOAT3(-2.0f, 0.0f, 32.0f), m_Spiderman.scale);
	m_Spiderman.model = new ModelClass(m_Spiderman.positions, m_Spiderman.instanceCount, XM_PI);

	m_Spongebob.instanceCount = 1;
	m_Spongebob.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Spongebob.positions = new XMFLOAT3[m_Spongebob.instanceCount];
	m_Spongebob.positions[0] = divideXMF3(XMFLOAT3(-6.0f, 0.0f, 33.0f), m_Spongebob.scale);
	m_Spongebob.model = new ModelClass(m_Spongebob.positions, m_Spongebob.instanceCount, 0.0f);

	m_Patrick.instanceCount = 1;
	m_Patrick.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Patrick.positions = new XMFLOAT3[m_Patrick.instanceCount];
	m_Patrick.positions[0] = divideXMF3(XMFLOAT3(6.0f, 0.0f, 33.0f), m_Patrick.scale);
	m_Patrick.model = new ModelClass(m_Patrick.positions, m_Patrick.instanceCount, 0.0f);

	m_Robocop.instanceCount = 1;
	m_Robocop.scale = XMFLOAT3(0.5f, 0.5f, 0.5f);
	m_Robocop.positions = new XMFLOAT3[m_Robocop.instanceCount];
	m_Robocop.positions[0] = divideXMF3(XMFLOAT3(4.0f, 0.0f, 32.0f), m_Robocop.scale);
	m_Robocop.model = new ModelClass(m_Robocop.positions, m_Robocop.instanceCount, 0.0f);

	m_Hulk.instanceCount = 1;
	m_Hulk.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Hulk.positions = new XMFLOAT3[m_Hulk.instanceCount];
	m_Hulk.positions[0] = divideXMF3(XMFLOAT3(0.0f, 0.0f, 35.0f), m_Hulk.scale);
	m_Hulk.model = new ModelClass(m_Hulk.positions, m_Hulk.instanceCount, 0.0f);

	// Multiple Heroes
	m_Tank.instanceCount = 108;
	m_Tank.scale = XMFLOAT3(0.005f, 0.005f, 0.005f);
	m_Tank.positions = new XMFLOAT3[m_Tank.instanceCount];
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_Tank.positions[6 * i + j] = divideXMF3(XMFLOAT3(-181.0f + (10.0f * i), 0.0f, 197.0f - (20.0f * j)), m_Tank.scale);
		}
	}
	m_Tank.model = new ModelClass(m_Tank.positions, m_Tank.instanceCount, XM_PI / 2);

	m_GiantRobot.instanceCount = 72;
	m_GiantRobot.scale = XMFLOAT3(10.0f, 10.0f, 10.0f);
	m_GiantRobot.positions = new XMFLOAT3[m_GiantRobot.instanceCount];
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_GiantRobot.positions[6 * i + j] = divideXMF3(XMFLOAT3(19.5f + (15.0f * i), 0.0f, 193.0f - (19.0f * j)), m_GiantRobot.scale);
		}
	}
	m_GiantRobot.model = new ModelClass(m_GiantRobot.positions, m_GiantRobot.instanceCount, 0.0f);

	m_HulkBuster.instanceCount = 69;
	m_HulkBuster.scale = XMFLOAT3(1.5f, 1.5f, 1.5f);
	m_HulkBuster.positions = new XMFLOAT3[m_HulkBuster.instanceCount];
	for (int i = 0; i < 23; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_HulkBuster.positions[3 * i + j] = divideXMF3(XMFLOAT3(-110.0f + (10.0f * i), 0.0f, 71.0f - (10.0f * j)), m_HulkBuster.scale);
		}
	}
	m_HulkBuster.model = new ModelClass(m_HulkBuster.positions, m_HulkBuster.instanceCount, 0.0f);

	m_RifleSoldier.instanceCount = 288;
	m_RifleSoldier.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_RifleSoldier.positions = new XMFLOAT3[m_RifleSoldier.instanceCount];
	for (int i = 0; i < 72; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_RifleSoldier.positions[2 * i + j] = divideXMF3(XMFLOAT3(-79.0f + (1.0f * i), 0.0f, 40.0f - (5.0f * j)), m_RifleSoldier.scale);
		}
	}
	for (int i = 0; i < 72; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_RifleSoldier.positions[2 * i + j + 144] = divideXMF3(XMFLOAT3(8.0f + (1.0f * i), 0.0f, 40.0f - (5.0f * j)), m_RifleSoldier.scale);
		}
	}
	m_RifleSoldier.model = new ModelClass(m_RifleSoldier.positions, m_RifleSoldier.instanceCount, 0.0f);
	
	// Single Villain
	m_Diablo.instanceCount = 1;
	m_Diablo.scale = XMFLOAT3(1.5f, 1.5f, 1.5f);
	m_Diablo.positions = new XMFLOAT3[m_Diablo.instanceCount];
	m_Diablo.positions[0] = divideXMF3(XMFLOAT3(0.0f, 0.0f, -52.0f), m_Diablo.scale);
	m_Diablo.model = new ModelClass(m_Diablo.positions, m_Diablo.instanceCount, XM_PI);

	m_Smoker.instanceCount = 1;
	m_Smoker.scale = XMFLOAT3(0.08f, 0.08f, 0.08f);
	m_Smoker.positions = new XMFLOAT3[m_Smoker.instanceCount];
	m_Smoker.positions[0] = divideXMF3(XMFLOAT3(20.0f, 0.0f, -60.0f), m_Smoker.scale);
	m_Smoker.model = new ModelClass(m_Smoker.positions, m_Smoker.instanceCount, XM_PI);

	m_AbominationHulk.instanceCount = 1;
	m_AbominationHulk.scale = XMFLOAT3(0.01f, 0.01f, 0.01f);
	m_AbominationHulk.positions = new XMFLOAT3[m_AbominationHulk.instanceCount];
	m_AbominationHulk.positions[0] = divideXMF3(XMFLOAT3(8.0f, 0.0f, -55.0f), m_AbominationHulk.scale);
	m_AbominationHulk.model = new ModelClass(m_AbominationHulk.positions, m_AbominationHulk.instanceCount, XM_PI);

	m_Venom.instanceCount = 1;
	m_Venom.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Venom.positions = new XMFLOAT3[m_Venom.instanceCount];
	m_Venom.positions[0] = divideXMF3(XMFLOAT3(-8.0f, 0.0f, -55.0f), m_Venom.scale);
	m_Venom.model = new ModelClass(m_Venom.positions, m_Venom.instanceCount, XM_PI);

	m_Xuchilbara.instanceCount = 1;
	m_Xuchilbara.scale = XMFLOAT3(0.05f, 0.05f, 0.05f);
	m_Xuchilbara.positions = new XMFLOAT3[m_Xuchilbara.instanceCount];
	m_Xuchilbara.positions[0] = divideXMF3(XMFLOAT3(-20.0f, 0.0f, -60.0f), m_Xuchilbara.scale);
	m_Xuchilbara.model = new ModelClass(m_Xuchilbara.positions, m_Xuchilbara.instanceCount, XM_PI);
}
