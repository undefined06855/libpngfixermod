/*
 * 0x3c00 seems to be a "verify srgb profile" function, and references three
 * strings relating to srgb, including our known incorrect srgb profile string,
 * returning non-zero if its invalid and zero if it is valid
 * 
 * this just returns early at the very start to return 0 instantly
 */

$on_mod(Loaded) {
    geode::log::debug("Patching verify sRGB profile...");
    auto libpng = reinterpret_cast<uintptr_t>(GetModuleHandleA("libpng16.dll"));
    std::vector<uint8_t> patch = {};
    
    // xor eax, eax
    patch.push_back(0x33);
    patch.push_back(0xc0);

    // ret
    patch.push_back(0xc3);

    auto res = geode::Mod::get()->patch((void*)(libpng + 0x3c00), patch);
    if (res.isErr()) {
        geode::log::warn("Failed to patch libpng!");
    }
}

/*
 * 0xd51a is in the exported png_read_image function, this patches out the call
 * to png_warning which tells you that interlace handling should be turned on
 * when using png_read_image
 * 
 * the best way to fix this is probably to turn on whatever the hell it wants
 * but this works
 *
 * maybe i should just patch out the logging function actually
*/

$on_mod(Loaded) {
    geode::log::debug("Patching interlace handling...");
    auto libpng = reinterpret_cast<uintptr_t>(GetModuleHandleA("libpng16.dll"));
    std::vector<uint8_t> patch = {};
    
    // 5 nops
    patch.push_back(0x90);
    patch.push_back(0x90);
    patch.push_back(0x90);
    patch.push_back(0x90);
    patch.push_back(0x90);

    auto res = geode::Mod::get()->patch((void*)(libpng + 0xd51a), patch);
    if (res.isErr()) {
        geode::log::warn("Failed to patch libpng!");
    }
}
