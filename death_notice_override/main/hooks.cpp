#include "main.hpp"
#include "../libraries/utilities.hpp"

sprintf_fn sprintf_original;

void hooks_init() {
	// don't ask
	static const auto sprintf_addy = utilities::find_signature(_("client.dll"), _("55 ? ? ? ? ? ? ? ? 45 10 ? ? ? ? ? ? ? FF 00 ? ?")).cast<void*>();

	if (MH_Initialize() != MH_STATUS::MH_OK)
		return utilities::console_print(_("couldn't initialize hooks"), printcode_error);
	else
		utilities::console_print(_("hooks initialized"), printcode_good);

	if (MH_CreateHook(sprintf_addy, sprintf_hook, reinterpret_cast<void**>(&sprintf_original)) != MH_STATUS::MH_OK)
		return utilities::console_print(_("couldn't hook sprintf"), printcode_error);
	else
		utilities::console_print(_("hooked sprintf"), printcode_good);

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_STATUS::MH_OK)
		return utilities::console_print(_("couldn't enable hooks"), printcode_error);
	else
		utilities::console_print(_("hooks enabled"), printcode_good);
}