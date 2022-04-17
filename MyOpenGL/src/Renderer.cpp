#include "Renderer.h"
#include "ErrorCatch.h"
#include <glad/glad.h>

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(VertexArray& va, Shader& shader) const
{
    va.Bind();
    shader.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void Renderer::Draw(VertexArray& va, IndexBuffer& ib, Shader& shader) const
{
    va.Bind();
    ib.Bind();
    shader.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
