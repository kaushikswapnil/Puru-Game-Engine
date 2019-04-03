#include "puruAnim.h"
#include "puruTexture.h"


puruAnim::puruAnim() : m_Sprite(nullptr), m_nFrames(0), m_nFPS(0), m_dtChangeTime(0.0f), m_dtCurrentTime(0.0f), m_bPlaying(false), m_bLoop(false), m_texLeft(0.0f), m_texTop(0.0f),
m_width(0), m_height(0), m_curFrame(0)
{
}

puruAnim::puruAnim(int Frames, int nFPS, float texLeft, float texTop, int width, int height, puruTexture * texture) : m_Sprite(nullptr), m_nFrames(0), m_nFPS(0),
m_dtChangeTime(0.0f), m_dtCurrentTime(0.0f), m_bPlaying(false), m_bLoop(false), m_texLeft(0.0f), m_texTop(0.0f), m_width(0), m_height(0), m_curFrame(0)
{
	Initialize(Frames, nFPS, texLeft, texTop, width, height, texture);
}

puruAnim::puruAnim(const puruAnim & other) 
{
	m_nFrames = other.m_nFrames;
	m_nFPS = other.m_nFPS;
	m_texLeft = other.m_texLeft;
	m_texTop = other.m_texTop;
	m_texStartLeft = other.m_texStartLeft;
	m_texStartTop = other.m_texStartTop;
	m_width = other.m_width;
	m_height = other.m_height;

	m_bLoop = other.m_bLoop;
	m_bPlaying = other.m_bPlaying;

	m_dtCurrentTime = 0.0f;
	m_dtChangeTime = (float)(1.0f / m_nFPS);

	m_curFrame = 0;

	m_Sprite = new puruSprite(*other.m_Sprite);
	if (!m_Sprite)
	{
		m_Sprite = nullptr;
		return;
	}
}


puruAnim::~puruAnim()
{
	Shutdown();
}

puruAnim & puruAnim::operator=(const puruAnim & other)
{
	m_nFrames = other.m_nFrames;
	m_nFPS = other.m_nFPS;
	m_texLeft = other.m_texLeft;
	m_texTop = other.m_texTop;
	m_texStartLeft = other.m_texStartLeft;
	m_texStartTop = other.m_texStartTop;
	m_width = other.m_width;
	m_height = other.m_height;

	m_bLoop = other.m_bLoop;
	m_bPlaying = other.m_bPlaying;

	m_dtCurrentTime = 0.0f;
	m_dtChangeTime = (float)(1.0f / m_nFPS);

	m_curFrame = 0;

	if (m_Sprite)
	{
		m_Sprite->Shutdown();
		delete m_Sprite;
		m_Sprite = nullptr;
	}

	m_Sprite = new puruSprite(*other.m_Sprite);

	return *this;
}

bool puruAnim::Initialize(int Frames, int nFPS, float texLeft, float texTop, int width, int height, puruTexture * texture)
{
	bool result;

	m_nFrames = Frames;
	m_nFPS = nFPS;
	m_texLeft = texLeft;
	m_texTop = texTop;
	m_texStartLeft = texLeft;
	m_texStartTop = texTop;
	m_width = width;
	m_height = height;

	m_dtCurrentTime = 0.0f;
	m_dtChangeTime = (float)(1.0f / nFPS);

	m_Sprite = new puruSprite();
	if (!m_Sprite)
	{
		return false;
	}

	result = m_Sprite->Initialize(texLeft, texTop, width, height, texture);
	if (!result)
	{
		delete m_Sprite;
		m_Sprite = nullptr;
		return false;
	}

	return true;
}

void puruAnim::Shutdown()
{
	if (m_Sprite)
	{
		m_Sprite->Shutdown();
		delete m_Sprite;
		m_Sprite = nullptr;
	}
}

void puruAnim::Update(float elapsedTime)
{
	if(m_bPlaying)
	{
		m_dtCurrentTime += elapsedTime;
		if (m_dtCurrentTime > m_dtChangeTime)
		{
			ChangeSprite();
			m_dtCurrentTime = 0.0f;
		}
	}
}

void puruAnim::Render(int x, int y, float scale)
{
	if(m_bPlaying)
	{
		if (m_Sprite)
		{
			m_Sprite->Render(x, y, scale);
		}
	}
}

void puruAnim::Play()
{
	m_bPlaying = true;
}

void puruAnim::Stop()
{
	m_bPlaying = false;
}

void puruAnim::SetLoopMode(bool value)
{
	m_bLoop = value;
}

bool puruAnim::IsPlaying()
{
	return m_bPlaying;
}

void puruAnim::ChangeSprite()
{
	if (m_bPlaying)
	{
		if(m_Sprite)
		{
			puruTexture* ptrTex = m_Sprite->GetTexture();
			if (ptrTex)
			{
				if (m_curFrame > m_nFrames-2)
				{
					m_curFrame = 0;
					if (!m_bLoop)
					{
						m_bPlaying = false;
						return;
					}
				}
				else
				{
					m_curFrame++;
				}
				int picWidth = ptrTex->GetWidth();
				int picHeight = ptrTex->GetHeight();
				bool result;

				m_texLeft = m_texStartLeft + ((float)(m_width*m_curFrame) / (float)picWidth);

				//Check for overfow
				int picLeft = (int)(m_texLeft * picWidth); //Pixel position on picture
				int picTop = (int)(m_texTop * picHeight); //^

				picLeft += m_width;
				if (picLeft > picWidth) //Send back to start of row and one col down
				{
					picLeft = 0;
					m_texLeft = m_texStartLeft;
					m_texTop += ((float)m_height / (float)picHeight);
				}			

				m_Sprite->Shutdown();
				delete m_Sprite;
				m_Sprite = nullptr;

				m_Sprite = new puruSprite;
				if (!m_Sprite)
				{
					return;
				}

				result = m_Sprite->Initialize(m_texLeft, m_texTop, m_width, m_height, ptrTex);
				if (!result)
				{
					delete m_Sprite;
					m_Sprite = nullptr;
					return;
				}

			}
		}
	}
}
