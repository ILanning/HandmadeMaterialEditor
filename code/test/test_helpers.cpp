#include <stdio.h>
#include <iostream>
#include "../math/Vector2.h"
#include "../math/Vector3.h"

//TODO(Ian): Should these really be macros, or is there a better solution?

#define TOCHARARRAYVECTOR(className)															\
char *ToCharArray(const className &a)															\
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

TOCHARARRAYVECTOR(Vector2)
TOCHARARRAYVECTOR(Vector3)

#undef TOCHARARRAYVECTOR

#define STREAMVECTOR(className)                                       \
std::ostream &operator<<(std::ostream &os, const className &a)		  \
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

STREAMVECTOR(Vector2)
STREAMVECTOR(Vector3)

#undef STREAMVECTOR