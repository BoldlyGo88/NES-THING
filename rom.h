#pragma once

#include <cstdint>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <bitset>

#define QBIT_COMP(x,y,z) ((x == 0) ? y : z)
#define DECIMAL(x) (static_cast<int>(x))

constexpr uint8_t NES_HEADER_CONSTANT[4] = { 0x4E, 0x45, 0x53, 0x1A };
constexpr uint8_t NES_HEADER_SIZE = 16;
constexpr auto CHR_ROM_SIZE = 8 * 1024;
constexpr auto PRG_ROM_SIZE = 16 * 1024;

class rom
{
public:
	int chr_size;
	int prg_size;

	uint8_t chr_banks;
	uint8_t mapper;
	uint8_t mirroring;
	uint8_t prg_banks;

	std::vector<uint8_t> nes_header;
	std::vector<uint8_t> prg_rom;
	std::vector<uint8_t> chr_rom;

	void load(const char* rom_path);
	void exportCHR(const char* output_path);
	void debugTile(uint8_t byte);

private:
	int readBit(uint8_t byte, int bit);
	void storeBytes(std::vector<uint8_t>& container, std::ifstream& source, int seek, int size);
};

