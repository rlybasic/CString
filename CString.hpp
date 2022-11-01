#ifndef CSTRING_HPP_GUARD
#define CSTRING_HPP_GUARD
#pragma once

#include <Windows.h>
#include <Shlwapi.h>

//THIS IS A HACK!!!
//Anyways we do assert
#pragma warning(disable: 6387)
//CString.hpp update #1:
//You should patch MSVC to decrease your executable size.
//Or just use GetProcAddress (based)
#pragma comment(lib, "shlwapi.lib")

#ifndef STRINGIZE_EXPAND
#define STRINGIZE_EXPAND(str) #str
#endif

#ifndef STRINGIZE
#define STRINGIZE(str) STRINGIZE_EXPAND(str)
#endif

#ifndef assert
#define assert(condition) if (!(condition)) MessageBoxA(NULL, "Assertion failed in " __FILE__ " at " STRINGIZE(__LINE__) " line", "Assertion Error", MB_ICONERROR);
#endif

typedef struct CString {
	explicit CString() : _str(new char[] {""}) {}
	explicit CString(const char* str) : _str(StrDupA(str)) {}

	~CString() {
		VirtualFree((void*)this->_str, 0, MEM_RELEASE);
	}

	void resize(int size) {
		if (this->_str) {
			const char* shadow = this->_str;
			char* result = (char*)VirtualAlloc(NULL, size + 1,
				MEM_COMMIT | MEM_RESERVE | MEM_WRITE_WATCH, PAGE_READWRITE);

			assert(result);

			if (size < lstrlenA(shadow)) {
				this->_str = result;
			} else {
				lstrcpyA(result, shadow);
				VirtualFree((void*)this->_str, 0, MEM_RELEASE);
				this->_str = result;
			}
		}
	}

	const char* c_str() {
		assert(this->_str);

		return this->_str;
	}

	bool contains(const char* other) {
		assert(this->_str);
		assert(other);

		return StrStrA(this->_str, other);
	}

	bool contains(const CString& other) {
		assert(this->_str);
		assert(other._str);

		return StrStrA(this->_str, other._str) != NULL;
	}

	long long length() {
		assert(this->_str);

		return lstrlenA(this->_str);
	}

	void operator+=(char* other) {
		assert(this->_str);
		assert(other);

		char* result = (char*)VirtualAlloc(NULL, (lstrlenA(this->_str) + lstrlenA(other) + 1),
			MEM_COMMIT | MEM_RESERVE | MEM_WRITE_WATCH, PAGE_READWRITE);

		assert(result);

		lstrcpyA(result, this->_str);
		lstrcatA(result, other);
		this->_str = result;
	}

	void operator+=(const char* other) {
		assert(this->_str);
		assert(other);

		char* result = (char*)VirtualAlloc(NULL, (lstrlenA(this->_str) + lstrlenA(other) + 1),
			MEM_COMMIT | MEM_RESERVE | MEM_WRITE_WATCH, PAGE_READWRITE);

		assert(result);

		lstrcpyA(result, this->_str);
		lstrcatA(result, other);
		this->_str = result;
	}

	void operator+=(const CString& other) {
		assert(this->_str);
		assert(other._str);

		char* result = (char*) VirtualAlloc(NULL, (lstrlenA(this->_str) + lstrlenA(other._str) + 1), 
			MEM_COMMIT | MEM_RESERVE | MEM_WRITE_WATCH, PAGE_READWRITE);

		assert(result);

		lstrcpyA(result, this->_str);
		lstrcatA(result, other._str);
		this->_str = result;
	}

	CString operator+(const CString& other) {
		assert(this->_str);
		assert(other._str);

		char* result = (char*)VirtualAlloc(NULL, (lstrlenA(this->_str) + lstrlenA(other._str) + 1),
			MEM_COMMIT | MEM_RESERVE | MEM_WRITE_WATCH, PAGE_READWRITE);

		assert(result);

		lstrcpyA(result, this->_str);
		lstrcatA(result, other._str);

		return CString(result);
	}

	const char* operator+(const CString& other) const {
		char* result = (char*)VirtualAlloc(NULL, (lstrlenA(this->_str) + lstrlenA(other._str) + 1),
			MEM_COMMIT | MEM_RESERVE | MEM_WRITE_WATCH, PAGE_READWRITE);

		assert(result);

		lstrcpyA(result, this->_str);
		lstrcatA(result, other._str);

		return result;
	}
	
	const char* operator+(const char* other) {
		assert(this->_str);
		assert(other);

		char* result = (char*)VirtualAlloc(NULL, (lstrlenA(this->_str) + lstrlenA(other) + 1),
			MEM_COMMIT | MEM_RESERVE | MEM_WRITE_WATCH, PAGE_READWRITE);

		assert(result);

		lstrcpyA(result, this->_str);
		lstrcatA(result, other);

		return result;
	}

	CString operator+(const char* other) const {
		assert(this->_str);
		assert(other);

		char* result = (char*)VirtualAlloc(NULL, (lstrlenA(this->_str) + lstrlenA(other) + 1),
			MEM_COMMIT | MEM_RESERVE | MEM_WRITE_WATCH, PAGE_READWRITE);

		assert(result);

		lstrcpyA(result, this->_str);
		lstrcatA(result, other);

		return CString(result);
	}
	
	bool operator==(const char* other) {
		assert(this->_str);
		assert(other);

		return lstrcmpA(this->_str, other) == 0;
	}

	bool operator==(const CString& other) {
		assert(this->_str);
		assert(other._str);

		return lstrcmpA(this->_str, other._str) == 0;
	}

	void operator=(const CString& other) {
		assert(other._str);

		if (other._str) {
			VirtualFree((void*)this->_str, 0, MEM_RELEASE);

			this->_str = other._str;
		}
	}
	
	void operator=(char* other) {
		assert(other);

		if (other) {
			VirtualFree((void*)this->_str, 0, MEM_RELEASE);

			this->_str = other;
		}
	}
	
	bool operator!=(const CString& other) {
		assert(other._str);

		if (this->_str && other._str) {
			bool result1 = lstrcmpA(this->_str, other._str) == 0;
			bool result2 = StrStrA(this->_str, other._str) != NULL;

			if (result1 && result2) return false;
			if (!result1 && result2) return true;
			if (!result1 && !result2) return true;
		}

		return true;
	}

	bool operator!=(const char* other) {
		assert(other);

		if (this->_str && other) {
			bool result1 = lstrcmpA(this->_str, other) == 0;
			bool result2 = StrStrA(this->_str, other) != NULL;

			if (result1 && result2) return false;
			if (!result1 && result2) return true;
			if (!result1 && !result2) return true;
		}

		return true;
	}

	void clear() {
		this->_str = new char[] {""};
	}

	bool empty() {
		if (this->_str) {
			if (this->_str[0] == '\0') {
				return true;
			}
		}

		return false;
	}

	char* replace(const char* which, const char* other) {
		assert(this->_str);
		assert(which);
		assert(other);

		if (!contains(which)) return new char[] {""};

		char* occurence = StrStrA(this->_str, which);

		char* result = (char*)VirtualAlloc(NULL, lstrlenA(this->_str) - lstrlenA(which) + lstrlenA(other) + 1,
			MEM_COMMIT | MEM_RESERVE | MEM_WRITE_WATCH, PAGE_READWRITE);

		lstrcpynA(result, this->_str, occurence - this->_str);
		wsprintfA(result + (occurence - this->_str), "%s%s", other, occurence + strlen(which));

		return result;
	}

	CString replace(const CString& which, const CString& other) {
		assert(this->_str);
		assert(which._str);
		assert(other._str);
		
		if (!contains(which)) return CString("");

		char* occurence = StrStrA(this->_str, which._str);

		char* result = (char*)VirtualAlloc(NULL, lstrlenA(this->_str) - lstrlenA(which._str) + lstrlenA(other._str) + 1,
			MEM_COMMIT | MEM_RESERVE | MEM_WRITE_WATCH, PAGE_READWRITE);

		lstrcpynA(result, this->_str, occurence - this->_str);
		wsprintfA(result + (occurence - this->_str), "%s%s", other, occurence + strlen(which._str));

		return CString(result);
	}

	bool swap_to(const CString& other) {
		assert(this->_str);
		assert(other._str);

		VirtualFree((void*)this->_str, 0, MEM_RELEASE);
		this->_str = other._str;

		return (this->_str != NULL);
	}

	bool swap_with(CString& other) {
		assert(this->_str);
		assert(other._str);

		VirtualFree((void*)other._str, 0, MEM_RELEASE);
		other._str = this->_str;

		return (other._str != NULL);
	}
private:
	char* _str;
} CString;

#endif //CSTRING_HPP_GUARD
