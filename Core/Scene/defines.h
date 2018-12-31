#pragma once

#define OVERRIDE_BASE_NODE_METHODS  bool Init()override;\
									void Destroy()override;\
									void Tick(float)override;\
									void PreRender()override;\
									void Render()override;\
									void RenderChildren()override;\
									void PostRender() override;
