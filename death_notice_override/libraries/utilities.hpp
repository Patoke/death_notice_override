#pragma once
#include "../main/main.hpp"

#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define return_address() uintptr_t(_ReturnAddress())
#define frame_address() (uintptr_t(_AddressOfReturnAddress()) - sizeof(uintptr_t))

struct pattern {
	std::uintptr_t address;

	pattern() { address = 0; }

	pattern(std::uintptr_t _address) { address = _address; }

	template<typename t> t cast() { return reinterpret_cast<t>(address); }

	template<typename t> t scast() { return static_cast<t>(address); }

	template<typename t> t ccast() { return const_cast<t>(address); }

	template< typename t >
	t relative(std::size_t offset = 0x1) {
		if (!address)
			return t();

		std::size_t new_address = address + offset;
		std::int32_t relative_offset = *reinterpret_cast<std::int32_t*>(new_address);

		if (!relative_offset)
			return t();

		return (t)(new_address + sizeof(std::size_t) + relative_offset);
	}
};

enum print_code : int {
	printcode_default = 0,
	printcode_good,
	printcode_error,
	printcode_warning,
	printcode_max
};

namespace utilities {
	static void console_print(const char* fmt, print_code print_type, ...) {
		char msg[512]{};
		va_list args;
		va_start(args, fmt);
		vsprintf_s(msg, fmt, args);
		std::cout << "[";
		static auto console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_handle, print_type == printcode_good ? 10 : print_type == printcode_error ? 12 : print_type == printcode_warning ? 14 : 8);
		std::cout << (print_type == printcode_good ? "+" : print_type == printcode_error ? "!" : print_type == printcode_warning ? "x" : "-");
		SetConsoleTextAttribute(console_handle, 7);
		std::cout << "] ";
		std::cout << msg << std::endl;
		va_end(args);
	}

	template<unsigned int idx, typename t, typename ... args>
	static t callv_function(void* thisptr, args ... arg_list) {
		using fn = t(__thiscall*)(void*, decltype(arg_list)...);
		return (*static_cast<fn**>(thisptr))[idx](thisptr, arg_list...);
	}

	template <typename t>
	static t getv_function(void* class_pointer, size_t index) {
		return (*(t**)class_pointer)[index];
	}

	static pattern find_signature(DWORD start, DWORD end, const char* signature) {
		const char* pat = signature;
		DWORD first_match = 0;
		for (DWORD cur = start; cur < end; cur++) {
			if (!*pat)
				return pattern(first_match);

			if (*(PBYTE)pat == '\?' || *(BYTE*)cur == GET_BYTE(pat)) {
				if (!first_match)
					first_match = cur;

				if (!pat[2])
					return pattern(first_match);

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;
				else
					pat += 2;
			}
			else {
				pat = signature;
				first_match = 0;
			}
		}

		return pattern();
	}

	static pattern find_signature(HMODULE hmodule, const char* signature) {
		DWORD range_start = (DWORD)hmodule;
		MODULEINFO mod_info{};
		K32GetModuleInformation(GetCurrentProcess(), (HMODULE)range_start, &mod_info, sizeof(MODULEINFO));
		DWORD range_end = range_start + mod_info.SizeOfImage;
		return find_signature(range_start, range_end, signature);
	}

	static pattern find_signature(const char* hmodule, const char* signature) {
		return find_signature(GetModuleHandleA(hmodule), signature);
	}

	static void attach_console(const std::string& name) {
		AllocConsole();
		freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		SetConsoleTitleA(name.c_str());
	}

	static void detach_console() {
		fclose((FILE*)stdin);
		fclose((FILE*)stdout);
		HWND console_hwnd = GetConsoleWindow();
		FreeConsole();
		PostMessageW(console_hwnd, WM_CLOSE, 0, 0);
	}
}