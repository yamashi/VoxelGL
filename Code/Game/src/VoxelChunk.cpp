#include "VoxelChunk.h"
#include "ShaderManager.h"

VoxelChunk::VoxelChunk()
{
	for (uint32_t x = 0; x < 32; ++x)
		for (uint32_t y = 0; y < 128; ++y)
			for (uint32_t z = 0; z < 32; ++z)
			{
				SetAt(x, y, z, x * 4 < y ? 0 : 1);
			}
}

VoxelChunk::~VoxelChunk()
{
	if (glIsBuffer(m_iboId))
		glDeleteBuffers(1, &m_iboId);
}

void VoxelChunk::Revert()
{
	TESTVAR = !TESTVAR;

	for (uint32_t x = 0; x < 32; ++x)
		for (uint32_t y = 0; y < 128; ++y)
			for (uint32_t z = 0; z < 32; ++z)
			{
				SetAt(x, y, z, x * 4 < y ? TESTVAR : !TESTVAR);
			}

	Update();
}


void VoxelChunk::SetAt(uint32_t aX, uint32_t aY, uint32_t aZ, uint8_t aValue)
{
	m_data[aX][aY][aZ] = aValue;

	m_flags |= KDirty;
}

uint8_t VoxelChunk::GetAt(uint32_t aX, uint32_t aY, uint32_t aZ)
{
	return m_data[aX][aY][aZ];
}

void VoxelChunk::Update()
{
	if ((m_flags & KDirty) && !(m_flags & KUpdating))
	{
		m_flags |= KUpdating;
		m_flags &= ~KDirty;
		TaskManager::GetInstance().AddBackground(new VoxelChunk::UpdateTask(shared_from_this()));
	}
}

void VoxelChunk::Draw(const glm::mat4& aProjection, const glm::mat4& aModelView, GLuint aProjectionId, GLuint aModelViewId, GLuint aMVPId)
{
	if (!glIsBuffer(m_iboId)) return;

	m_renderList.Bind();
	{
		glUniformMatrix4fv(aProjectionId, 1, GL_FALSE, value_ptr(aProjection));
		glUniformMatrix4fv(aModelViewId, 1, GL_FALSE, value_ptr(aModelView));
		glUniformMatrix4fv(aMVPId, 1, GL_FALSE, value_ptr(aProjection * aModelView));

		glDrawElements(GL_TRIANGLES, m_indices, GL_UNSIGNED_INT, NULL);
	}
	m_renderList.Unbind();
}

VoxelChunk::UpdateTask::UpdateTask(const std::shared_ptr<VoxelChunk>& aVoxelChunk)
	: m_pVoxelChunk(aVoxelChunk)
{
	m_vertices.reserve(1000000);
	m_indices.reserve(1000000);
	m_colors.reserve(1000000);
	m_textureCoords.reserve(1000000);
}

VoxelChunk::UpdateTask::~UpdateTask()
{
}

