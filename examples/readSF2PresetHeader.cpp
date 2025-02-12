#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <iomanip>

// SF2 Preset Header structure
struct SF2PresetHeader {
    char presetName[20];      // 20-byte null-terminated string
    uint16_t presetNumber;    // MIDI preset number
    uint16_t bankNumber;      // MIDI bank number
    uint16_t presetBagIndex;  // Index into the PBAG chunk
    uint32_t library;         // Reserved
    uint32_t genre;           // Reserved
    uint32_t morphology;      // Reserved
};

// Function to read a 16-bit unsigned integer from a file
uint16_t readU16(std::ifstream& file) {
    uint16_t value;
    file.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

// Function to read a 32-bit unsigned integer from a file
uint32_t readU32(std::ifstream& file) {
    uint32_t value;
    file.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

// Function to list all presets in an SF2 file
void listPresets(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return;
    }

    // Read the RIFF header
    char riffHeader[4];
    file.read(riffHeader, 4);
    if (std::string(riffHeader, 4) != "RIFF") {
        std::cerr << "Error: Not a valid RIFF file" << std::endl;
        return;
    }

    // Skip the file size (4 bytes)
    file.seekg(4, std::ios::cur);

    // Read the SF2 type
    char sf2Type[4];
    file.read(sf2Type, 4);
    if (std::string(sf2Type, 4) != "sfbk") {
        std::cerr << "Error: Not a valid SoundFont file" << std::endl;
        return;
    }

    // Locate the pdta chunk
    bool foundPdta = false;
    while (!file.eof()) {
        char chunkId[4];
        file.read(chunkId, 4);
        if (std::string(chunkId, 4) == "LIST") {
            uint32_t listSize = readU32(file);
            char listType[4];
            file.read(listType, 4);
            if (std::string(listType, 4) == "pdta") {
                foundPdta = true;
                break;
            }
            else {
                file.seekg(listSize - 4, std::ios::cur); // Skip this LIST chunk
            }
        }
        else {
            uint32_t chunkSize = readU32(file);
            file.seekg(chunkSize, std::ios::cur); // Skip this chunk
        }
    }

    if (!foundPdta) {
        std::cerr << "Error: Could not find pdta chunk" << std::endl;
        return;
    }

    // Locate the PHDR chunk within the pdta chunk
    bool foundPhdr = false;
    while (!file.eof()) {
        char chunkId[4];
        file.read(chunkId, 4);
        if (std::string(chunkId, 4) == "phdr") {
            foundPhdr = true;
            break;
        }
        else {
            uint32_t chunkSize = readU32(file);
            file.seekg(chunkSize, std::ios::cur); // Skip this chunk
        }
    }

    if (!foundPhdr) {
        std::cerr << "Error: Could not find phdr chunk" << std::endl;
        return;
    }

    // Read the PHDR chunk size
    uint32_t phdrSize = readU32(file);

    // Calculate the number of presets (each preset header is 38 bytes)
    uint32_t numPresets = phdrSize / 38;

    // Read and print each preset header
    for (uint32_t i = 0; i < numPresets; ++i) {
        SF2PresetHeader preset;
        file.read(preset.presetName, 20);
        preset.presetNumber = readU16(file);
        preset.bankNumber = readU16(file);
        preset.presetBagIndex = readU16(file);
        preset.library = readU32(file);
        preset.genre = readU32(file);
        preset.morphology = readU32(file);

        // Print the preset information
        std::cout << "Preset Name: " << preset.presetName << std::endl;
        std::cout << "Preset Number: " << preset.presetNumber << std::endl;
        std::cout << "Bank Number: " << preset.bankNumber << std::endl;
        std::cout << "-------------------------" << std::endl;
    }

    file.close();
}

int main() {
    //std::string sf2FilePath = "example.sf2"; // Replace with your SF2 file path
    std::string sf2FilePath = "41.8mg_saphyr_two_thousand_gm_gs_bank.sf2";
    listPresets(sf2FilePath);
    return 0;
}