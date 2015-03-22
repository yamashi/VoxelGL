#pragma once

template<class T>
class IManager
{
public:

	using TPtr = std::shared_ptr < T > ;
	using TWeakPtr = std::weak_ptr < T > ;

	IManager(const uint32_t aExpectedSize);

	TPtr Get(const std::string& acString, bool aFailDelete = false);

protected:

	~IManager(){};

	using TPtrMap = std::unordered_map < std::string, TWeakPtr > ;

	TPtrMap m_ptrMap;
};

template<class T>
inline IManager<T>::IManager(const uint32_t aExpectedSize)
{
	m_ptrMap.reserve(aExpectedSize);
}

template<class T>
inline std::shared_ptr<T> IManager<T>::Get(const std::string& acString, bool aFailDelete /* = false */)
{
	std::shared_ptr<T> ptr;

	auto itor = m_ptrMap.find(acString);
	if (itor != m_ptrMap.end())
	{
		ptr = itor->second.lock();

		if (aFailDelete && !ptr)
		{
			m_ptrMap.erase(itor);
		}
	}
	else
	{
		ptr = T::Create();

		m_ptrMap[acString] = ptr;
	}

	return ptr;
}