Task::State VoxelChunk::UpdateTask::DoBackgroundWork()
{
	auto start = std::chrono::high_resolution_clock::now();

	int32_t indices[33][129][33];
	for (int i = 0; i < 33; ++i)
		for (int j = 0; j < 129; ++j)
			for (int k = 0; k < 33; ++k)
				indices[i][j][k] = -1;

	auto AddVertex = [&, this](int32_t aX, int32_t aY, int32_t aZ)
	{
		int32_t idx = indices[aX][aY][aZ];
		if (idx == -1)
		{
			m_vertices.push_back((float)aX);
			m_vertices.push_back((float)aY); 
			m_vertices.push_back((float)aZ); 

			m_colors.push_back(1.0f);
			m_colors.push_back(0.0f);
			m_colors.push_back(0.0f);

			idx = m_vertices.size() / 3 - 1;
			indices[aX][aY][aZ] = idx;
		}
		m_indices.push_back( (uint32_t)idx);
	};

	for (int32_t x = 0; x < 32; ++x)
		for (int32_t y = 0; y < 128; ++y)
			for (int32_t z = 0; z < 32; ++z)
			{
				if (m_pVoxelChunk->m_data[x][y][z] != 0)
				{
					// left 
					if ((x < 31 && m_pVoxelChunk->m_data[x + 1][y][z] == 0) || x == 31)
					{
						AddVertex(1 + x, y, 1 + z);
						AddVertex(1 + x, y, z);
						AddVertex(1 + x, 1 + y, z);

						AddVertex(1 + x, 1 + y, 1 + z);
						AddVertex(1 + x, y, 1 + z);
						AddVertex(1 + x, 1 + y, z);
					}

					// right
					if ((x > 0 && m_pVoxelChunk->m_data[x - 1][y][x] == 0) || x == 0)
					{
						x--;

						AddVertex(1 + x, y, 1 + z);
						AddVertex(1 + x, 1 + y, 0 + z);
						AddVertex(1 + x, 0 + y, 0 + z);

						AddVertex(1 + x, 0 + y, 1 + z);
						AddVertex(1 + x, 1 + y, 1 + z);
						AddVertex(1 + x, 1 + y, 0 + z);

						x++;
					}

					// front
					if ((z < 31 && m_pVoxelChunk->m_data[x][y][z + 1] == 0) || z == 31)
					{
						z++;

						AddVertex(0 + x, 0 + y, 0 + z);
						AddVertex(1 + x, 0 + y, 0 + z);
						AddVertex(1 + x, 1 + y, 0 + z);

						AddVertex(0 + x, 1 + y, 0 + z);
						AddVertex(0 + x, 0 + y, 0 + z);
						AddVertex(1 + x, 1 + y, 0 + z);
						z--;
					}

					// back
					if ((z > 0 && m_pVoxelChunk->m_data[x][y][z - 1] == 0) || z == 0)
					{
						AddVertex(0 + x, 0 + y, 0 + z);
						AddVertex(1 + x, 1 + y, 0 + z);
						AddVertex(1 + x, 0 + y, 0 + z);


						AddVertex(0 + x, 0 + y, 0 + z);
						AddVertex(0 + x, 1 + y, 0 + z);
						AddVertex(1 + x, 1 + y, 0 + z);
					}

					// top
					if ((y < 127 && m_pVoxelChunk->m_data[x][y + 1][z] == 0) || y == 127)
					{
						y++;

						AddVertex(1 + x, 0 + y, 1 + z);
						AddVertex(1 + x, 0 + y, 0 + z);
						AddVertex(0 + x, 0 + y, 1 + z);

						AddVertex(0 + x, 0 + y, 0 + z);
						AddVertex(0 + x, 0 + y, 1 + z);
						AddVertex(1 + x, 0 + y, 0 + z);

						y--;
					}

					// bottom
					if ((y > 0 && m_pVoxelChunk->m_data[x][y - 1][z] == 0) || y == 0)
					{
						AddVertex(1 + x, 0 + y, 1 + z);
						AddVertex(0 + x, 0 + y, 1 + z);
						AddVertex(1 + x, 0 + y, 0 + z);

						AddVertex(0 + x, 0 + y, 1 + z);
						AddVertex(0 + x, 0 + y, 0 + z);
						AddVertex(1 + x, 0 + y, 0 + z);
					}
				}
			}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<uint32_t, std::milli> >(end - start);

	std::cout << "took : " << duration.count() << " ms" << std::endl;

	return Task::KNeedMainthreadTime;
}

Task::State VoxelChunk::UpdateTask::DoMainthreadWork()
{
	if (glIsBuffer(m_pVoxelChunk->m_iboId))
		glDeleteBuffers(1, &m_pVoxelChunk->m_iboId);

	glGenBuffers(1, &m_pVoxelChunk->m_iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pVoxelChunk->m_iboId);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * m_indices.size(), m_indices.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_pVoxelChunk->m_indices = m_indices.size();
	m_pVoxelChunk->m_vertices = m_vertices.size();

	m_pVoxelChunk->m_mesh.Set(m_vertices, m_colors, m_textureCoords);

	m_pVoxelChunk->m_renderList.Setup();

	m_pVoxelChunk->m_renderList.Bind();
	{
		m_pVoxelChunk->m_mesh.Bind();

		m_pVoxelChunk->m_mesh.Enable(Mesh::KVertices, 0);
		m_pVoxelChunk->m_mesh.Enable(Mesh::KColors, 1);

		m_pVoxelChunk->m_mesh.Unbind();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pVoxelChunk->m_iboId);
	}
	m_pVoxelChunk->m_renderList.Unbind();

	m_pVoxelChunk->m_flags &= ~VoxelChunk::KUpdating;

	return Task::KFinished;
}