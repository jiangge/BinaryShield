#include <iostream>
#include <string>
#include <cstdlib>
#include "pe.h"

int main(int argc, char* argv[])
{
    bool useMarkers = false;
    bool enableAntiDebug = false;
    bool enableEncrypt = true;
    const char* binaryPath = nullptr;
    DWORD startRva = 0, endRva = 0;

    // Parse flags
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--markers") { useMarkers = true; continue; }
        if (arg == "--antidebug") { enableAntiDebug = true; continue; }
        if (arg == "--no-encrypt") { enableEncrypt = false; continue; }
        if (!binaryPath) { binaryPath = argv[i]; }
        else if (!startRva) { startRva = strtoul(argv[i], nullptr, 16); }
        else if (!endRva) { endRva = strtoul(argv[i], nullptr, 16); }
    }

    if (!binaryPath || (!useMarkers && (!startRva || !endRva))) {
        std::cerr << "BinaryShield v1.1.0 — x86-64 Code Virtualizer\n";
        std::cerr << "Usage: " << argv[0] << " <binary> <start-rva> <end-rva> [flags]\n";
        std::cerr << "       " << argv[0] << " <binary> --markers [flags]\n";
        std::cerr << "Flags: --markers  Use __BINSHIELD_START/END code markers\n";
        std::cerr << "       --antidebug  Inject anti-debugger checks\n";
        std::cerr << "       --no-encrypt  Disable bytecode encryption\n";
        return 1;
    }

    if (!enableEncrypt) {
        extern DWORD g_encryptKey;
        g_encryptKey = 0; // Disable encryption
    }

    PE pe(binaryPath);
    if (!pe.load()) {
        std::cerr << "Error: Cannot load " << binaryPath << std::endl;
        return 1;
    }

    if (useMarkers) {
        pe.addFunctionByMarkers();
    } else {
        pe.addFunctionByRva(startRva, endRva);
    }

    pe.virtualizeFunctions();
    pe.addVmSection();

    std::string outPath = std::string(binaryPath) + ".protected.exe";
    pe.save(outPath);

    std::cout << "Protected: " << outPath << std::endl;
    return 0;
}
