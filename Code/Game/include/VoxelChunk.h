#pragma once

#include "Mesh.h"
#include "RenderList.h"
#include "TaskManager.h"

class VoxelChunk : public std::enable_shared_from_this<VoxelChunk>
{
public:

	static void* operator new(size_t sz)
	{
		void* p = malloc(sz);
		return p;
	}

	static void operator delete(void* p) 
	{
		free(p);
	}

	enum Flags
	{
		KDirty = 1 << 0,
		KUpdating = 1 << 1,
		KReady = 1 << 2
	};

	VoxelChunk();
	~VoxelChunk();

	void Revert();

	void SetAt(uint32_t aX, uint32_t aY, uint32_t aZ, uint8_t aValue);
	uint8_t GetAt(uint32_t aX, uint32_t aY, uint32_t aZ);
	const uint8_t* GetData() const { return (const uint8_t*)m_data; }

	void Draw(const glm::mat4& aProjection, const glm::mat4& aModelView, GLuint aProjectionId, GLuint aModelViewId, GLuint aMVPId);

	void Update();

	template<class... Args>
	static std::shared_ptr<VoxelChunk> Create(Args&&... args)
	{
		return std::shared_ptr<VoxelChunk>(new VoxelChunk(std::forward<Args>(args)...));
	}

	template<class... Args>
	static std::shared_ptr<VoxelChunk> CreateUpdate(Args&&... args)
	{
		auto ptr = std::shared_ptr<VoxelChunk>(new VoxelChunk(std::forward<Args>(args)...));
		ptr->Update();
		return ptr;
	}

protected:

	struct UpdateTask : public Task
	{
		UpdateTask(const std::shared_ptr<VoxelChunk>& aVoxelChunk);
		~UpdateTask();

		Task::State DoBackgroundWork();
		Task::State DoMainthreadWork();

	private:

		std::shared_ptr<VoxelChunk> m_pVoxelChunk;
		std::vector<float> m_vertices;
		std::vector<uint32_t> m_indices;
		std::vector<float> m_colors;
		std::vector<float> m_textureCoords;
	};

private:

	friend struct UpdateTask;

	bool TESTVAR{ true };

	uint32_t m_flags{ KDirty };
	Mesh m_mesh;
	RenderList m_renderList;
	GLuint m_iboId;
	uint32_t m_indices, m_vertices;
	uint8_t m_data[32][128][32]; // Randomy lel
};

using VoxelChunkPtr = std::shared_ptr < VoxelChunk > ;
using VoxelChunkWeakPtr = std::shared_ptr < VoxelChunk > ;