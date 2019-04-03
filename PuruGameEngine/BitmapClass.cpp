#include "BitmapClass.h"
#include "PuruGameSystem.h"
#include "GraphicsClass.h"

BitmapClass::BitmapClass() : m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_Texture(nullptr), m_texLeft(0.0f), m_texTop(0.0f), m_texRight(1.0f), m_texBottom(1.0f)
{
}

BitmapClass::BitmapClass(const BitmapClass &other)
{
	m_Texture = other.m_Texture;
	m_texLeft = other.m_texLeft;
	m_texRight = other.m_texRight;
	m_texTop = other.m_texTop;
	m_texBottom = other.m_texBottom;
	m_bitmapWidth = other.m_bitmapWidth;
	m_bitmapHeight = other.m_bitmapHeight;
	m_screenWidth = other.m_screenWidth;
	m_screenHeight = other.m_screenHeight;

	PGE* pge = PGE::GetInstance();
	GraphicsClass* gfx = pge->GetGraphics();

	InitializeBuffers(gfx->GetDevice());
}


BitmapClass::~BitmapClass()
{
}

BitmapClass & BitmapClass::operator=(const BitmapClass & other)
{
	Shutdown();
	
	m_Texture = other.m_Texture;
	m_texLeft = other.m_texLeft;
	m_texRight = other.m_texRight;
	m_texTop = other.m_texTop;
	m_texBottom = other.m_texBottom;
	m_bitmapWidth = other.m_bitmapWidth;
	m_bitmapHeight = other.m_bitmapHeight;
	m_screenWidth = other.m_screenWidth;
	m_screenHeight = other.m_screenHeight;

	PGE* pge = PGE::GetInstance();
	GraphicsClass* gfx = pge->GetGraphics();

	InitializeBuffers(gfx->GetDevice());

	return *this;
}

bool BitmapClass::Initialize(ID3D11Device * device, int screenWidth, int screenHeight, puruTexture * texture, int bitmapWidth, int bitmapHeight)
{
	bool result;

	//Store the screen size
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//Store the size in pixels that this bitmap should be rendered at
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	//The previous rendering position is initialized to -1
	//this is important because if the location has not 
	//changed, we will not modify the vertex buffer
	m_previousPosX = m_previousPosY = -1;

	//initialize the vertex and index buffers
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	//load texture for the model
	result = LoadTexture(texture);
	if (!result)
	{
		return false;
	}


	return true;
}

void BitmapClass::Shutdown()
{

	ShutdownBuffers();
}

bool BitmapClass::Render(ID3D11DeviceContext *deviceContext, int positionX, int positionY, float scale)
{
	bool result;

	//Re build the dynamic vertex buffer for rendering to possibly a different location on screen
	result = UpdateBuffers(deviceContext, positionX, positionY, scale);
	if (!result)
	{
		return false;
	}

	//Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	RenderBuffers(deviceContext);

	return true;
}

int BitmapClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView * BitmapClass::GetTexture()
{
	return m_Texture->GetTexture();
}

puruTexture * BitmapClass::GetPTexture()
{
	return m_Texture;
}

bool BitmapClass::InitializeBuffers(ID3D11Device *device)
{
	VertexType* vertices;
	unsigned long *indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//Set the number of vertices
	m_vertexCount = 4;

	//Set the number of indices
	m_indexCount = 6;

	//Create the vertex array;
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	//Initialize vertex array to zeros
	memset(vertices, 0, (sizeof(VertexType)*m_vertexCount));

	//Load the index array with data
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 1;

	//Now creating a dynamic vertex buffer. To make it dynamic
	//we set usage to d3d11_usage_dynamic and cpuaccessflags to
	//d3d11_cpu_access_write

	//Set up desc of dynamic buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)*m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the sub resource data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Now create vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Set up desc of indices buffer. this does not have to be
	//dynamic since the indices always points to the same 
	//vertices

	//Set up desc of index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)*m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give sub resource data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Release the arrays now that the vertex and index buffers have been created and loaded
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void BitmapClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext *deviceContext, int positionX, int positionY, float scale)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	//We check if the position to render the image has changed. If it hasn't changed then we just
	//exit since the vertex buffer doesn't need any processing

	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}

	//If it has changed then we update our positions
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	//Calculating the 4 sides of the image
	left = (float)((m_screenWidth / 2)*-1) + (float)positionX;
	right = left + ((float)m_bitmapWidth*scale);
	top = (float)(m_screenHeight / 2) - (float)positionY;
	bottom = top - ((float)m_bitmapHeight*scale);

	//Now we create a temporary vertex array and fill it with new vertex points

	//Create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}
	//Load the vertex array with data
	// First triangle.
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[0].texture = D3DXVECTOR2(m_texLeft, m_texTop);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = D3DXVECTOR2(m_texRight, m_texBottom);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = D3DXVECTOR2(m_texLeft, m_texBottom);

	vertices[3].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	vertices[3].texture = D3DXVECTOR2(m_texRight, m_texTop);


	//Now copy the contents of vertex array to the vertex buffer using Map and memcpy

	//Lock the vertex buffer
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//Get a pointer to data in vertex buffer
	verticesPtr = (VertexType*)mappedResource.pData;

	//Copy the data into the vertex buffer
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)*m_vertexCount));

	//Unlock the vertex buffer
	deviceContext->Unmap(m_vertexBuffer, 0);

	//Release the vertex array
	delete[] vertices;
	vertices = nullptr;

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//Set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	//Set vertex buffer to active in input assembler 
	//so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//Set the index buffer to active
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the topology of primitive that should be rendered
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool BitmapClass::LoadTexture(puruTexture * texture)
{
	m_Texture = texture;
	if (!m_Texture)
	{
		return false;
	}
	return true;
}

void BitmapClass::CalculateTextureLocations(float left, float top)
{
	m_texLeft = left;
	m_texTop = top;
	m_texRight = m_texLeft + ((float)m_bitmapWidth / (float)(m_Texture->GetWidth()));
	m_texBottom = m_texTop + ((float)m_bitmapHeight / (float)(m_Texture->GetHeight()));
}
