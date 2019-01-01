#pragma once

#include <glad/glad.h>

class IFramebuffer
{
public:
	IFramebuffer() {}
	virtual ~IFramebuffer() {}
	virtual bool Setup() = 0;
	virtual void Bind() = 0;
	virtual void Destroy() = 0;
	virtual void Unbind() = 0;
	inline unsigned int GetID() { return m_ID; }
protected:
	GLuint m_ID;
};


class ShadowPassFB : public IFramebuffer
{
public:
	ShadowPassFB(int depthPrecision,int width,int height);
	~ShadowPassFB();

	bool Setup()override;
	void Bind()override;
	void Unbind()override;
	void Destroy()override;

	GLuint GetDepthAttachment() { return m_DepthTexture; }

private:
	unsigned int m_DepthTexture;
	int m_DepthSize;
	int m_Width;
	int m_Height;
};

class FinalOutputFB : public IFramebuffer
{
public:
	FinalOutputFB(int channelsCount,int depthSize,float width,float height);
	~FinalOutputFB();
	
	bool Setup()override;
	void Bind()override;
	void Unbind()override;
	void Destroy()override;
	
	GLuint GetColorAttachment() 
	{
		return m_ColorTexture; 
	}

private:
	float m_Width, m_Height;
	int m_DepthSize;
	int m_Channels;
	unsigned int m_ColorTexture;
	unsigned int m_DepthRBO;

};