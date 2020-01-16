#include<render/opengl_render.h>

#include <tools/my_shader_loader.h>
#include <model/surround_camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdimage/stb_image.h>
#include <GLFW/glfw3.h>

const float scale = 4.0f;

// TODO
void createFrameBuffer(const Context* context, float scale, unsigned int* framebuffer, unsigned int* textureColorbuffer);

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
	model = new Model(context->getPath("resources/objects/gd/sazabi_1.obj"), true);
	// Model ourModel(FileSystem::getPath("resources/objects/Sphere-BotBasic/Armature_001-(FBX 7.4 binary mit Animation).fbx"), true);
	// Model ourModel(FileSystem::getPath("resources/objects/gun/Handgun_fbx_7.4_binary.fbx"), true);
	// Model ourModel(FileSystem::getPath("resources/objects/dragon/Dragon 2.5_fbx.fbx"));
	// Model ourModel(FileSystem::getPath("resources/objects/dragon/Dragon_Baked_Actions_fbx_6.1_ASCII.fbx"));
	// Model ourModel(FileSystem::getPath("resources/objects/zz/Spider_3.fbx"));

	modelShader = new ShaderLoader("animation.vs", "model_2d.fs");
	modelShader->use();
	modelShader->setBool("hasAnimation", model->hasAnimation());
	modelShader->setVec3("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	modelShader->setVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	modelShader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	modelShader->setVec3("light.position", glm::vec3(10.0f, 10.0f, 10.0f));

	// TODO
	for(int i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		createFrameBuffer(context, i + 1, &framebuffers[i], &textureColorbuffers[i]);
	}

	float texCoords[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

	// screen quad VAO
    unsigned int textureVBO;
    glGenVertexArrays(1, &textureVAO);
    glGenBuffers(1, &textureVBO);
    glBindVertexArray(textureVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), &texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	seShader = new ShaderLoader("es.vs", "es.fs");
	seShader->use();
    seShader->setInt("screenTexture", 0);

}

OpenglRender::~OpenglRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(FRAME_BUFFER_COUNT, framebuffers);
	glDeleteTextures(FRAME_BUFFER_COUNT, textureColorbuffers);

    delete model;
    delete seShader;
}

void OpenglRender::render()
{   
	if(is_3n)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[PL - 1]);
		glViewport(0,0,context->scrWidth / PL ,context->scrHeight / PL);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
		glViewport(0,0,context->scrWidth ,context->scrHeight);
	}
	
	
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelShader->use();
	modelShader->setVec3("viewPos", context->camera->Eye);
	glm::mat4 projection = glm::mat4(1.0f);

	if(is_3n)
	{
		float L_OR_R = 1.0f;
		float T_OR_B = 1.0f;
		if(context->H2W() < 1)
			L_OR_R = context->W2H();
		else
			T_OR_B = context->H2W();
		projection = glm::ortho(-L_OR_R, L_OR_R, -T_OR_B, T_OR_B, 0.1f, 100.0f);
	}
	else
	{
		projection = glm::perspective(glm::radians(context->camera->Zoom), (float)context->scrWidth / (float)context->scrHeight, 0.1f, 100.0f);
	}

	glm::mat4 view = context->camera->GetViewMatrix();
	modelShader->setMat4("projection", projection);
	modelShader->setMat4("view", view);
	model->runAnimator(modelShader);

	glm::mat4 modelmat4 = glm::mat4(1.0f);
	model->Draw(modelShader, modelmat4);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,context->scrWidth,context->scrHeight);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	seShader->use();
	glBindVertexArray(textureVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffers[PL - 1]);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	
}

// TODO
void createFrameBuffer(const Context* context, float scale, unsigned int* framebuffer, unsigned int* textureColorbuffer)
{
	// framebuffer configuration
    // -------------------------

	
    glGenFramebuffers(1, framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
    // create a color attachment texture
    glGenTextures(1, textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, *textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, context->scrWidth / scale, context->scrHeight / scale, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, context->scrWidth / scale, context->scrHeight / scale); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}