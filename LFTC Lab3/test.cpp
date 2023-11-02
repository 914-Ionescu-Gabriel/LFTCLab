#include "pch.h"
#include "../SymbolTable/SymbolTable.h"
#include "CppUnitTest.h"
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TEST
{
	TEST_CLASS(TEST)
	{
	public:

		SymbolTable<int> intTable;
		SymbolTable<std::string> stringTable;

		
		TEST_METHOD(IntTest)
		{
			intTable.SetSymbol(42);
			assert(intTable.HasValue(42));
			assert(!intTable.HasValue(100));

		}
		TEST_METHOD(StringTest)
		{
			stringTable.SetSymbol("hello");
			stringTable.SetSymbol("world");
			assert(stringTable.HasValue("hello"));
			assert(!stringTable.HasValue("helloworld"));
		}
		TEST_METHOD(GetFind)
		{
			auto hashAndIndex = intTable.GetHashAndIndex(42);
			assert(hashAndIndex.first == intTable.HashValue(42));
			assert(hashAndIndex.second == 0);

			auto foundValue = intTable.FindByHashAndIndex(hashAndIndex.first, hashAndIndex.second);
			assert(foundValue == 42);

			hashAndIndex = stringTable.GetHashAndIndex("world");
			assert(hashAndIndex.first == stringTable.HashValue("world"));
			assert(hashAndIndex.second == 1);

			std::string foundValue1 = stringTable.FindByHashAndIndex(hashAndIndex.first, hashAndIndex.second);
			assert(foundValue1 == "world");
		}
		TEST_METHOD(Collision)
		{
			intTable.SetSymbol(4061);  // This should collide with 42
			auto hashAndIndex = intTable.GetHashAndIndex(4061);
			assert(hashAndIndex.first == intTable.HashValue(4061));
			assert(hashAndIndex.second == 1); // It should be placed at the next index due to collision
			auto foundValue = intTable.FindByHashAndIndex(hashAndIndex.first, hashAndIndex.second);
			assert(foundValue == 4061);
		}
		TEST_METHOD(COllision_Handle)
		{
			intTable.SetSymbol(8113);
			intTable.SetSymbol(8132);
			auto hashAndIndex = intTable.GetHashAndIndex(8113);
			assert(hashAndIndex.first == intTable.HashValue(8113));
			assert(hashAndIndex.second == 2); // Should be the third value in the slot
			auto foundValue = intTable.FindByHashAndIndex(hashAndIndex.first, hashAndIndex.second);
			assert(foundValue == 8113);

		}
	};
}
