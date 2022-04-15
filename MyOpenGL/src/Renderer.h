#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

class Renderer
{
public:
    void Clear() const;
    void Draw(VertexArray& va, IndexBuffer& ib, Shader& shader) const;
};