#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;
	unsigned int m_IndexCount;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, const unsigned int& stride);
	void Bind() const;
	void Unbind() const;
	inline unsigned int GetRenderID() const { return m_RendererID; }
};