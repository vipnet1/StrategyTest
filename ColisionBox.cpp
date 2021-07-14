#include "ColisionBox.h"
#include <algorithm>
#include<iostream>

ColisionBox::ColisionBox(float constCOLleftX, float constCOLleftZ, float lengthX, float lengthZ)
	:constCOLleftX(constCOLleftX), constCOLleftZ(constCOLleftZ)
{
	this->topLeftX = constCOLleftX;
	this->topLeftZ = constCOLleftZ;
	this->lengthX = lengthX;
	this->lengthZ = lengthZ;
	this->angle = 0.0f;
	this->maxRib = std::fmaxf(lengthX, lengthZ);
}

bool ColisionBox::CheckColision_AABB(ColisionBox* colisionBox)
{
	if (this->topLeftX + this->lengthX >= colisionBox->topLeftX &&
		this->topLeftX <= colisionBox->topLeftX + colisionBox->lengthX &&
		this->topLeftZ + this->lengthZ >= colisionBox->topLeftZ &&
		this->topLeftZ <= colisionBox->topLeftZ + colisionBox->lengthZ)
		return true;
	else
		return false;
}

bool ColisionBox::CheckColision_AABB_MaxRib(ColisionBox* colisionBox)
{
	if (this->topLeftX + this->maxRib >= colisionBox->topLeftX &&
		this->topLeftX <= colisionBox->topLeftX + colisionBox->maxRib &&
		this->topLeftZ + this->maxRib >= colisionBox->topLeftZ &&
		this->topLeftZ <= colisionBox->topLeftZ + colisionBox->maxRib)
		return true;
	else
		return false;
}

bool ColisionBox::CheckColision_SAT(ColisionBox* colisionBox)
{
	for (int a = 0; a < 4; a++)
	{
		glm::vec3 firstPoint = this->GetPointOnBox((ColisionBoxPoint)a);
		glm::vec3 secondPoint = this->GetPointOnBox((ColisionBoxPoint)((a + 1) % 4));
		glm::vec2 axisProj(-(secondPoint.z - firstPoint.z), secondPoint.x - firstPoint.x); //swap to fet normal


		float min_r1 = INFINITY, max_r1 = -INFINITY; //represents min and max shadow points on projection
		for (int p = 0; p < 4; p++)
		{
			glm::vec3 currPoint = this->GetPointOnBox((ColisionBoxPoint)p);
			float q = (currPoint.x * axisProj.x + currPoint.z * axisProj.y);//y is z in our case
			min_r1 = std::min(min_r1, q);
			max_r1 = std::max(max_r1, q);
		}

		float min_r2 = INFINITY, max_r2 = -INFINITY; //represents min and max shadow points on projection
		for (int p = 0; p < 4; p++)
		{
			glm::vec3 currPoint = colisionBox->GetPointOnBox((ColisionBoxPoint)p);
			float q = (currPoint.x * axisProj.x + currPoint.z * axisProj.y);//y is z in our case
			min_r2 = std::min(min_r2, q);
			max_r2 = std::max(max_r2, q);
		}

		if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
			return false;
	}
	return true;
}

glm::vec3 ColisionBox::GetPointOnBox(ColisionBoxPoint type)
{
	glm::mat4 trans(1.0f);

	switch (type)
	{
	case ColisionBoxPoint::TOP_LEFT:
		trans = glm::translate(trans, glm::vec3(this->topLeftX - this->constCOLleftX, 0.0f, this->topLeftZ - this->constCOLleftZ));
		trans = glm::rotate(trans, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		return trans * glm::vec4(this->constCOLleftX, 0.0f, this->constCOLleftZ, 1.0f);
	case ColisionBoxPoint::TOP_RIGHT:
		trans = glm::translate(trans, glm::vec3(this->topLeftX - this->constCOLleftX, 0.0f, this->topLeftZ - this->constCOLleftZ));
		trans = glm::rotate(trans, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		return trans * glm::vec4(this->constCOLleftX + this->lengthX, 0.0f, this->constCOLleftZ, 1.0f);
	case ColisionBoxPoint::BOT_LEFT:
		trans = glm::translate(trans, glm::vec3(this->topLeftX - this->constCOLleftX, 0.0f, this->topLeftZ - this->constCOLleftZ));
		trans = glm::rotate(trans, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		return trans * glm::vec4(this->constCOLleftX, 0.0f, this->constCOLleftZ + this->lengthZ, 1.0f);
	case ColisionBoxPoint::BOT_RIGHT:
		trans = glm::translate(trans, glm::vec3(this->topLeftX - this->constCOLleftX, 0.0f, this->topLeftZ - this->constCOLleftZ));
		trans = glm::rotate(trans, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		return trans * glm::vec4(this->constCOLleftX + this->lengthX, 0.0f, this->constCOLleftZ + this->lengthZ, 1.0f);
	}
}