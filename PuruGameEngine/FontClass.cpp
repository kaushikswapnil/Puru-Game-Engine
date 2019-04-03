#include "FontClass.h"



FontClass::FontClass() : m_Font(nullptr), m_Texture(nullptr)
{
}

FontClass::FontClass(const FontClass &)
{
}


FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename)
{
	bool result;


	// Load in the text file containing the font data.
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	ReleaseTexture();

	ReleaseFontData();
}

ID3D11ShaderResourceView * FontClass::GetTexture()
{
	return m_Texture->GetTexture();
}

//BuildVertexArray will be called by the TextClass to build vertex buffers out of the text sentences it sends 
//to this function as input. This way each sentence in the TextClass that needs to be drawn has its
//own vertex buffer that can be rendered easily after being created. The vertices input is the pointer to the
//vertex array that will be returned to the TextClass once it is built. The sentence input is the text
//Sentence that will be used to create the vertex array
void FontClass::BuildVertexArray(void *vertices, char *sentence, float drawX, float drawY, float& width, float& height)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;

	width = height = 0;

	//This loop builds the vertex and index arrays. It takes each character from the sentence
	//and creates two triangles for it. It then maps the character from the font texture onto those two triangles
	//using the m_Font array
	// Draw each letter onto a quad.
	// Draw each letter onto a quad.
	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// First triangle in quad.
		vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
		vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].texLeft, m_Font[letter].texTop);
		index++;

		vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].width), (drawY - m_Font[letter].height), 0.0f);  // Bottom right.
		vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].texRight, m_Font[letter].texBottom);
		index++;

		vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - m_Font[letter].height), 0.0f);  // Bottom left.
		vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].texLeft, m_Font[letter].texBottom);
		index++;

			// Second triangle in quad.
		vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
		vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].texLeft, m_Font[letter].texTop);
		index++;

		vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].width, drawY, 0.0f);  // Top right.
		vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].texRight, m_Font[letter].texTop);
		index++;

		vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].width), (drawY - m_Font[letter].height), 0.0f);  // Bottom right.
		vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].texRight, m_Font[letter].texBottom);
		index++;

		// Update the x location for drawing by the size of the letter and one pixel.
		drawX = drawX + m_Font[letter].width + 1.0f;
		width += m_Font[letter].width + 1.0f;
		height = m_Font[letter].height;
	}

	return;
}

bool FontClass::LoadFontData(char *filename)
{
	std::ifstream fin;
	int i;
	char temp;

	//First we create an array  of FontType structure
	//The size of the array is set to 95 as that is the number
	//of characters in the texture and hence the number of
	//indexes in the fontdata.txt

	//Create the font spacing buffer
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	//Read in the font size and spacing between chars
	fin.open(filename, std::ios::in);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i < 95; i++)
	{
		fin >> temp;

		fin >> m_Font[i].texLeft;
		fin >> m_Font[i].texTop;
		fin >> m_Font[i].width;
		fin >> m_Font[i].height;
		m_Font[i].texRight = m_Font[i].texLeft + ((float)m_Font[i].width / 256);
		m_Font[i].texBottom = m_Font[i].texTop + ((float)m_Font[i].height / 256);
	}

	// Close the file.
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	// Release the font data array.
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = nullptr;
	}

	return;
}

bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}
