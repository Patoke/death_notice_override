#include "main.hpp"
#include "../libraries/utilities.hpp"

DWORD __stdcall init_thread(PVOID base_address) {
	// has csgo fully loaded yet
	HANDLE csgo_window{};
	while (!(csgo_window = FindWindowA(_("Valve001"), LPCSTR('\0'))) || !GetModuleHandleA(_("serverbrowser.dll")))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	utilities::attach_console(_("Death Notice Override (.SVG to .PNG)"));
	utilities::console_print(_("by Patoke :scream:"), printcode_warning);

	hooks_init();

	// uninjecter XD
	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	// to lazey to add detach shart in the dllmain
	utilities::detach_console();
	MH_Uninitialize();

	FreeLibraryAndExitThread(static_cast<HMODULE>(base_address), 1);
}

DWORD __stdcall DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		HANDLE h = CreateThread(nullptr, NULL, init_thread, hmodule, NULL, nullptr);
		if (!h)
			throw std::exception(_("error while creating main thread"));
		else // only close the handle if the thread is valid
			CloseHandle(h);
	}

	return TRUE;
}