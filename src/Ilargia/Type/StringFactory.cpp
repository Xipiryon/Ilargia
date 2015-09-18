/*************************************************************************
* DNA Engine - http://gitweb.louis-schnellbach.com
* C++ Modular Data Oriented Game Enginee
*------------------------------------------------------------------------
* Copyright (c) 2014-2015, Louis Schnellbach
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
*************************************************************************/

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <map>

#include <Muon/Type/String.hpp>
#include <Muon/System/Log.hpp>

#include "Ilargia/System/Script.hpp"

using namespace std;
namespace ilg
{
	namespace
	{
		/*
		const asPWORD STRING_POOL = 1001;
		static const muon::String emptyString;

		static const muon::String &StringFactory(asUINT length, const char* s)
		{
			// Each engine instance has its own muon::String pool
			asIScriptContext *ctx = asGetActiveContext();
			if( ctx == 0 )
			{
				// The muon::String factory can only be called from a script
				return emptyString;
			}
			asIScriptEngine *engine = ctx->GetEngine();

			// TODO: runtime optimize: Use unordered_map if C++11 is supported, i.e. MSVC10+, gcc 4.?+
			map<const char*, muon::String> *pool = reinterpret_cast< map<const char*, muon::String>* >(engine->GetUserData(STRING_POOL));

			if( !pool )
			{
				// The muon::String pool hasn't been created yet, so we'll create it now
				asAcquireExclusiveLock();

				// Make sure the muon::String pool wasn't created while we were waiting for the lock
				pool = reinterpret_cast< map<const char*, muon::String>* >(engine->GetUserData(STRING_POOL));
				if( !pool )
				{

					pool = new map<const char*, muon::String>;
					if( pool == 0 )
					{
						ctx->SetException("Out of memory");
						asReleaseExclusiveLock();
						return emptyString;
					}
					engine->SetUserData(pool, STRING_POOL);
				}

				asReleaseExclusiveLock();
			}

			// We can't let other threads modify the pool while we query it
			asAcquireSharedLock();

			// First check if a muon::String object hasn't been created already
			map<const char*, muon::String>::iterator it;
			it = pool->find(s);
			if( it != pool->end() )
			{
				asReleaseSharedLock();
				return it->second;
			}

			asReleaseSharedLock();

			// Acquire an exclusive lock so we can add the new muon::String to the pool
			asAcquireExclusiveLock();

			// Make sure the muon::String wasn't created while we were waiting for the exclusive lock
			it = pool->find(s);
			if( it == pool->end() )
			{
				// Create a new muon::String object
				it = pool->insert(map<const char*, muon::String>::value_type(s, muon::String(s))).first;
			}

			asReleaseExclusiveLock();
			return it->second;
		}

		static void CleanupEngineStringPool(asIScriptEngine *engine)
		{
			map<const char*, muon::String> *pool = reinterpret_cast< map<const char*, muon::String>* >(engine->GetUserData(STRING_POOL));
			if( pool )
				delete pool;
		}

		static void ConstructString(String *thisPointer)
		{
			new(thisPointer) muon::String();
		}

		static void CopyConstructString(const muon::String &other, muon::String *thisPointer)
		{
			new(thisPointer) muon::String(other);
		}

		static void DestructString(String *thisPointer)
		{
			thisPointer->~String();
		}

		static muon::String& AssignString(const muon::String& str, muon::String& dest)
		{
			dest = str;
			return dest;
		}

		static muon::String& AddAssignStringToString(const muon::String &str, muon::String &dest)
		{
			// We don't register the method directly because some compilers
			// and standard libraries inline the definition, resulting in the
			// linker being unable to find the declaration.
			// Example: CLang/LLVM with XCode 4.3 on OSX 10.7
			dest += str;
			return dest;
		}

		static bool muon::StringIsEmpty(const muon::String &str)
		{
			// We don't register the method directly because some compilers
			// and standard libraries inline the definition, resulting in the
			// linker being unable to find the declaration
			// Example: CLang/LLVM with XCode 4.3 on OSX 10.7
			return str.empty();
		}

		static muon::String &AssignUInt64ToString(asQWORD i, muon::String &dest)
		{
			ostringstream stream;
			stream << i;
			dest = stream.str().c_str();
			return dest;
		}

		static muon::String &AddAssignUInt64ToString(asQWORD i, muon::String &dest)
		{
			ostringstream stream;
			stream << i;
			dest += stream.str().c_str();
			return dest;
		}

		static muon::String AddStringUInt64(const muon::String &str, asQWORD i)
		{
			ostringstream stream;
			stream << i;
			return str + stream.str().c_str();
		}

		static muon::String AddInt64String(asINT64 i, const muon::String &str)
		{
			ostringstream stream;
			stream << i;
			return muon::String(stream.str().c_str()) + str;
		}

		static muon::String &AssignInt64ToString(asINT64 i, muon::String &dest)
		{
			ostringstream stream;
			stream << i;
			dest = stream.str().c_str();
			return dest;
		}

		static muon::String &AddAssignInt64ToString(asINT64 i, muon::String &dest)
		{
			ostringstream stream;
			stream << i;
			dest += stream.str().c_str();
			return dest;
		}

		static muon::String AddStringInt64(const muon::String &str, asINT64 i)
		{
			ostringstream stream;
			stream << i;
			return str + stream.str().c_str();
		}

		static muon::String AddUInt64String(asQWORD i, const muon::String &str)
		{
			ostringstream stream;
			stream << i;
			return muon::String(stream.str().c_str()) + str;
		}

		static muon::String &AssignDoubleToString(double f, muon::String &dest)
		{
			ostringstream stream;
			stream << f;
			dest = stream.str().c_str();
			return dest;
		}

		static muon::String &AddAssignDoubleToString(double f, muon::String &dest)
		{
			ostringstream stream;
			stream << f;
			dest += stream.str().c_str();
			return dest;
		}

		static muon::String &AssignBoolToString(bool b, muon::String &dest)
		{
			ostringstream stream;
			stream << (b ? "true" : "false");
			dest = stream.str().c_str();
			return dest;
		}

		static muon::String &AddAssignBoolToString(bool b, muon::String &dest)
		{
			ostringstream stream;
			stream << (b ? "true" : "false");
			dest += stream.str().c_str();
			return dest;
		}

		static muon::String AddStringDouble(const muon::String &str, double f)
		{
			ostringstream stream;
			stream << f;
			return str + stream.str().c_str();
		}

		static muon::String AddDoubleString(double f, const muon::String &str)
		{
			ostringstream stream;
			stream << f;
			return muon::String(stream.str().c_str()) + str;
		}

		static muon::String AddStringBool(const muon::String &str, bool b)
		{
			ostringstream stream;
			stream << (b ? "true" : "false");
			return str + stream.str().c_str();
		}

		static muon::String AddBoolString(bool b, const muon::String &str)
		{
			ostringstream stream;
			stream << (b ? "true" : "false");
			return muon::String(stream.str().c_str()) + str;
		}

		static char *StringCharAt(unsigned int i, muon::String &str)
		{
			if( i >= str.size() )
			{
				// Set a script exception
				asIScriptContext *ctx = asGetActiveContext();
				if (ctx != 0)
				{
					ctx->SetException("Out of range");
				}

				// Return a null pointer
				return 0;
			}

			return &str[i];
		}

		// AngelScript signature:
		// int muon::String::opCmp(const muon::String &in) const
		static int muon::StringCmp(const muon::String &a, const muon::String &b)
		{
			const char* as = a.cStr();
			const char* bs = b.cStr();

			while(*(as++) && *(bs++))
			{
				if(*as ^ *bs)
				{
					return (*as < *bs ? -1 : 1);
				}
			}

			return 0;
		}

		static muon::String muon::StringAdd(const muon::String& a, const muon::String& b)
		{
			muon::String ca = a;
			return ca + b;
		}

		/*
		// This function returns the index of the first position where the substring
		// exists in the input muon::String. If the substring doesn't exist in the input
		// muon::String -1 is returned.
		//
		// AngelScript signature:
		// int muon::String::findFirst(const muon::String &in sub, uint start = 0) const
		static int muon::StringFindFirst(const muon::String &sub, asUINT start, const muon::String &str)
		{
		    // We don't register the method directly because the argument types change between 32bit and 64bit platforms
		    return (int)str.find(sub, start);
		}

		// This function returns the index of the last position where the substring
		// exists in the input muon::String. If the substring doesn't exist in the input
		// muon::String -1 is returned.
		//
		// AngelScript signature:
		// int muon::String::findLast(const muon::String &in sub, int start = -1) const
		static int muon::StringFindLast(const muon::String &sub, int start, const muon::String &str)
		{
		    // We don't register the method directly because the argument types change between 32bit and 64bit platforms
		    return (int)str.rfind(sub, (size_t)start);
		}

		// AngelScript signature:
		// uint muon::String::length() const
		static asUINT muon::StringSize(const muon::String &str)
		{
			// We don't register the method directly because the return type changes between 32bit and 64bit platforms
			return (asUINT)str.size();
		}


		// AngelScript signature:
		// void muon::String::resize(uint l)
		static void muon::StringResize(asUINT l, muon::String &str)
		{
			// We don't register the method directly because the argument types change between 32bit and 64bit platforms
			str.resize(l);
		}

		/*
		// AngelScript signature:
		// muon::String formatInt(int64 val, const muon::String &in options, uint width)
		static muon::String formatInt(asINT64 value, const muon::String &options, asUINT width)
		{
		    bool leftJustify = options.find("l") != muon::String::npos;
		    bool padWithZero = options.find("0") != muon::String::npos;
		    bool alwaysSign  = options.find("+") != muon::String::npos;
		    bool spaceOnSign = options.find(" ") != muon::String::npos;
		    bool hexSmall    = options.find("h") != muon::String::npos;
		    bool hexLarge    = options.find("H") != muon::String::npos;

		    muon::String fmt = "%";
		    if( leftJustify ) fmt += "-";
		    if( alwaysSign ) fmt += "+";
		    if( spaceOnSign ) fmt += " ";
		    if( padWithZero ) fmt += "0";

		#ifdef __GNUC__
		#ifdef _LP64
		    fmt += "*l";
		#else
		    fmt += "*ll";
		#endif
		#else
		    fmt += "*I64";
		#endif

		    if( hexSmall ) fmt += "x";
		    else if( hexLarge ) fmt += "X";
		    else fmt += "d";

		    muon::String buf;
		    buf.resize(width+30);
		#if _MSC_VER >= 1400 && !defined(__S3E__)
		    // MSVC 8.0 / 2005 or newer
		    sprintf_s(&buf[0], buf.size(), fmt.cStr(), width, value);
		#else
		    sprintf(&buf[0], fmt.cStr(), width, value);
		#endif
		    buf.resize(strlen(&buf[0]));

		    return buf;
		}

		// AngelScript signature:
		// muon::String formatFloat(double val, const muon::String &in options, uint width, uint precision)
		static muon::String formatFloat(double value, const muon::String &options, asUINT width, asUINT precision)
		{
		    bool leftJustify = options.find("l") != muon::String::npos;
		    bool padWithZero = options.find("0") != muon::String::npos;
		    bool alwaysSign  = options.find("+") != muon::String::npos;
		    bool spaceOnSign = options.find(" ") != muon::String::npos;
		    bool expSmall    = options.find("e") != muon::String::npos;
		    bool expLarge    = options.find("E") != muon::String::npos;

		    muon::String fmt = "%";
		    if( leftJustify ) fmt += "-";
		    if( alwaysSign ) fmt += "+";
		    if( spaceOnSign ) fmt += " ";
		    if( padWithZero ) fmt += "0";

		    fmt += "*.*";

		    if( expSmall ) fmt += "e";
		    else if( expLarge ) fmt += "E";
		    else fmt += "f";

		    muon::String buf;
		    buf.resize(width+precision+50);
		#if _MSC_VER >= 1400 && !defined(__S3E__)
		    // MSVC 8.0 / 2005 or newer
		    sprintf_s(&buf[0], buf.size(), fmt.cStr(), width, precision, value);
		#else
		    sprintf(&buf[0], fmt.cStr(), width, precision, value);
		#endif
		    buf.resize(strlen(&buf[0]));

		    return buf;
		}

		// AngelScript signature:
		// int64 parseInt(const muon::String &in val, uint base = 10, uint &out byteCount = 0)
		static asINT64 parseInt(const muon::String &val, asUINT base, asUINT *byteCount)
		{
		    // Only accept base 10 and 16
		    if( base != 10 && base != 16 )
		    {
		        if( byteCount ) *byteCount = 0;
		        return 0;
		    }

		    const char* end = &val[0];

		    // Determine the sign
		    bool sign = false;
		    if( *end == '-' )
		    {
		        sign = true;
		        end++;
		    }
		    else if( *end == '+' )
		        end++;

		    asINT64 res = 0;
		    if( base == 10 )
		    {
		        while( *end >= '0' && *end <= '9' )
		        {
		            res *= 10;
		            res += *end++ - '0';
		        }
		    }
		    else if( base == 16 )
		    {
		        while( (*end >= '0' && *end <= '9') ||
		                (*end >= 'a' && *end <= 'f') ||
		                (*end >= 'A' && *end <= 'F') )
		        {
		            res *= 16;
		            if( *end >= '0' && *end <= '9' )
		                res += *end++ - '0';
		            else if( *end >= 'a' && *end <= 'f' )
		                res += *end++ - 'a' + 10;
		            else if( *end >= 'A' && *end <= 'F' )
		                res += *end++ - 'A' + 10;
		        }
		    }

		    if( byteCount )
		        *byteCount = asUINT(size_t(end - val.cStr()));

		    if( sign )
		        res = -res;

		    return res;
		}

		// AngelScript signature:
		// double parseFloat(const muon::String &in val, uint &out byteCount = 0)
		double parseFloat(const muon::String &val, asUINT *byteCount)
		{
		    char *end;

		    // WinCE doesn't have setlocale. Some quick testing on my current platform
		    // still manages to parse the numbers such as "3.14" even if the decimal for the
		    // locale is ",".
		#if !defined(_WIN32_WCE) && !defined(ANDROID)
		    // Set the locale to C so that we are guaranteed to parse the float value correctly
		    char *orig = setlocale(LC_NUMERIC, 0);
		    setlocale(LC_NUMERIC, "C");
		#endif

		    double res = strtod(val.cStr(), &end);

		#if !defined(_WIN32_WCE) && !defined(ANDROID)
		    // Restore the locale
		    setlocale(LC_NUMERIC, orig);
		#endif

		    if( byteCount )
		        *byteCount = asUINT(size_t(end - val.cStr()));

		    return res;
		}

		// This function returns a muon::String containing the substring of the input muon::String
		// determined by the starting index and count of characters.
		//
		// AngelScript signature:
		// muon::String muon::String::substr(uint start = 0, int count = -1) const
		static muon::String muon::StringSubString(asUINT start, int count, const muon::String &str)
		{
		    // Check for out-of-bounds
		    muon::String ret;
		    if( start < str.length() && count != 0 )
		        ret = str.substr(start, count);

		    return ret;
		}

		// muon::String equality comparison.
		// Returns true iff lhs is equal to rhs.
		//
		// For some reason gcc 4.7 has difficulties resolving the
		// asFUNCTIONPR(operator==, (const muon::String &, const muon::String &)
		// makro, so this wrapper was introduced as work around.
		static bool muon::StringEquals(const muon::String& lhs, const muon::String& rhs)
		{
			return lhs == rhs;
		}
		//*/
	}

