#include "util.h"

int getRandomInt(int min, int max)
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> distribution(min, max);

	return distribution(generator);
}

DWORD align(DWORD x, DWORD alignment) { return (x + alignment - 1) & ~(alignment - 1); }

DWORD rvaToFileOffset(DWORD rva, DWORD virtualAddress, DWORD pointerToRawData) { return (rva - virtualAddress) + pointerToRawData; }

DWORD fileOffsetToRva(DWORD fileOffset, DWORD virtualAddress, DWORD pointerToRawData) { return (fileOffset - pointerToRawData) + virtualAddress; }
DWORD g_encryptKey = 0xB979379E; // randomized per build

std::vector<BYTE> xorEncrypt(const std::vector<BYTE>& data, DWORD key)
{
    std::vector<BYTE> out = data;
    BYTE k0 = (BYTE)(key & 0xFF);
    BYTE k1 = (BYTE)((key >> 8) & 0xFF);
    BYTE k2 = (BYTE)((key >> 16) & 0xFF);
    BYTE k3 = (BYTE)((key >> 24) & 0xFF);
    for (size_t i = 0; i < out.size(); i++) {
        switch (i & 3) {
            case 0: out[i] ^= k0; break;
            case 1: out[i] ^= k1; break;
            case 2: out[i] ^= k2; break;
            case 3: out[i] ^= k3; break;
        }
    }
    return out;
}

std::vector<BYTE> xorDecrypt(const std::vector<BYTE>& data, DWORD key)
{
    return xorEncrypt(data, key); // XOR is symmetric
}
