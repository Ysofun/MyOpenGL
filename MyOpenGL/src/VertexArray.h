#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;
	int m_IndexCount;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, const unsigned int& stride);
	void Bind() const;
	void Unbind() const;
};