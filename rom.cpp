#include "rom.h"

void rom::load(const char* rom_path)
{
	std::ifstream romdata(rom_path, std::ios::in | std::ios::binary);

	if (romdata.good())
	{
		storeBytes(nes_header, romdata, 0, NES_HEADER_SIZE);

		if (!std::equal(nes_header.begin(), nes_header.begin() + 4, std::begin(NES_HEADER_CONSTANT))) {
			std::cout << rom_path << " is not a valid NES file or is corrupt!" << std::endl;
			std::cout << "Press any key to exit..." << std::endl;
			std::cin.get();
			exit(1);
		}

		prg_banks = nes_header[4];
		chr_banks = nes_header[5];
		mirroring = readBit(nes_header[6], 7);
		mapper = ((nes_header[6] & 0xF0) >> 4) + (nes_header[7] & 0xF0);

		if (!prg_banks)
		{
			std::cout << rom_path << " does not have PRG-ROM. File is not a valid NES rom or is possibly corrupt!" << std::endl;
			std::cout << "Press any key to exit..." << std::endl;
			std::cin.get();
			exit(1);
		}

		prg_size = PRG_ROM_SIZE * prg_banks;
		chr_size = CHR_ROM_SIZE * chr_banks;

		storeBytes(prg_rom, romdata, NES_HEADER_SIZE, prg_size);
		storeBytes(chr_rom, romdata, prg_size, chr_size);

		std::cout << "PRG: " << DECIMAL(prg_banks) << " " << prg_size << " " << prg_rom.size() << std::endl;
		std::cout << "CHR: " << DECIMAL(chr_banks) << " " << chr_size << " " << chr_rom.size() << std::endl;
		std::cout << "Mirroring: " << QBIT_COMP(mirroring, "HORIZONTAL", "VERTICAL") << std::endl;
		std::cout << "Mapper: " << DECIMAL(mapper) << std::endl;
		
	}

	romdata.close();
	std::cin.get();
}

void rom::debugTile(uint8_t byte)
{
	uint8_t hTable = (byte >> 7) & 0x01;
	uint8_t plane = (byte >> 3) & 0x01;
	uint8_t tile = (byte >> 4) & 0x0F;
	uint8_t yOffset = byte & 0x07;

	std::cout << "Pattern Table: " << DECIMAL(hTable) << " (" << QBIT_COMP(hTable, "LEFT", "RIGHT") << ")" << std::endl;
	std::cout << "Plane: " << DECIMAL(plane) << " (" << QBIT_COMP(plane, "LESS", "MORE") << ")" << std::endl;
	std::cout << "Tile Number: " << DECIMAL(tile) << std::endl;
	std::cout << "Y Offset: " << DECIMAL(yOffset) << std::endl;
}

void rom::exportCHR(const char* output_path)
{
	if (chr_rom.size() > 0)
	{
		std::ofstream output(output_path, std::ios::binary);
		output.write(reinterpret_cast<char*>(chr_rom.data()), chr_rom.size());
		output.close();
	}
}

int rom::readBit(uint8_t byte, int bit)
{
	return (byte & (1 << bit));
}

void rom::storeBytes(std::vector<uint8_t>& container, std::ifstream& source, int seek, int size)
{
	container.resize(size);
	source.seekg(seek);
	source.read(reinterpret_cast<char*>(container.data()), size);
}
