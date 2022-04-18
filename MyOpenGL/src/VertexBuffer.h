#pragma once
#include <vector>

struct Vertex;

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const std::vector<Vertex>& data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};