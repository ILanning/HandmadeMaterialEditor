#ifndef HANDMADE_TEST_TESTHELPERS
#define HANDMADE_TEST_TESTHELPERS

#include <stdio.h>
#include <iostream>
#include "../handmade_funcdefs.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Matrix3.h"
#include "../math/Matrix4.h"
#include "../math/Quaternion.h"

inline GENERAL_ALLOC_MEMORY(TestAlloc)
{
	uint8 *memBlock = (uint8 *)malloc((size_t)size);
	if (outSuccess)
	{
		*outSuccess = true;
	}
	return memBlock;
}

inline GENERAL_DEALLOC_MEMORY(TestDealloc)
{
	free(memory);
	if (outSuccess)
	{
		*outSuccess = true;
	}
}

//TODO(Ian): Should these really be macros, or is there a better solution?

#define HANDMADE_TO_CHARARRAY_VECTOR(className)													\
inline char *ToCharArray(const className &a)													\
{																								\
	const int bufferSize = 21;																	\
	const int componentCount = className::ElementCount;											\
	char componentString[componentCount][bufferSize];											\
	int componentStringSize[componentCount];													\
																								\
	/* Build out float strings */																\
	for (int componentIter = 0; componentIter < componentCount; componentIter++)				\
	{																							\
		componentStringSize[componentIter] = snprintf(componentString[componentIter], 			\
										      bufferSize, "%f", a.elements[componentIter]);		\
		if (componentStringSize[componentIter] > bufferSize)									\
		{																						\
			componentStringSize[componentIter] = bufferSize;									\
		}																						\
	}																							\
																								\
	/* Format: (x, y, z)\0 */																	\
	char *result = new char[(bufferSize + 2) * componentCount + 1];								\
																								\
	int resultIter = 0;																			\
	result[resultIter++] = '(';																	\
	for (int componentIter = 0; componentIter < componentCount; componentIter++)				\
	{																							\
		for (int i = 0; i < componentStringSize[componentIter]; i++)							\
		{																						\
			result[resultIter++] = componentString[componentIter][i];							\
		}																						\
		if (componentIter < componentCount - 1)													\
		{																						\
			result[resultIter++] = ',';															\
			result[resultIter++] = ' ';															\
		}																						\
	}																							\
	result[resultIter++] = ')';																	\
	result[resultIter] = '\0';																	\
																								\
	return result;																				\
}

HANDMADE_TO_CHARARRAY_VECTOR(Vector2)
HANDMADE_TO_CHARARRAY_VECTOR(Vector3)
HANDMADE_TO_CHARARRAY_VECTOR(Quaternion)

#undef HANDMADE_TO_CHARARRAY_VECTOR

#define HANDMADE_STREAM_VECTOR(className)                             \
inline std::ostream &operator<<(std::ostream &os, const className &a) \
{																	  \
	os << '(';														  \
	for (int i = 0; i < className::ElementCount; i++)				  \
	{																  \
		os << a.elements[i];										  \
		if (i < className::ElementCount - 1)						  \
		{															  \
			os << ", ";												  \
		}															  \
	}																  \
	return os << ")";												  \
}

HANDMADE_STREAM_VECTOR(Vector2)
HANDMADE_STREAM_VECTOR(Vector3)
HANDMADE_STREAM_VECTOR(Quaternion)

#undef HANDMADE_STREAM_VECTOR

#define HANDMADE_STREAM_MATRIX(className)                                \
inline std::ostream &operator<<(std::ostream &os, const className &a)	 \
{																		 \
	os << "\n";															 \
	for (int y = 0; y < className::RowCount; y++)						 \
	{																	 \
		os << "[";														 \
		for (int x = 0; x < className::RowCount; x++)					 \
		{																 \
			os << a.m[y][x];											 \
			if (x < className::RowCount - 1)							 \
			{															 \
				os << ", ";												 \
			}															 \
		}																 \
		os << "]\n";													 \
	}																	 \
	return os;															 \
}

HANDMADE_STREAM_MATRIX(Matrix3)
HANDMADE_STREAM_MATRIX(Matrix4)

#undef HANDMADE_STREAM_MATRIX

/*#define HANDMADE_CHECK_WITHIN_BOUNDS(className)						\
inline bool CheckWithinBounds(const className &a, const className &b)	\
{																		\
	for (int i = 0; i < className::ElementCount; i++)					\
	{																	\
		real32 difference = a.elements[i] - b.elements[i];				\
		if (difference > 0.00001 || difference < -0.00001)				\
		{																\
			return false;												\
		}																\
	}																	\
	return true;														\
}

HANDMADE_CHECK_WITHIN_BOUNDS(Vector2)
HANDMADE_CHECK_WITHIN_BOUNDS(Vector3)
HANDMADE_CHECK_WITHIN_BOUNDS(Quaternion)
HANDMADE_CHECK_WITHIN_BOUNDS(Matrix3)
HANDMADE_CHECK_WITHIN_BOUNDS(Matrix4)

#undef HANDMADE_CHECK_WITHIN_BOUNDS*/

template<typename T>
inline bool CheckWithinBounds(const T &a, const T &b)
{
	for (int i = 0; i < T::ElementCount; i++)
	{
		real32 difference = a.elements[i] - b.elements[i];
		if (difference > 0.00001 || difference < -0.00001)
		{
			return false;
		}
	}
	return true;
}

template bool CheckWithinBounds(const Vector2&, const Vector2&);
template bool CheckWithinBounds(const Vector3&, const Vector3&);
template bool CheckWithinBounds(const Quaternion&, const Quaternion&);
template bool CheckWithinBounds(const Matrix3&, const Matrix3&);
template bool CheckWithinBounds(const Matrix4&, const Matrix4&);

#endif