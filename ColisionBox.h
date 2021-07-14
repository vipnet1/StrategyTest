#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class ColisionBoxPoint
{
	TOP_LEFT = 0, TOP_RIGHT = 1, BOT_RIGHT = 2, BOT_LEFT = 3
};

class ColisionBox
{
private:
	const float constCOLleftX, constCOLleftZ;
	glm::vec3 GetPointOnBox(ColisionBoxPoint type);

public:
	float topLeftX, topLeftZ;
	float lengthX, lengthZ;
	float angle;

	float maxRib;//max between lengthX and lengthZ

	ColisionBox(float constCOLleftX, float constCOLleftZ, float lengthX, float lengthZ);

	bool CheckColision_AABB(ColisionBox* colisionBox); //no rotation angle algoritm included
	bool CheckColision_AABB_MaxRib(ColisionBox* colisionBox);
	bool CheckColision_SAT(ColisionBox* colisionBox); //will rotate
};

