#pragma once
#include<string>

class TextClass;

class puruFont
{
public:
	puruFont();
	puruFont(char* fontDataFilename, char* textureFilename);
	~puruFont();

	void Shutdown();

	bool Initialize(char* fontDataFilename, char* textureFilename);

	void printf(int x, int y, char* strToPrint, float red = 1.0f, float green = 1.0f, float blue = 1.0f);
	void printf(int x, int y, const char* strToPrint, float red = 1.0f, float green = 1.0f, float blue = 1.0f);
	void printf(int x, int y, std::string strToPrint, float red = 1.0f, float green = 1.0f, float blue = 1.0f);

	void GetStringDimensions(char* strToPrint, float& width, float& height);	

private:
	TextClass* m_text;
};

