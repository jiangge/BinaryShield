#include "vm_handler.h"

VMHandlerTypes VMHandler::getType() { return type; }

DWORD VMHandler::getRva() { return rva; }

DWORD VMHandler::getFileOffset() { return fileOffset; }

std::vector<BYTE> VMHandler::getBytes() { return bytes; }

void VMHandler::setRva(DWORD rva) { this->rva = rva; }

void VMHandler::setFileOffset(DWORD fileOffset) { this->fileOffset = fileOffset; }

void VMHandler::mutate()
{
    // Insert junk NOP-equivalent instructions at random positions
    // to create unique handler instances. Only mutate non-critical handlers.
    if (type == ENTER || type == EXIT || type == ANTIDBG) return;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 100);

    if (dist(rng) < 30) return; // 30% chance of mutation

    // Insert junk: push rcx; pop rcx (2 bytes, no effect)
    std::vector<BYTE> junk = {0x51, 0x59};
    std::uniform_int_distribution<int> pos_dist(0, (int)bytes.size() - 1);
    int pos = pos_dist(rng);
    bytes.insert(bytes.begin() + pos, junk.begin(), junk.end());
}

