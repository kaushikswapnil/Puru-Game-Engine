#pragma once

class BitmapClass;
class TextureShaderClass;
class puruTexture;

class puruQuad
{
	struct VerticeStructure
	{
		int x, y;
	};
public:
	puruQuad();
	puruQuad(puruTexture * texture, float texLeft, float texTop, unsigned int width, unsigned int height);
	puruQuad(const puruQuad&);
	~puruQuad();

	puruQuad& operator=(const puruQuad& other);

	bool Initialize(puruTexture * texture, float texLeft, float texTop, unsigned int width, unsigned int height);

	void CalculateVerticeLocations(int x, int y);

	void Shutdown();

	void Render(int x, int y, float scale = 1.0f);
	void RenderQuad();

	void SetTextureLocations(float texLeft, float texTop);

	puruTexture* GetTexture();

public:
	VerticeStructure v[4]; //Clockwise topleft, topright, bottomright, bottomleft
	
private:
	void UpdateBitmapDimensions();
	void Render(float scale = 1.0f);
	float m_texLeft, m_texTop;

private:
	BitmapClass* m_Bitmap;
	TextureShaderClass* m_TextureShader;
	
};

