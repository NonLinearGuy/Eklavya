#pragma once

class IRenderState
{
public:
	virtual void Enter() = 0;
	virtual void Exit() = 0;
protected:
};

class BlendState : public IRenderState
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