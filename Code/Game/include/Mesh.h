#pragma once

class Mesh
{
public:

	enum Type
	{
		KVertices,
		KColors,
		KTextureCoords
	};

	Mesh();
	~Mesh();

	void Set(const std::vector<float>& acVertices, const std::vector<float>& acColors, const std::vector<float>& acTextureCoords);

	void Bind() const;
	void Unbind() const;

	void Enable(Type aType, uint32_t aIndex) const;

	uint32_t GetVerticesCount() const { return m_verticesSize; }

private:

	uint32_t m_verticesSize{ 0 };
	uint32_t m_colorsSize{ 0 };
	uint32_t m_textureCoordsSize{ 0 };

	GLuint m_vaoId{ 0 };
	GLuint m_vboId{ 0 };
};