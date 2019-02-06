#pragma once

#include "ShaderProgram.h"
#include <memory>

class IRenderState
{
public:
	virtual void Enter() = 0;
	virtual void Exit() = 0;
protected:
};

class AlphaRenderState : public IRenderState
{
public:
	void Enter()override;
	void Exit()override;
private:
};


class SkyBoxState : public IRenderState
{
public:
	void Enter()override;
	void Exit()override;
};


class DefaultState : public IRenderState
{
public:
	void Enter()override;
	void Exit()override;
};

class UIState : public IRenderState
{
public:
	void Enter()override;
	void Exit()override;
};