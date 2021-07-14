
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include "Camera.h"

#include <ctime>

#include "Highlight.h"
#include "SelectableManager.h"
#include "Map.h"

#include "InfluencePointGenerator.h"

#include "Renderer.h"


#define SBswitching_rate 60.0f

Camera* camera;
float lastFrame, deltaTime;
irrklang::ISoundEngine* soundEngine;

void reactEvent(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->Scroll(Direction::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->Scroll(Direction::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
	{
		camera->fov = camera->initFov;
	}
}

float calcNightIntensity()
{
	float currTime = glfwGetTime();
	float mod = std::fmodf(currTime / SBswitching_rate, 2.0f);
	if (mod < 0.75f)
		return 0.0f;
	else if (mod < 1.0f)
		return (mod - 0.75f) * 4.0f;
	else if (mod < 1.75f)
		return 1.0f;
	else
		return 1.0f - (mod - 1.75f) * 4.0f;
}


int main(void)
{
	srand(time(0));

	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	soundEngine = irrklang::createIrrKlangDevice();

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	const GLFWvidmode* vid = glfwGetVideoMode(primary);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	float widthToHeightRatio = (float)vid->width / vid->height;
	float heightToWidthRatio = (float)vid->height / vid->width;

	window = glfwCreateWindow(vid->width, vid->height, "", primary, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	float WidthFactor = vid->width / MY_RESOLUTION_WIDTH;
	float  HeightFactor = vid->height / MY_RESOLUTION_HEIGHT;

	glfwMakeContextCurrent(window);

	std::cout << glGetString(GL_VERSION) << std::endl;

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}

	float cubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f, // Bottom-left
		 0.5f,  0.5f, -0.5f, // top-right
		 0.5f, -0.5f, -0.5f, // bottom-right         
		 0.5f,  0.5f, -0.5f, // top-right
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f,  0.5f, -0.5f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f, // bottom-left
		 0.5f, -0.5f,  0.5f, // bottom-right
		 0.5f,  0.5f,  0.5f, // top-right
		 0.5f,  0.5f,  0.5f, // top-right
		-0.5f,  0.5f,  0.5f, // top-left
		-0.5f, -0.5f,  0.5f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f, // top-right
		-0.5f,  0.5f, -0.5f, // top-left
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f, -0.5f,  0.5f,// bottom-right
		-0.5f,  0.5f,  0.5f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f, // top-left
		 0.5f, -0.5f, -0.5f, // bottom-right
		 0.5f,  0.5f, -0.5f, // top-right         
		 0.5f, -0.5f, -0.5f, // bottom-right
		 0.5f,  0.5f,  0.5f, // top-left
		 0.5f, -0.5f,  0.5f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f, // top-right
		 0.5f, -0.5f, -0.5f, // top-left
		 0.5f, -0.5f,  0.5f, // bottom-left
		 0.5f, -0.5f,  0.5f, // bottom-left
		-0.5f, -0.5f,  0.5f, // bottom-right
		-0.5f, -0.5f, -0.5f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f, // top-left
		 0.5f,  0.5f,  0.5f, // bottom-right
		 0.5f,  0.5f, -0.5f, // top-right     
		 0.5f,  0.5f,  0.5f, // bottom-right
		-0.5f,  0.5f, -0.5f, // top-left
		-0.5f,  0.5f,  0.5f  // bottom-left        
	};


	VertexArray* BOXva;
	VertexBuffer* BOXvb;

	BOXva = new VertexArray;
	VertexBufferLayout layout;
	layout.Push<float>(3);
	BOXvb = new VertexBuffer(sizeof(cubeVertices), cubeVertices);
	BOXva->AddBuffer(*BOXvb, layout);


	Terrain terrain(0.0f, 0.0f, "res/textures/maps/heightmap.png", "res/textures/maps/ground.jpg", "res/textures/maps/leaves.jpg", 
		"res/textures/maps/grassFlowers.png", "res/textures/maps/mud.png", "res/textures/maps/blendMap.png");


	const Shader* tShader = terrain.getShader();
	const Shader animalShader("res/shaders/Animal.shader");
	const Shader IFpointShader("res/shaders/InfluencePoint.shader");
	const Shader buildingShader("res/shaders/Building.shader");
	const Shader outline("res/shaders/animalOutline.shader");
	const Shader BOXshader("res/shaders/Bounds.shader");

	tShader->Bind();
	tShader->SetUniform1f("cordsTilingFactor", sqrt(WORLD_SIZE));


	terrain.SetDirLightProperties(glm::vec3(0.0f, -0.5f, 1.0f), glm::vec3(0.1f), glm::vec3(0.5f));

	camera = new Camera;

	double xpos, ypos;

	Model wolfModel("res/objects/Wolf/wolf.obj");
	Model caveModel("res/objects/Cave/cave.obj");
	Model pandaModel("res/objects/Panda/panda.obj");
	Model tortoiseModel("res/objects/Tortoise/tortoise.obj");
	Model treeModel("res/objects/Tree/Tree.obj");
	Model holyTreeModel("res/objects/HolyTree/Tree.obj");
	Model targetModel("res/objects/Target/target.obj");
	Model pyramidModel("res/objects/Pyramid/Pyramid.obj");
	Model statueModel("res/objects/Statue/Statue.obj");

	float cavex = -5.0f, cavez = -5.0f;
	Cave cave(cavex, cavez, 1, 0, 0, WORLD_SIZE, soundEngine, &wolfModel, &pandaModel, &tortoiseModel, &caveModel, terrain.getHeightOfTerrain(cavex, cavez), BOXva, BOXvb, &BOXshader, &animalShader, &outline, &terrain);

	Highlight highlight(vid->width, vid->height);

	TextRenderer tr(0, vid->width, vid->height);

	ScoreHolder scoreHolder(10000.0f, 1.0f);

	BuildingHolder buildingHolder(&targetModel, &pyramidModel, &statueModel, BOXva, BOXvb, &BOXshader, &buildingShader);

	SelectableManager selectableManager(-0.5f, -0.9f, &tr, &scoreHolder, WidthFactor, HeightFactor, &cave, &buildingHolder);

	Map map(vid->width, vid->height);

	InfluencePointGenerator IFgenerator(10, WORLD_SIZE, &treeModel, &holyTreeModel, &terrain, vid->width, vid->height, &scoreHolder, BOXva, BOXvb, &BOXshader, &IFpointShader);

	bool insideTheMap = false;

	Renderer masterRenderer(&cave, &tr, &scoreHolder, &buildingHolder, &selectableManager, &IFgenerator, &terrain, tShader, &animalShader,
		&IFpointShader, &buildingShader, &outline, &BOXshader, WidthFactor, HeightFactor);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	while (!glfwWindowShouldClose(window))
	{
		glm::mat4 projection = glm::perspective(glm::radians(camera->fov), widthToHeightRatio, 0.1f, 300.0f);

		float currTime = glfwGetTime();
		deltaTime = currTime - lastFrame;
		lastFrame = currTime;

		reactEvent(window);
		glfwGetCursorPos(window, &xpos, &ypos);
		scoreHolder.UpdateEssenceValue(deltaTime);
		
		if (xpos <= 3)
			camera->Move(Direction::LEFT, deltaTime);
		else if (xpos >= vid->width - 3)
			camera->Move(Direction::RIGHT, deltaTime);

		if (ypos <= 3)
			camera->Move(Direction::FORWARD, deltaTime);
		else if (ypos >= vid->height - 3)
			camera->Move(Direction::BACKWARD, deltaTime);

		insideTheMap = false;

		glm::vec3 cameraPosition = camera->GetPosition();
		glm::vec3 cameraInitialPosition = camera->GetInitialPosition();

		float* hp = new float;
		*hp = 5.0f;

		cave.MoveAnimals(deltaTime, cameraPosition, hp, currTime);
		cave.CheckAnimal_AnimalColision(deltaTime);

		IFgenerator.CaptureInfluencePoints(cave.GetTurtlePositions(), deltaTime);

		bool isMovingBuildingColliding = false;
		if (buildingHolder.IsBuildingMovingNow())
		{
			isMovingBuildingColliding = buildingHolder.CheckMovingBuilding_BuildingColision() ||
				buildingHolder.CheckMovingBuilding_IFPointsColision(IFgenerator.GetIFColisionBoxes()) ||
				buildingHolder.CheckMovingBuilding_AnimalColision(cave.GetAnimalColisionBoxes())
				|| buildingHolder.CheckMovingBuilding_OutOfBounds(WORLD_SIZE);
		}

		glm::mat4 view = camera->GetViewMatrix();

		masterRenderer.render(projection, view, isMovingBuildingColliding);

		int fps = 1.0f / deltaTime;
		tr.RenderText(std::to_string(fps), (MY_RESOLUTION_WIDTH - 100.0f) * WidthFactor, (MY_RESOLUTION_HEIGHT - 50.0f) * HeightFactor, 0.75f * std::fminf(WidthFactor, HeightFactor), glm::vec3(0.0f, 1.0f, 0.0f));

		float cursorXPosNormalized = (xpos / vid->width) * 2.0f - 1.0f;
		float cursorYPosNormalized = ((vid->height - ypos) / vid->height) * 2.0f - 1.0f;
		insideTheMap = map.isInside(cursorXPosNormalized, cursorYPosNormalized);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			BuildingType buildingType = buildingHolder.GetLastMovingBuildingType();
			if (buildingHolder.TryToCancelMovingBuilding())
				selectableManager.RefundCostOfBuilding(buildingType);
			int area = selectableManager.GetSelectableChosenArea(cursorXPosNormalized, cursorYPosNormalized);

			if (insideTheMap)
			{
				glm::vec3 result = map.processMouseInput(cursorXPosNormalized, cursorYPosNormalized);
				result *= WORLD_SIZE;
				cave.takePathForSelected(result);
			}
			else
			{
				if (area != -1)
				{
					cave.selectAnimalType((AnimalType)area);
				}
				else
				{
					float linez = tan(glm::radians(camera->fov / 2.0f)) * cameraPosition.y * 2.0f;
					float line = linez * widthToHeightRatio;
					float putXPos = (vid->width - xpos) / vid->width;
					float putZPos = (vid->height - ypos) / vid->height;
					putXPos *= line;
					putZPos *= linez;
					glm::vec3 deltaPos = cameraPosition - cameraInitialPosition;
					glm::vec3 pos;
					pos.x = -putXPos + 0.5f * (line - tan(glm::radians(camera->initFov / 2.0f)) * cameraPosition.y * 2.0f * widthToHeightRatio) + deltaPos.x;
					if (pos.x > 0.0f)
						pos.x = 0.0f;
					else if (pos.x < -WORLD_SIZE)
						pos.x = -WORLD_SIZE;

					pos.y = -0.5f;
					pos.z = -putZPos + 0.5f * (linez - tan(glm::radians(camera->initFov / 2.0f)) * cameraPosition.y * 2.0f) + deltaPos.z;
					if (pos.z > 0.0f)
						pos.z = 0.0f;
					else if (pos.z < -WORLD_SIZE)
						pos.z = -WORLD_SIZE;
					cave.takePathForSelected(pos);
				}
			}
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			if (insideTheMap && !highlight.GetHaveInitHL())
			{
				glm::vec3 result = map.processMouseInput(cursorXPosNormalized, cursorYPosNormalized);
				result *= WORLD_SIZE;
				camera->SetPosition(result);
			}
			else
			{
				highlight.processHighlight(xpos, ypos);
				glDrawElements(GL_LINES, 8, GL_UNSIGNED_BYTE, nullptr);
			}
		}
		else if(highlight.GetHaveInitHL())
		{
			highlight.DisableInitHL();
			float* currHighlight = highlight.GetCurrHighlight();

			float linez = tan(glm::radians(camera->fov / 2.0f)) * cameraPosition.y * 2.0f;
			float line = linez * widthToHeightRatio;
			float initXPos = 1.0f - (currHighlight[0] / 2.0f + 0.5f);
			float initZPos = currHighlight[1] / 2.0f + 0.5f;
			initXPos *= line;
			initZPos *= linez;

			float currXPos = 1.0f - (currHighlight[4] / 2.0f + 0.5f);
			float currZPos = currHighlight[5] / 2.0f + 0.5f;
			currXPos *= line;
			currZPos *= linez;

			glm::vec3 deltaPos = cameraPosition - cameraInitialPosition;

			initXPos = -initXPos + 0.5f * (line - tan(glm::radians(camera->initFov / 2.0f)) * cameraPosition.y * 2.0f * widthToHeightRatio) + deltaPos.x;
			initZPos = -initZPos + 0.5f * (linez - tan(glm::radians(camera->initFov / 2.0f)) * cameraPosition.y * 2.0f) + deltaPos.z;

			currXPos = -currXPos + 0.5f * (line - tan(glm::radians(camera->initFov / 2.0f)) * cameraPosition.y * 2.0f * widthToHeightRatio) + deltaPos.x;
			currZPos = -currZPos + 0.5f * (linez - tan(glm::radians(camera->initFov / 2.0f)) * cameraPosition.y * 2.0f) + deltaPos.z;

			ImgSelectedData data = selectableManager.FindImageSelected(cursorXPosNormalized, cursorYPosNormalized);
			if (data.animalSelectedNow)
			{
				ImgAnimal animalData = (ImgAnimal)data.ImgSelected;
				switch (animalData)
				{
					case ImgAnimal::IMG_WOLF:
						cave.AddAnimal(AnimalType::WOLF);
						break;
					case ImgAnimal::IMG_PANDA:
						cave.AddAnimal(AnimalType::PANDA);
						break;
					case ImgAnimal::IMG_TURTLE:
						cave.AddAnimal(AnimalType::TURTLE);
						break;
					case ImgAnimal::IMG_HAMMER:
						selectableManager.SwapSelectables();
						break;
					case ImgAnimal::NONE:
						if (!isMovingBuildingColliding)
							buildingHolder.TryToPlaceMovingBuilding();
						break;
				}
			}
			else
			{
				ImgBuilding buildingData = (ImgBuilding)data.ImgSelected;
				switch (buildingData)
				{
					case ImgBuilding::IMG_PARROT:
						selectableManager.SwapSelectables();
						break;
					case ImgBuilding::IMG_TARGET:
						buildingHolder.AddMovingBuilding(BuildingType::TARGET);
						break;
					case ImgBuilding::IMG_PYRAMID:
						buildingHolder.AddMovingBuilding(BuildingType::PYRAMID);
						break;
					case ImgBuilding::IMG_STATUE:
						buildingHolder.AddMovingBuilding(BuildingType::STATUE);
						break;
					case ImgBuilding::NONE:
						if(!isMovingBuildingColliding)
							buildingHolder.TryToPlaceMovingBuilding();
						break;
				}
			}
			cave.UpdateSelectedStatus(initXPos, initZPos, currXPos, currZPos);
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDepthFunc(GL_ALWAYS);
		int nrSelected = cave.GetNrSelectedAnimals();
		map.Draw(cave.GetAnimalPositionsNormalizedNotSelected(heightToWidthRatio), sizeof(float) * 2 * (cave.GetAnimalCount(AnimalType::ALL) - nrSelected), 
			cave.GetAnimalPositionsNormalizedSelected(heightToWidthRatio), sizeof(float) * 2 * nrSelected, buildingHolder.GetBuildingPositionsNormalized(heightToWidthRatio),
			buildingHolder.GetNumberOfBuildings(), IFgenerator.GetIFPositionsNormalized(heightToWidthRatio), IFgenerator.GetNumberOfIF(), camera->GetPositionNormalized(heightToWidthRatio));
		
		if (cave.GetSelected())
		{
			selectableManager.Draw();
		}

		glDepthFunc(GL_LESS);

		{
			float linez = tan(glm::radians(camera->fov / 2.0f)) * cameraPosition.y * 2.0f;
			float line = linez * widthToHeightRatio;

			float pointXNor = 2.0f * ((xpos) / vid->width) - 1.0f;
			float pointYNor = 2.0f * ((vid->height - ypos) / vid->height) - 1.0f;

			float currXPos = 1.0f - (pointXNor / 2.0f + 0.5f);
			float currZPos = pointYNor / 2.0f + 0.5f;
			currXPos *= line;
			currZPos *= linez;
			glm::vec3 deltaPos = cameraPosition - cameraInitialPosition;
			currXPos = -currXPos + 0.5f * (line - tan(glm::radians(camera->initFov / 2.0f)) * cameraPosition.y * 2.0f * widthToHeightRatio) + deltaPos.x;
			currZPos = -currZPos + 0.5f * (linez - tan(glm::radians(camera->initFov / 2.0f)) * cameraPosition.y * 2.0f) + deltaPos.z;

			if(buildingHolder.IsBuildingMovingNow())
				buildingHolder.MoveBuilding(currXPos, currZPos, terrain.getHeightOfTerrain(currXPos, currZPos));
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}