#include "puruSprite.h"



puruSprite::puruSprite() : m_quad(nullptr)
{
}

puruSprite::puruSprite(float texLeft, float texTop, int width, int height, puruTexture * texture) :m_quad(nullptr)
{
	Initialize(texLeft, texTop, width, height, texture);
}

bool puruSprite::Initialize(float texLeft, float texTop, int width, int height, puruTexture * texture)
{
	m_quad = new puruQuad(texture, texLeft, texTop, width, height);
	
	if (!m_quad)
	{
		m_quad = nullptr;
		return false;
	}

	return true;
}

void puruSprite::Render(int x, int y, float scale)
{
	if (!m_quad)
	{
		return;
	}

	m_quad->Render(x, y, scale);
}

void puruSprite::Shutdown()
{
	if (m_quad)
	{
		m_quad->Shutdown();
		delete m_quad;
		m_quad = nullptr;
	}
}

puruTexture * puruSprite::GetTexture()
{
	return m_quad->GetTexture();
}

puruSprite::puruSprite(const puruSprite &copyValue)
{
	m_quad = new puruQuad();
	*m_quad = *(copyValue.m_quad);
}


puruSprite::~puruSprite()
{
	Shutdown();
}

puruSprite & puruSprite::operator=(const puruSprite & other)
{
	Shutdown();

	m_quad = new puruQuad();
	*m_quad = *(other.m_quad);

	return *this;
}
