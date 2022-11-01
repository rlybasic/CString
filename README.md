# CString
Really lightweight implementation of std::string in plain WinAPI. (may be missing some features)

## How to use
It's... simple?
```cpp
#include <Windows.h>
#include "CString.hpp"

int main() {
	CString str("katch is the best");
	str.replace("katch", "xWhitey");
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.length(), NULL, NULL);
}
```
