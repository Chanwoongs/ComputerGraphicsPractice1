#pragma once

// for warning C4316
// 위 경고는 __declspec(align(byte))로 정렬되어야 하는 데이터를 member로 가지거나, 
// 할당할 때 생기는 경고임. For example, using XMMATRIX as a class member. 
// 자료형에 따라 동적할당 및 해제를 해주는 클래스
template<size_t T>
class AlignedAllocationPolicy
{
public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, T);
	}

	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};
