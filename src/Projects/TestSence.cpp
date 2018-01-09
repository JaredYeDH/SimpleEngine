#include "TestSence.h"

TestSence::TestSence() 
	:cubePositions({
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f) })
{
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	SpriteBatch::GetInstance()->Init();
	CubeBatch::GetInstance()->Init();



   // pMapObject = new MapObject("Map");
	pMapObject = new MapObject();
	
	p_Texture = new Texture( Environment::GetAbsPath("Resource/Assets/bizhi.jpg"));
	tw = p_Texture->GetWidth();
	th = p_Texture->GetHeight();
}

TestSence::~TestSence()
{

}

void TestSence::Update()
{
	for(int i=0;i<1;i++)
	{
		SpriteInfo* info = new SpriteInfo();
		info->Pos.x = 100+i*10;
		info->Pos.y = -200+i*10;
		(info->Size).x = tw;
		(info->Size).y = th;
		info->Alpha = 0.9f;
		info->TextureID = p_Texture->GetTextureID();

		SpriteBatch::GetInstance()->AddSprite(info);
	}


	if(InputManager::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		pos.y-=0.1F;
	}


	if(InputManager::GetInstance()->IsKeyDown(GLFW_KEY_A))
	{
		pos.x+=0.1F;
	}


	if(InputManager::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		pos.y+=0.1F;
	}


	if(InputManager::GetInstance()->IsKeyDown(GLFW_KEY_D))
	{
		pos.x-=0.1F;
	}

	view = glm::lookAt(pos, pos + front, up);

	for(int i=0;i<10;i++)
	{
		glm::mat4 model;
        //model = glm::scale(model,glm::vec3(0.5,0.5,0.5));            
		model = glm::translate(model,cubePositions[i]);            
		GLfloat angle = 20.0f * i; 
		model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));

		CubeInfo* info = new CubeInfo();
		info->Model =glm::mat4(model);
		info->View =glm::mat4(view);
		info->Projection = glm::mat4(projection);
		info->FillColor = glm::vec4(0,1,1,1);
		info->TextureID = p_Texture->GetTextureID();
		CubeBatch::GetInstance()->AddCube(info);
	}


	pMapObject -> Update();

}

void TestSence::Draw()
{

	pMapObject->Draw();

	CubeBatch::GetInstance()->Flush();
	SpriteBatch::GetInstance()->Flush();
}

