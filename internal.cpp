// External Dependencies
#include <jni.h>

// Standard Dependencies
#include <codecvt>

// Local Dependencies
#include "internal.h"

namespace jni
{
	// Forward Declarations
	JNIEnv* env();

	namespace internal
	{
		// Base Type Signatures
		template <> std::string valueSig(const void*)			{ return "V"; }
		template <> std::string valueSig(const bool*)			{ return "Z"; }
		template <> std::string valueSig(const wchar_t*)		{ return "C"; }
		template <> std::string valueSig(const short*)			{ return "S"; }
		template <> std::string valueSig(const int*)			{ return "I"; }
		template <> std::string valueSig(const long long*)		{ return "J"; }
		template <> std::string valueSig(const float*)			{ return "F"; }
		template <> std::string valueSig(const double*)			{ return "D"; }
		template <> std::string valueSig(const String*)			{ return "Ljava/lang/String;"; }
		template <> std::string valueSig(const char* const*)	{ return "Ljava/lang/String;"; }
		template <> std::string valueSig(const wchar_t* const*)	{ return "Ljava/lang/String;"; }

		// Base Type Conversions
		template <> void valueArg(value_t* v, const bool& a)		{ ((jvalue*) v)->z = a; }
		template <> void valueArg(value_t* v, const wchar_t& a)		{ ((jvalue*) v)->c = a; }
		template <> void valueArg(value_t* v, const short& a)		{ ((jvalue*) v)->s = a; }
		template <> void valueArg(value_t* v, const int& a)			{ ((jvalue*) v)->i = a; }
		template <> void valueArg(value_t* v, const long long& a)	{ ((jvalue*) v)->j = a; }
		template <> void valueArg(value_t* v, const float& a)		{ ((jvalue*) v)->f = a; }
		template <> void valueArg(value_t* v, const double& a)		{ ((jvalue*) v)->d = a; }
		template <> void valueArg(value_t* v, const jobject& a)		{ ((jvalue*) v)->l = a; }

		/*
			String Implementations
		 */

		template <> void valueArg(value_t* v, const String& a)
		{
			((jvalue*) v)->l = env()->NewStringUTF(a.c_str());
		}

		template <> void cleanupArg<String>(value_t* v)
		{
			env()->DeleteLocalRef(((jvalue*)v)->l);
		}

		template <> void valueArg(value_t* v, const char* const& a)
		{
			((jvalue*) v)->l = env()->NewStringUTF(a);
		}

		template <> void cleanupArg<const char*>(value_t* v)
		{
			env()->DeleteLocalRef(((jvalue*) v)->l);
		}

		template <> void valueArg(value_t* v, const wchar_t* const& a)
		{
			// Convert to UTF-8 first.
			std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
			std::string bytes = cvt.to_bytes(a);

			((jvalue*) v)->l = env()->NewStringUTF(bytes.c_str());
		}

		template <> void cleanupArg<const wchar_t*>(value_t* v)
		{
			env()->DeleteLocalRef(((jvalue*) v)->l);
		}
	}
}

