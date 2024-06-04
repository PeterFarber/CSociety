
#pragma once
#include <cstddef>

#define COUNTER_READ_CRUMB( TAG, RANK, ACC ) _AutoIdInternal::counter_crumb( TAG(), _AutoIdInternal::constant_index< RANK >(), _AutoIdInternal::constant_index< ACC >() )
#define COUNTER_READ( TAG ) COUNTER_READ_CRUMB( TAG, 1, COUNTER_READ_CRUMB( TAG, 2, COUNTER_READ_CRUMB( TAG, 4, COUNTER_READ_CRUMB( TAG, 8, \
    COUNTER_READ_CRUMB( TAG, 16, COUNTER_READ_CRUMB( TAG, 32, COUNTER_READ_CRUMB( TAG, 64, COUNTER_READ_CRUMB( TAG, 128, 0 ) ) ) ) ) ) ) )

#define COUNTER_INC( TAG ) \
namespace _AutoIdInternal { \
constexpr \
constant_index< COUNTER_READ( TAG ) + 1 > \
counter_crumb( TAG, constant_index< ( COUNTER_READ( TAG ) + 1 ) & ~ COUNTER_READ( TAG ) >, \
                                                constant_index< ( COUNTER_READ( TAG ) + 1 ) & COUNTER_READ( TAG ) > ) { return {}; } \
}

namespace _AutoIdInternal {

	template< std::size_t n >
	struct constant_index : std::integral_constant< std::size_t, n > {};

	template< typename id, std::size_t rank, std::size_t acc >
	constexpr constant_index< acc > counter_crumb(id, constant_index< rank >, constant_index< acc >) { return {}; } // found by ADL via constant_index
}



#define MEMORYPOOL_DECLARATION(__defaultNumChunks__) \
public: \
	static MemoryPool* s_pMemoryPool; \
	static void InitMemoryPool(unsigned int numChunks = __defaultNumChunks__, const char* debugName = nullptr); \
	static void DestroyMemoryPool(); \
	static void* operator new(size_t size); \
	static void operator delete(void* pPtr); \
	static void* operator new[](size_t size); \
	static void operator delete[](void* pPtr); \
private: \

