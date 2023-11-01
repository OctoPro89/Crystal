#include "crystalpch.h"
#include "ScriptGlue.h"
#include <mono/metadata/object.h>

namespace Crystal {
#define CRYSTAL_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Crystal.InternalCalls::"#Name, Name)

	static void NativeLog(MonoString* string, int value)
	{
		char* str = mono_string_to_utf8(string);
		std::cout << str << value << "\n";
		mono_free(str);
	}

	void ScriptGlue::RegisterFunctions()
	{
		CRYSTAL_ADD_INTERNAL_CALL(NativeLog);
	}
}
