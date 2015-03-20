#pragma once

class Initializable
{
public:

	Initializable() : m_flags(KDirty) {}

	enum
	{
		KDirty = 1,
		KNextFlag = 1 << 1
	};

	bool IsDirty() const { return m_flags & KDirty; }

protected:

	uint32_t m_flags{ KDirty };
};