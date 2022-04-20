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
        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };

        glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f,  0.2f,  2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3(0.0f,  0.0f, -3.0f)
        };

        Shader modelShader("res/Shaders/Model.shader");
        Shader lightShader("res/Shaders/LightCube.shader");
        Model myModel("res/Objects/nanosuit/nanosuit.obj");

        VertexArray lightVAO;
        VertexBuffer vb(vertices, sizeof(vertices));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);

        lightVAO.AddBuffer(vb, layout);

        Renderer renderer;

        GLCall(glEnable(GL_DEPTH_TEST));
		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			DeltaTime = currentFrame - LastFrame;
			LastFrame = currentFrame;

			ProcessInput(window);

			glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            modelShader.Bind();
            modelShader.SetUniformVec3("u_ViewPos", camera.m_Position);
            modelShader.SetUniform1f("u_Material.shininess", 64.0f);

            glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT,
                0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f));
            model = glm::scale(model, glm::vec3(0.2f));
            modelShader.SetUniformMat4("u_Model", model);
            modelShader.SetUniformMat4("u_View", view);
            modelShader.SetUniformMat4("u_Projection", projection);

            modelShader.SetUniformVec3("u_DirLight.direction",glm::vec3(1.0f));
            modelShader.SetUniformVec3("u_DirLight.ambient", 0.05f, 0.05f, 0.05f);
            modelShader.SetUniformVec3("u_DirLight.diffuse", 0.4f, 0.4f, 0.4f);
            modelShader.SetUniformVec3("u_DirLight.specular", 0.5f, 0.5f, 0.5f);

            modelShader.SetUniformVec3("u_PointLight.position", 0.7f, 0.2f, 2.0f);
            modelShader.SetUniformVec3("u_PointLight.ambient", 0.05f, 0.05f, 0.05f);
            modelShader.SetUniformVec3("u_PointLight.diffuse", 0.8f, 0.8f, 0.8f);
            modelShader.SetUniformVec3("u_PointLight.specular", 1.0f, 1.0f, 1.0f);
            modelShader.SetUniform1f("u_PointLight.m_Constant", 1.0f);
            modelShader.SetUniform1f("u_PointLight.m_Linear", 0.09f);
            modelShader.SetUniform1f("u_PointLight.m_Quadratic", 0.032f);

            modelShader.SetUniformVec3("u_SpotLight.position", camera.m_Position);
            modelShader.SetUniformVec3("u_SpotLight.direction", camera.m_Front);
            modelShader.SetUniformVec3("u_SpotLight.ambient", 0.0f, 0.0f, 0.0f);
            modelShader.SetUniformVec3("u_SpotLight.diffuse", 1.0f, 1.0f, 1.0f);
            modelShader.SetUniformVec3("u_SpotLight.specular", 1.0f, 1.0f, 1.0f);
            modelShader.SetUniform1f("u_SpotLight.m_Constant", 1.0f);
            modelShader.SetUniform1f("u_SpotLight.m_Linear", 0.09f);
            modelShader.SetUniform1f("u_SpotLight.m_Quadratic", 0.032f);
            modelShader.SetUniform1f("u_SpotLight.cutOff", glm::cos(glm::radians(12.5f)));
            modelShader.SetUniform1f("u_SpotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

            myModel.Draw(modelShader);
            modelShader.Unbind();

            lightShader.Bind();
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.7f, 0.2f, 2.0f));
            model = glm::scale(model, glm::vec3(0.2f));
            glm::mat4 mvp = projection * view * model;
            lightShader.SetUniformMat4("u_MVP", mvp);
            renderer.Draw(lightVAO, lightShader);
            lightShader.Unbind();

            glfwSwapBuffers(window);
            glfwPollEvents();
		}
    }
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
