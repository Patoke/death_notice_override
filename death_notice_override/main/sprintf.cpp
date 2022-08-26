#include "main.hpp"
#include "../libraries/utilities.hpp"

CFmtStrN* sprintf_hook(CFmtStrN* thisptr, const char* format, ...) {
	// inlined InitQuietTruncation
	thisptr->m_bQuietTruncation = false;
	
	// FmtStrVSNPrintf
	{
		int result{};
		va_list arg_ptr{};
		bool bTruncated = false;
		static int scAsserted = 0;

		// V_vsnprintfRet
		{
			va_start(arg_ptr, format);

			// override death notice (kill feed) icons
			{
				static auto on_death_notice = utilities::find_signature(_("client.dll"), _("8D 8C 24 ? ? ? ? 83 C4 0C 8D 51 01")).address;
				if (return_address() == on_death_notice) {
					// modify the format so the original function uses .png instead of .svg
					format = _("file://{images}/icons/equipment/%s.png");
					std::cout << format << _(" | ") << va_arg(arg_ptr, const char*) << _("\n");
				}
			}

			result = _vsnprintf(thisptr->m_szBuf, DEFAULT_BUF_SIZE - 1, format, arg_ptr);
			va_end(arg_ptr);

			if (result > DEFAULT_BUF_SIZE - 2) {
				bTruncated = 1;
				thisptr->m_szBuf[DEFAULT_BUF_SIZE - 2] = 0;
				result = DEFAULT_BUF_SIZE - 2;
			}
			else
				bTruncated = 0;
		}

		thisptr->m_szBuf[DEFAULT_BUF_SIZE - 1] = 0;

		if (bTruncated && !(thisptr->m_bQuietTruncation) && scAsserted < 5)
			scAsserted++;

		thisptr->m_nLength = result;
	}

	return thisptr;
}