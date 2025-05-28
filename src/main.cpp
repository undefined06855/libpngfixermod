/*
 * 0x3c00 seems to be a "verify srgb profile" function, and references three
 * strings relating to srgb, including our known incorrect srgb profile string,
 * returning non-zero if its invalid and zero if it is valid
 * this just returns early at the very start to return 0 instantly
 */

$on_mod(Loaded) {
	auto libpng = reinterpret_cast<uintptr_t>(GetModuleHandleA("libpng16.dll"));
	long long start = libpng + 0x3c00;
	std::vector<uint8_t> patch = {};
	
	// xor eax, eax
	patch.push_back(0x33);
	patch.push_back(0xc0);

	// ret
	patch.push_back(0xc3);

	geode::Mod::get()->patch((void*)start, patch);
}
