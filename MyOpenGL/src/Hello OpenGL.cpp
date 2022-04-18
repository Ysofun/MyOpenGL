#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "ErrorCatch.h"
#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Model.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <Texture.h>

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void ProcessInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 980;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float LastX = SCR_WIDTH / 2.0f;
float LastY = SCR_HEIGHT / 2.0f;
bool FirstMouse = true;

float DeltaTime = 0.0f;
float LastFrame = 0.0f;

glm::vec3 LightPos(1.2f, 1.0f, 2.0f);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create a window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

    {
        Shader modelShader("res/Shaders/Model.shader");
        Model myModel("res/Objects/nanosuit/nanosuit.obj");

		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			DeltaTime = currentFrame - LastFrame;
			LastFrame = currentFrame;

			ProcessInput(window);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            modelShader.Bind();

            glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT,
                0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            modelShader.SetUniformMat4("projection", projection);
            modelShader.SetUniformMat4("view", view);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f));
            model = glm::scale(model, glm::vec3(1.0f));
            modelShader.SetUniformMat4("model", model);
            myModel.Draw(modelShader);

            glfwSwapBuffers(window);
            glfwPollEvents();
		}
    }

    //{
    //    glEnable(GL_DEPTH_TEST);

    //    Shader lightShader("res/Shaders/LightCube.shader");
    //    Shader BasicShader("res/Shaders/Basic.shader");

    //    float vertices[] = {
    //        // positions          // normals           // texture coords
    //        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    //         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
    //         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    //         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    //        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    //        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    //        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    //         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
    //         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    //         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    //        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    //        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

    //        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    //        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    //        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    //        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    //        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    //        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    //         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    //         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    //         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    //         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    //         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    //         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    //        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    //         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    //         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    //         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    //        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    //        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    //        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    //         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    //         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    //         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    //        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    //        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    //    };

    //    glm::vec3 cubePositions[] = {
    //        glm::vec3(0.0f,  0.0f,  0.0f),
    //        glm::vec3(2.0f,  5.0f, -15.0f),
    //        glm::vec3(-1.5f, -2.2f, -2.5f),
    //        glm::vec3(-3.8f, -2.0f, -12.3f),
    //        glm::vec3(2.4f, -0.4f, -3.5f),
    //        glm::vec3(-1.7f,  3.0f, -7.5f),
    //        glm::vec3(1.3f, -2.0f, -2.5f),
    //        glm::vec3(1.5f,  2.0f, -2.5f),
    //        glm::vec3(1.5f,  0.2f, -1.5f),
    //        glm::vec3(-1.3f,  1.0f, -1.5f)
    //    };

    //    glm::vec3 pointLightPositions[] = {
    //    glm::vec3(0.7f,  0.2f,  2.0f),
    //    glm::vec3(2.3f, -3.3f, -4.0f),
    //    glm::vec3(-4.0f,  2.0f, -12.0f),
    //    glm::vec3(0.0f,  0.0f, -3.0f)
    //    };

    //    VertexArray basicVAO, lightVAO;
    //    VertexBuffer vb(vertices, sizeof(vertices));

    //    VertexBufferLayout layout;
    //    layout.Push<float>(3);
    //    layout.Push<float>(3);
    //    layout.Push<float>(2);
    //    basicVAO.AddBuffer(vb, layout);

    //    lightVAO.AddBuffer(vb, layout);

    //    Renderer renderer;

    //    Texture diffTexture("res/Textures/container2.png");
    //    Texture specTexture("res/Textures/container2_specular.png");

    //    while (!glfwWindowShouldClose(window))
    //    {
    //        float currentFrame = static_cast<float>(glfwGetTime());
    //        DeltaTime = currentFrame - LastFrame;
    //        LastFrame = currentFrame;

    //        ProcessInput(window);

    //        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    //        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //        BasicShader.Bind();
    //        BasicShader.SetUniformVec3("u_ViewPos", camera.m_Position);

    //        // light properties
    //        BasicShader.SetUniformVec3("u_DirLight.direction", -0.2f, -1.0f, -0.3f);
    //        BasicShader.SetUniformVec3("u_DirLight.ambient", 0.05f, 0.05f, 0.05f);
    //        BasicShader.SetUniformVec3("u_DirLight.diffuse", 0.4f, 0.4f, 0.4f);
    //        BasicShader.SetUniformVec3("u_DirLight.specular", 0.5f, 0.5f, 0.5f);
    //        // point light 1
    //        BasicShader.SetUniformVec3("u_PointLights[0].position", pointLightPositions[0]);
    //        BasicShader.SetUniformVec3("u_PointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    //        BasicShader.SetUniformVec3("u_PointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    //        BasicShader.SetUniformVec3("u_PointLights[0].specular", 1.0f, 1.0f, 1.0f);
    //        BasicShader.SetUniform1f("u_PointLights[0].m_Constant", 1.0f);
    //        BasicShader.SetUniform1f("u_PointLights[0].m_Linear", 0.09f);
    //        BasicShader.SetUniform1f("u_PointLights[0].m_m_Quadratic", 0.032f);
    //        // point light 2
    //        BasicShader.SetUniformVec3("u_PointLights[1].position", pointLightPositions[1]);
    //        BasicShader.SetUniformVec3("u_PointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    //        BasicShader.SetUniformVec3("u_PointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    //        BasicShader.SetUniformVec3("u_PointLights[1].specular", 1.0f, 1.0f, 1.0f);
    //        BasicShader.SetUniform1f("u_PointLights[1].m_Constant", 1.0f);
    //        BasicShader.SetUniform1f("u_PointLights[1].m_Linear", 0.09f);
    //        BasicShader.SetUniform1f("u_PointLights[1].m_Quadratic", 0.032f);
    //        // point light 3
    //        BasicShader.SetUniformVec3("u_PointLights[2].position", pointLightPositions[2]);
    //        BasicShader.SetUniformVec3("u_PointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    //        BasicShader.SetUniformVec3("u_PointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    //        BasicShader.SetUniformVec3("u_PointLights[2].specular", 1.0f, 1.0f, 1.0f);
    //        BasicShader.SetUniform1f("u_PointLights[2].m_Constant", 1.0f);
    //        BasicShader.SetUniform1f("u_PointLights[2].m_Linear", 0.09f);
    //        BasicShader.SetUniform1f("u_PointLights[2].m_Quadratic", 0.032f);
    //        // point light 4
    //        BasicShader.SetUniformVec3("u_PointLights[3].position", pointLightPositions[3]);
    //        BasicShader.SetUniformVec3("u_PointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    //        BasicShader.SetUniformVec3("u_PointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    //        BasicShader.SetUniformVec3("u_PointLights[3].specular", 1.0f, 1.0f, 1.0f);
    //        BasicShader.SetUniform1f("u_PointLights[3].m_Constant", 1.0f);
    //        BasicShader.SetUniform1f("u_PointLights[3].m_Linear", 0.09f);
    //        BasicShader.SetUniform1f("u_PointLights[3].m_Quadratic", 0.032f);

    //        BasicShader.SetUniformVec3("u_SpotLight.position", camera.m_Position);
    //        BasicShader.SetUniformVec3("u_SpotLight.direction", camera.m_Front);
    //        BasicShader.SetUniformVec3("u_SpotLight.ambient", 0.0f, 0.0f, 0.0f);
    //        BasicShader.SetUniformVec3("u_SpotLight.diffuse", 1.0f, 1.0f, 1.0f);
    //        BasicShader.SetUniformVec3("u_SpotLight.specular", 1.0f, 1.0f, 1.0f);
    //        BasicShader.SetUniform1f("u_SpotLight.m_Constant", 1.0f);
    //        BasicShader.SetUniform1f("u_SpotLight.m_Linear", 0.09f);
    //        BasicShader.SetUniform1f("u_SpotLight.m_Quadratic", 0.032f);
    //        BasicShader.SetUniform1f("u_SpotLight.cutOff", glm::cos(glm::radians(12.5f)));
    //        BasicShader.SetUniform1f("u_SpotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    //        // material properties
    //        BasicShader.SetUniform1f("material.shininess", 32.0f);

    //        // texture properties
    //        diffTexture.Bind();
    //        specTexture.Bind(1);
    //        BasicShader.SetUniform1i("material.diffuse", 0);
    //        BasicShader.SetUniform1i("material.specular", 1);

    //        // mvp setting
    //        glm::mat4 model(1.0f);
    //        glm::mat4 view = camera.GetViewMatrix();
    //        glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom),
    //            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    //        BasicShader.SetUniformMat4("u_Projection", projection);
    //        BasicShader.SetUniformMat4("u_View", view);
    //        //BasicShader.SetUniformMat4("u_Model", model);
    //        renderer.Draw(basicVAO, BasicShader);
    //        for (int i = 0; i < 10; i++)
    //        {
    //            model = glm::mat4(1.0f);
    //            model = glm::translate(model, cubePositions[i]);
    //            float angle = 20.0f * i;
    //            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    //            BasicShader.SetUniformMat4("u_Model", model);
    //            renderer.Draw(basicVAO, BasicShader);
    //        }

    //        lightShader.Bind();
    //        for (unsigned int i = 0; i < 4; i++)
    //        {
    //            model = glm::mat4(1.0f);
    //            model = glm::translate(model, pointLightPositions[i]);
    //            model = glm::scale(model, glm::vec3(0.2f));
    //            glm::mat4 mvp = projection * view * model;
    //            lightShader.SetUniformMat4("u_MVP", mvp);
    //            renderer.Draw(lightVAO, lightShader);
    //        }

    //        glfwSwapBuffers(window);
    //        glfwPollEvents();
    //    }
    //}
    glfwTerminate();
    return 0;
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    GLCall(glViewport(0, 0, width, height));
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    float xpos = static_cast<float>(xPos);
    float ypos = static_cast<float>(yPos);

    if (FirstMouse)
    {
        LastX = xpos;
        LastY = ypos;
        FirstMouse = false;
    }

    float xoffset = xpos - LastX;
    float yoffset = ypos - LastY;

    LastX = xoffset;
    LastY = yoffset;

    camera.ProcessMouse(xoffset, yoffset);

}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yOffset));
}

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, DeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, DeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, DeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, DeltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, DeltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, DeltaTime);
}
