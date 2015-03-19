#pragma once

#ifdef WIN32
#	define StackAllocateArray(type, c) (type*)_malloca(sizeof(type) * c)
#	define StackAllocate(type) StackAllocateArray(type, 1)
#else
#	define StackAllocateArray(type, c) (type*)alloca(sizeof(type) * c)
#	define StackAllocate(type) StackAllocateArray(type, 1)
#endif