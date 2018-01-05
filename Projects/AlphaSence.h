#include "../defines.h"
#include "IUpdateDraw.h"
class AlphaSence :public IUpdateDraw
{
public:
	AlphaSence();
	~AlphaSence();
	void Update() override ;
	 void Draw() override;
	
};
