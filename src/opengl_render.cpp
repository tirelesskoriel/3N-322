#include<render/opengl_render.h>

#include <tools/my_shader_loader.h>
#include <model/surround_camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

OpenglRender::OpenglRender(Context* context): context(context)
{
    // Model ourModel(FileSystem::getPath("resources/objects/lj/WhaleAnim03.fbx"));
	// Model ourModel(FileSystem::getPath("resources/objects/mg/source/MANTA2.fbx"));
	// Model ourModel(FileSystem::getPath("resources/ignore/qw/Quarantine722Wraith.stl"));
	// Model ourModel(FileSystem::getPath("resources/ignore/sd/SheHulk_Decimated.obj"));
	// Model ourModel(FileSystem::getPath("resources/objects/knight/knight.obj"));
	// Model ourModel(FileSystem::getPath("resources/objects/GEOMETRY/moto_001.obj"), true);
	// Model ourModel(FileSystem::getPath("resources/objects/sz/sz.fbx"), true);
	// Model ourModel(FileSystem::getPath("resources/objects/dd/gedou.fbx"));
	// Model ourModel(FileSystem::getPath("resources/ignore/Nv_JianKe/NnJianKe_Delete Light UV_Mod.fbx"));
	// Model ourModel(FileSystem::getPath("resources/ignore/r/robot.FBX"));
	// Model ourModel(FileSystem::getPath("resources/objects/gd/sazabi_1.obj"), true);
	model = new Model(context->getPath("resources/objects/sz/sz.fbx"), true);
	// Model ourModel(FileSystem::getPath("resources/objects/Sphere-BotBasic/Armature_001-(FBX 7.4 binary mit Animation).fbx"), true);
	// Model ourModel(FileSystem::getPath("resources/objects/gun/Handgun_fbx_7.4_binary.fbx"), true);
	// Model ourModel(FileSystem::getPath("resources/objects/dragon/Dragon 2.5_fbx.fbx"));
	// Model ourModel(FileSystem::getPath("resources/objects/dragon/Dragon_Baked_Actions_fbx_6.1_ASCII.fbx"));
	// Model ourModel(FileSystem::getPath("resources/objects/zz/Spider_3.fbx"));
	glm::vec3 light = glm::vec3(1.0f, 1.0f, 1.0f);

	const ShaderLoader* ourShader = model->shader;
	ourShader->use();
	ourShader->setBool("hasAnimation", model->hasAnimation());
	ourShader->setVec3("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	ourShader->setVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	ourShader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	ourShader->setVec3("light.position", glm::vec3(10.0f, 10.0f, 10.0f));
}

OpenglRender::~OpenglRender()
{
    delete model;
}

void OpenglRender::render()
{   
    const ShaderLoader* ourShader = model->shader;
    ourShader->use();
		ourShader->setVec3("viewPos", context->camera->Eye);

		glm::mat4 projection = glm::perspective(glm::radians(context->camera->Zoom), (float)context->scrWidth / (float)context->scrHeight, 0.1f, 100.0f);
		glm::mat4 view = context->camera->GetViewMatrix();
		ourShader->setMat4("projection", projection);
		ourShader->setMat4("view", view);

		// glm::rotate();
		model->runAnimator();

		// if(autoRotate)
		// {
		// 	model = glm::rotate(model, glm::radians((float)glfwGetTime()) * 0.01f, glm::vec3(0.0, 1.0, 0.0));
		// }
		glm::mat4 modelmat4 = glm::mat4(1.0f);
		model->Draw(modelmat4);
}