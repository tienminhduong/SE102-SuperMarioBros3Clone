#include "StaticObject.h"

void StaticObject::Render()
{
	CSprites::GetInstance()->Get(spriteID)->Draw(x, y);
}
