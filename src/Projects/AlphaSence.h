#include "../defines.h"
#include "IUpdateDraw.h"

#include "../core/Renderer.h"
class AlphaSence :public IUpdateDraw
{
public:
	AlphaSence();
	~AlphaSence();
	void Update() override ;
	 void Draw() override;
	Renderer2D m_Render;
};
