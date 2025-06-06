#include "TransformMushroom.h"

int CTransformMushroom::GetAniId(int state)
{
	if (state == MUSHROOM_STATE_RAISING)
		return ID_ANI_TRANSFORM_MUSHROOM;
	return ID_ANI_TRANSFORM_MUSHROOM_COMPLETE;
}
