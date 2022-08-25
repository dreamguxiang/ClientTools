#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <string_view>
#include <windows.h>

#include <algorithm>
#include <fstream>
#include <unordered_set>

#include "cxxopts.hpp"
#include "../PortableExecutable/pe_bliss.h"

#include <windows.h>


using namespace pe_bliss;
inline void Pause(bool Pause) {
	if (Pause)
		system("pause");
}

int main(int argc, char** argv) {
	if (argc == 1) {
		SetConsoleCtrlHandler([](DWORD signal) -> BOOL {return TRUE; }, TRUE);
		EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE,
			MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}

	cxxopts::Options options("LLPeEditor", "Editor For BDS");
	options.allow_unrecognised_options();
	options.add_options()
		("exe", "Exe file name", cxxopts::value<std::string>()
			->default_value("Minecraft.Windows.exe"))
		("dll", "Dll file name", cxxopts::value<std::string>()
			->default_value("ClientPreLoader.dll"))
		("h,help", "Print usage");

	auto optionsResult = options.parse(argc, argv);

	if (optionsResult.count("help")) {
		std::cout << options.help() << std::endl;
		exit(0);
	}


	std::string mExeFile = optionsResult["exe"].as<std::string>();
	std::string mDllFile = optionsResult["dll"].as<std::string>();
	
	std::ifstream             OriginalBDS;
	std::ofstream             ModifiedBDS;
	pe_base* OriginalBDS_PE = nullptr;

	OriginalBDS.open(mExeFile, std::ios::in | std::ios::binary);

	if (OriginalBDS) {
		try {
			ModifiedBDS.open("mod.exe", std::ios::out | std::ios::binary | std::ios::trunc);
			if (!ModifiedBDS) {
				std::cout << "[Err] Cannot create mod.exe" << std::endl;
				Pause(1);
				return -1;
			}
			OriginalBDS_PE = new pe_base(pe_factory::create_pe(OriginalBDS));

			imported_functions_list imports(get_imported_functions(*OriginalBDS_PE));

			import_library preLoader;
			preLoader.set_name(mDllFile);

			imported_function func;
			func.set_name("imp_func");
			func.set_iat_va(0x1);

			preLoader.add_import(func);
			imports.push_back(preLoader);

			section ImportSection;
			ImportSection.get_raw_data().resize(1);
			ImportSection.set_name("ImpFunc");
			ImportSection.readable(true).writeable(true);
			section& attachedImportedSection = OriginalBDS_PE->add_section(ImportSection);
			rebuild_imports(*OriginalBDS_PE, imports, attachedImportedSection, import_rebuilder_settings(true, false));



			rebuild_pe(*OriginalBDS_PE, ModifiedBDS);
			ModifiedBDS.close();
			std::cout << "[Info] Created" << std::endl;
		}
		catch (pe_exception e) {
			std::cout << "[Error] Failed to rebuild mod.exe" << std::endl;
			std::cout << "[Error] " << e.what() << std::endl;
			ModifiedBDS.close();
			std::filesystem::remove(std::filesystem::path("mod.exe"));
		}
		catch (...) {
			std::cout << "[Error] Failed to rebuild mod.exe with unk err" << std::endl;
			ModifiedBDS.close();
			std::filesystem::remove(std::filesystem::path("mod.exe"));
		}
	}
	Pause(1);
	return 0;
}
