#include "Game/Collision.h"
#include "Game/Components/ColliderComponent.h"

bool Collision::coll(const SDL_Rect& rec1, const SDL_Rect& rec2)
{
	if (
		rec1.x + rec1.w > rec2.x &&
		rec2.x + rec2.w > rec1.x &&
		rec1.y + rec1.h > rec2.y &&
		rec2.y + rec2.h > rec1.y
		)
	{
		return true;
	}

	return false;
}

bool Collision::coll(const ColliderComponent& col1, const ColliderComponent& col2)
{
	if (coll(col1.collider, col2.collider) and col1.tag != col2.tag)
	{
		// std::cout << col1.tag << " hit " << col2.tag << std::endl;
		return  true;
	}
	else {
		return false;
	}
}