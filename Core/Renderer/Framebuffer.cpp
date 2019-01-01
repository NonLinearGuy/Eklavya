#include "Framebuffer.h"
#include "../Utils/Logger.h"
#include <glad/glad.h>

using namespace HipHop;

//SHADOW PASS
ShadowPassFB::ShadowPassFB(int depthPrecision,int width,int height) : m_DepthSize(depthPrecision), m_Width(width),m_Height(height)
{
}

ShadowPassFB::~ShadowPassFB()
{
}

bool ShadowPassFB::Setup()
{
	if (m_DepthSize != 24 && m_DepthSize != 16 && m_DepthSize != 32)
	{
		Logger::GetInstance()->Log("Main Pass FB error : depth size is invalid");
		return false;
	}
	glGenFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	//Creating color attachemnt
	glGenTextures(1, &m_DepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Logger::GetInstance()->Log("ERROR:: SHADOW PASS FRAMEBUFFER:: Framebuffer is not complete!");
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void ShadowPassFB::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER,m_ID);
}

void ShadowPassFB::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowPassFB::Destroy()
{
	glDeleteTextures(1,&m_DepthTexture);
	glDeleteFramebuffers(1,&m_ID);
}

//FOR MAIN PASS

FinalOutputFB::FinalOutputFB(int channelsCount, int depthSize,float width,float height)
	:
	m_Channels(channelsCount), m_DepthSize(depthSize), m_Width(width), m_Height(height)
{
}

FinalOutputFB::~FinalOutputFB()
{
}

bool FinalOutputFB::Setup()
{
	if (m_DepthSize != 24 && m_DepthSize != 16 && m_DepthSize != 32)
	{
		Logger::GetInstance()->Log("Main Pass FB error : depth size is invalid");
		return false;
	}
	glGenFramebuffers(1,&m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER,m_ID);

	//Creating color attachemnt
	glGenTextures(1,&m_ColorTexture);
	glBindTexture(GL_TEXTURE_2D,m_ColorTexture);

	GLuint format = m_Channels == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_Width,m_Height,0,GL_RGB,GL_UNSIGNED_BYTE,0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_ColorTexture,0);

	//Creating depth attachment
	glGenRenderbuffers(1,&m_DepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER,m_DepthRBO);

	GLenum depthFormat = GL_DEPTH_COMPONENT24;
	if (16 == m_DepthSize)
		depthFormat = GL_DEPTH_COMPONENT16;
	else if (32 == m_DepthSize)
		depthFormat = GL_DEPTH_COMPONENT32;

	glRenderbufferStorage(GL_RENDERBUFFER,depthFormat, m_Width, m_Height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Logger::GetInstance()->Log("ERROR:: MAIN PASS FRAMEBUFFER:: Framebuffer is not complete!");
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;

}

void FinalOutputFB::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER,m_ID);
}

void FinalOutputFB::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FinalOutputFB::Destroy()
{
	glDeleteTextures(1, &m_ColorTexture);
	glDeleteRenderbuffers(1, &m_DepthRBO);
	glDeleteFramebuffers(1,&m_ID);
}