	/*
	void muon::String::registerScriptBinding(asIScriptEngine *engine)
	{
		// Register the muon::String type
		engine->RegisterObjectType("String", sizeof(String), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);

		// Register the muon::String factory
		engine->RegisterStringFactory("const muon::String &", asFUNCTION(StringFactory), asCALL_CDECL);

		// Register the cleanup callback for the muon::String pool
		engine->SetEngineUserDataCleanupCallback(CleanupEngineStringPool, STRING_POOL);

		// Register the object operator overloads
		engine->RegisterObjectBehaviour("String", asBEHAVE_CONSTRUCT,  "void f()",                    asFUNCTION(ConstructString), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectBehaviour("String", asBEHAVE_CONSTRUCT,  "void f(const muon::String &in)",    asFUNCTION(CopyConstructString), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectBehaviour("String", asBEHAVE_DESTRUCT,   "void f()",                    asFUNCTION(DestructString),  asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "String &opAssign(const muon::String &in)", asFUNCTION(AssignString), asCALL_CDECL_OBJLAST);
		// Need to use a wrapper on Mac OS X 10.7/XCode 4.3 and CLang/LLVM, otherwise the linker fails
		engine->RegisterObjectMethod("String", "String &opAddAssign(const muon::String &in)", asFUNCTION(AddAssignStringToString), asCALL_CDECL_OBJLAST);

		// Need to use a wrapper for operator== otherwise gcc 4.7 fails to compile
		engine->RegisterObjectMethod("String", "bool opEquals(const muon::String &in) const", asFUNCTIONPR(StringEquals, (const muon::String &, const muon::String &), bool), asCALL_CDECL_OBJFIRST);
		engine->RegisterObjectMethod("String", "int opCmp(const muon::String &in) const", asFUNCTION(StringCmp), asCALL_CDECL_OBJFIRST);
		engine->RegisterObjectMethod("String", "String opAdd(const muon::String &in) const", asFUNCTION(StringAdd), asCALL_CDECL_OBJFIRST);

		// The muon::String length can be accessed through methods or through virtual property
		engine->RegisterObjectMethod("String", "uint size() const", asFUNCTION(StringSize), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "void resize(uint)", asFUNCTION(StringResize), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "bool empty() const", asFUNCTION(StringIsEmpty), asCALL_CDECL_OBJLAST);

		// Register the index operator, both as a mutator and as an inspector
		// Note that we don't register the operator[] directly, as it doesn't do bounds checking
		engine->RegisterObjectMethod("String", "uint8 &opIndex(uint)", asFUNCTION(StringCharAt), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "const uint8 &opIndex(uint) const", asFUNCTION(StringCharAt), asCALL_CDECL_OBJLAST);

		// Automatic conversion from values
		engine->RegisterObjectMethod("String", "String &opAssign(double)", asFUNCTION(AssignDoubleToString), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "String &opAddAssign(double)", asFUNCTION(AddAssignDoubleToString), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "String opAdd(double) const", asFUNCTION(AddStringDouble), asCALL_CDECL_OBJFIRST);
		engine->RegisterObjectMethod("String", "String opAdd_r(double) const", asFUNCTION(AddDoubleString), asCALL_CDECL_OBJLAST);

		engine->RegisterObjectMethod("String", "String &opAssign(int64)", asFUNCTION(AssignInt64ToString), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "String &opAddAssign(int64)", asFUNCTION(AddAssignInt64ToString), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "String opAdd(int64) const", asFUNCTION(AddStringInt64), asCALL_CDECL_OBJFIRST);
		engine->RegisterObjectMethod("String", "String opAdd_r(int64) const", asFUNCTION(AddInt64String), asCALL_CDECL_OBJLAST);

		engine->RegisterObjectMethod("String", "String &opAssign(uint64)", asFUNCTION(AssignUInt64ToString), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "String &opAddAssign(uint64)", asFUNCTION(AddAssignUInt64ToString), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "String opAdd(uint64) const", asFUNCTION(AddStringUInt64), asCALL_CDECL_OBJFIRST);
		engine->RegisterObjectMethod("String", "String opAdd_r(uint64) const", asFUNCTION(AddUInt64String), asCALL_CDECL_OBJLAST);

		engine->RegisterObjectMethod("String", "String &opAssign(bool)", asFUNCTION(AssignBoolToString), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "String &opAddAssign(bool)", asFUNCTION(AddAssignBoolToString), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "String opAdd(bool) const", asFUNCTION(AddStringBool), asCALL_CDECL_OBJFIRST);
		engine->RegisterObjectMethod("String", "String opAdd_r(bool) const", asFUNCTION(AddBoolString), asCALL_CDECL_OBJLAST);

		// Utilities
		/*
		engine->RegisterObjectMethod("String", "String substr(uint start = 0, int count = -1) const", asFUNCTION(StringSubString), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "int findFirst(const muon::String &in, uint start = 0) const", asFUNCTION(StringFindFirst), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("String", "int findLast(const muon::String &in, int start = -1) const", asFUNCTION(StringFindLast), asCALL_CDECL_OBJLAST);

		engine->RegisterGlobalFunction("String formatInt(int64 val, const muon::String &in options, uint width = 0)", asFUNCTION(formatInt), asCALL_CDECL);
		engine->RegisterGlobalFunction("String formatFloat(double val, const muon::String &in options, uint width = 0, uint precision = 0)", asFUNCTION(formatFloat), asCALL_CDECL);
		engine->RegisterGlobalFunction("int64 parseInt(const muon::String &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(parseInt), asCALL_CDECL);
		engine->RegisterGlobalFunction("double parseFloat(const muon::String &in, uint &out byteCount = 0)", asFUNCTION(parseFloat), asCALL_CDECL);
	}
	//*/
}