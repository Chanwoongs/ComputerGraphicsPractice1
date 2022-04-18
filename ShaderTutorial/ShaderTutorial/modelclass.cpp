////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, Shape name)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device, name);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


bool ModelClass::InitializeBuffers(ID3D11Device* device, Shape name)
{
	// Hexagon
	if (name == Shape::HEXAGON)
	{
		// LocalType -> D3D
		VertexType* vertices;
		unsigned long* indices;

		// 메모리를 잡기 위해 Description 사용
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;

		// Set the number of vertices in the vertex array.
		// vertex 갯수 설정
		m_vertexCount = 12;

		// Set the number of indices in the index array.
		// index 갯수 설정
		m_indexCount = 12;

		// Create the vertex array.
		vertices = new VertexType[m_vertexCount];
		if (!vertices)
		{
			return false;
		}

		// Create the index array.
		indices = new unsigned long[m_indexCount];
		if (!indices)
		{
			return false;
		}

		// Load the vertex array with data.
		vertices[0].position = XMFLOAT3(-2.0f, 0.0f, 0.0f);
		vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		vertices[1].position = XMFLOAT3(-1.0f, 2.0f, 0.0f);
		vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		vertices[2].position = XMFLOAT3(-1.0f, -2.0f, 0.0f);
		vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

		vertices[3].position = XMFLOAT3(-1.0f, 2.0f, 0.0f);
		vertices[3].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		vertices[4].position = XMFLOAT3(1.0f, -2.0f, 0.0f);
		vertices[4].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		vertices[5].position = XMFLOAT3(-1.0f, -2.0f, 0.0f);
		vertices[5].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

		vertices[6].position = XMFLOAT3(-1.0f, 2.0f, 0.0f);
		vertices[6].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		vertices[7].position = XMFLOAT3(1.0f, 2.0f, 0.0f);
		vertices[7].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		vertices[8].position = XMFLOAT3(1.0f, -2.0f, 0.0f);
		vertices[8].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

		vertices[9].position = XMFLOAT3(1.0f, 2.0f, 0.0f);
		vertices[9].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		vertices[10].position = XMFLOAT3(2.0f, 0.0f, 0.0f);
		vertices[10].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		vertices[11].position = XMFLOAT3(1.0f, -2.0f, 0.0f);
		vertices[11].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

		// Load the index array with data.
		// Create the triangle in the clockwise order (counterclockwise: back face culling).
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;

		indices[3] = 3;
		indices[4] = 4;
		indices[5] = 5;

		indices[6] = 6;
		indices[7] = 7;
		indices[8] = 8;

		indices[9] = 9;
		indices[10] = 10;
		indices[11] = 11;

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount; // 잡을 메모리 크기
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		// device -> 하드웨어에 올린다.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// 메모리에 정보를 올렸지만 아직 화면에 올린 것은 아니다.
		// 화면에 출력은 Shader가 담당

		// Release the arrays now that the vertex and index buffers have been created and loaded.
		// local로 만들었던 것들 삭제
		delete[] vertices;
		vertices = 0;

		delete[] indices;
		indices = 0;
	}

	// Star
	else if (name == Shape::STAR)
	{
		// LocalType -> D3D
		VertexType* vertices;
		unsigned long* indices;

		// 메모리를 잡기 위해 Description 사용
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;

		// Set the number of vertices in the vertex array.
		// vertex 갯수 설정
		m_vertexCount = 15;

		// Set the number of indices in the index array.
		// index 갯수 설정
		m_indexCount = 15;

		// Create the vertex array.
		vertices = new VertexType[m_vertexCount];
		if (!vertices)
		{
			return false;
		}

		// Create the index array.
		indices = new unsigned long[m_indexCount];
		if (!indices)
		{
			return false;
		}

		// Load the vertex array with data.
		vertices[0].position = XMFLOAT3(-0.2f, 0.1f, 0.0f);
		vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[1].position = XMFLOAT3(0.0f, 0.8f, 0.0f);
		vertices[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[2].position = XMFLOAT3(0.2f, 0.1f, 0.0f);
		vertices[2].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

		vertices[3].position = XMFLOAT3(0.2f, 0.1f, 0.0f);
		vertices[3].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[4].position = XMFLOAT3(0.9f, 0.1f, 0.0f);
		vertices[4].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[5].position = XMFLOAT3(0.3f, -0.3f, 0.0f);
		vertices[5].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

		vertices[6].position = XMFLOAT3(0.0f, -0.5f, 0.0f);
		vertices[6].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[7].position = XMFLOAT3(0.3f, -0.3f, 0.0f);
		vertices[7].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[8].position = XMFLOAT3(0.5f, -0.8f, 0.0f);
		vertices[8].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

		vertices[9].position = XMFLOAT3(-0.5f, -0.8f, 0.0f);
		vertices[9].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[10].position = XMFLOAT3(-0.3f, -0.3f, 0.0f);
		vertices[10].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[11].position = XMFLOAT3(0.0f, -0.5f, 0.0f);
		vertices[11].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

		vertices[12].position = XMFLOAT3(-0.9f, 0.1f, 0.0f);
		vertices[12].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[13].position = XMFLOAT3(-0.2, 0.1f, 0.0f);
		vertices[13].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[14].position = XMFLOAT3(-0.3f, -0.3f, 0.0f);
		vertices[14].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

		// Load the index array with data.
		// Create the triangle in the clockwise order (counterclockwise: back face culling).
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;

		indices[3] = 3;
		indices[4] = 4;
		indices[5] = 5;

		indices[6] = 6;
		indices[7] = 7;
		indices[8] = 8;

		indices[9] = 9;
		indices[10] = 10;
		indices[11] = 11;

		indices[12] = 12;
		indices[13] = 13;
		indices[14] = 14;

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount; // 잡을 메모리 크기
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		// device -> 하드웨어에 올린다.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// 메모리에 정보를 올렸지만 아직 화면에 올린 것은 아니다.
		// 화면에 출력은 Shader가 담당

		// Release the arrays now that the vertex and index buffers have been created and loaded.
		// local로 만들었던 것들 삭제
		delete[] vertices;
		vertices = 0;

		delete[] indices;
		indices = 0;
	}

	// HoleTriangle
	else if (name == Shape::HOLETRIANGLE)
	{
		// LocalType -> D3D
		VertexType* vertices;
		unsigned long* indices;

		// 메모리를 잡기 위해 Description 사용
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;

		// Set the number of vertices in the vertex array.
		// vertex 갯수 설정
		m_vertexCount = 9;

		// Set the number of indices in the index array.
		// index 갯수 설정
		m_indexCount = 9;

		// Create the vertex array.
		vertices = new VertexType[m_vertexCount];
		if (!vertices)
		{
			return false;
		}

		// Create the index array.
		indices = new unsigned long[m_indexCount];
		if (!indices)
		{
			return false;
		}

		// Load the vertex array with data.
		vertices[0].position = XMFLOAT3(-0.2f, 0.2f, 0.0f);
		vertices[0].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		vertices[1].position = XMFLOAT3(0.0f, 0.6f, 0.0f);
		vertices[1].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		vertices[2].position = XMFLOAT3(0.2f, 0.2f, 0.0f);
		vertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

		vertices[3].position = XMFLOAT3(-0.4f, -0.2f, 0.0f);
		vertices[3].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		vertices[4].position = XMFLOAT3(-0.2f, 0.2f, 0.0f);
		vertices[4].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		vertices[5].position = XMFLOAT3(0.0f, -0.2f, 0.0f);
		vertices[5].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

		vertices[6].position = XMFLOAT3(0.0f, -0.2f, 0.0f);
		vertices[6].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		vertices[7].position = XMFLOAT3(0.2f, 0.2f, 0.0f);
		vertices[7].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		vertices[8].position = XMFLOAT3(0.4f, -0.2f, 0.0f);
		vertices[8].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

		// Load the index array with data.
		// Create the triangle in the clockwise order (counterclockwise: back face culling).
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;

		indices[3] = 3;
		indices[4] = 4;
		indices[5] = 5;

		indices[6] = 6;
		indices[7] = 7;
		indices[8] = 8;

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount; // 잡을 메모리 크기
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		// device -> 하드웨어에 올린다.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// 메모리에 정보를 올렸지만 아직 화면에 올린 것은 아니다.
		// 화면에 출력은 Shader가 담당

		// Release the arrays now that the vertex and index buffers have been created and loaded.
		// local로 만들었던 것들 삭제
		delete[] vertices;
		vertices = 0;

		delete[] indices;
		indices = 0;
	}

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

// This sets the vertex buffer and index buffer as active on the input assembler in the GPU. 
// Once the GPU has an active vertex buffer, it can then use the shader to render that buffer. 
// This function also defines how those buffers should be drawn such as triangles, lines, fans, 
// and etc using the IASetPrimitiveTopology DirectX function.
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// deviceContext -> GPU에 무엇을 할 때
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	// VertexBuffer 전달
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	// IndexBuffer 전달
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	// 어떤 삼각형의 구성으로 그릴지 결정할 수 있다. ex) TRIANGLESTRIP, TRIANGELIST, LINE, POINT 
 	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}