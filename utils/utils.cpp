//
//  utils.h
//  MeeDee
//
//  Created by Sergey Slyutov on 1/1/25.
//
#include "utils.h"

#include <CoreFoundation/CoreFoundation.h>

#include <CoreMIDI/CoreMIDI.h>

#include <AudioToolbox/AudioToolbox.h>

#include <QString>

#include <QComboBox>

#include <Qthread>

#include <iostream>

static int SF2PresetHeaderID = qRegisterMetaType<SF2PresetHeader>("SF2PresetHeader");


/* \fn fills up combobox with list of midi source, midi destinations and their channels
 * \param pcbmidisrc combobox for the list of midi sources
 * \param pcbmidisrcchan combobox for the list of channels of midi sources
 * \param pcbmididst combobox for the list of midi destinations
 * \param pcbmididstchan combobox for the list of channels of midi destinations */
void fillupMidiSrcDstComboBoxes(QComboBox * pcbmidisrc, QComboBox * pcbmidisrcchan, QComboBox * pcbmididst, QComboBox * pcbmididstchan)
{
    if(pcbmidisrc){
        ItemCount sourceCount = MIDIGetNumberOfSources();
        for (ItemCount i = 0; i < sourceCount; ++i) {
            MIDIEndpointRef endpoint = MIDIGetSource(i);
            CFStringRef name = nullptr;
            MIDIObjectGetStringProperty(endpoint, kMIDIPropertyName, &name);
     
            SInt32 srcid;
            MIDIObjectGetIntegerProperty(endpoint, kMIDIPropertyUniqueID, &srcid);
            
            
            MIDIObjectSetStringProperty(endpoint, kMIDIPropertyName, QString("%1-%2")
                                        .arg(QString::fromCFString(name))
                                        .arg(srcid).toCFString());
            CFRelease(name);
            
            MIDIObjectGetStringProperty(endpoint, kMIDIPropertyName, &name);
            
            pcbmidisrc->addItem(QString::fromCFString(name));
            pcbmidisrc->setItemData(pcbmidisrc->count()-1, srcid); // sourceid will be received with midi package
                                        
            CFRelease(name);
        }
    }
    
    // sources channels
    if(pcbmidisrcchan){
        for(int i = 0; i < 16; i++){
            qDebug() << QString("channel %1").arg(i+1);
            pcbmidisrcchan->addItem(QString("channel %1").arg(i+1));
            pcbmidisrcchan->setItemData(i, i+1);
        }
    }
   
    // destination combobox
    if(pcbmididst){
        ItemCount destCount = MIDIGetNumberOfDestinations();
        for(ItemCount i = 0; i < destCount; i++){
            MIDIEndpointRef endpoint = MIDIGetDestination(i);
           
            SInt32 dstid;
            MIDIObjectGetIntegerProperty(endpoint, kMIDIPropertyUniqueID, &dstid);
                       
            CFStringRef name = nullptr;
            MIDIObjectGetStringProperty(endpoint, kMIDIPropertyName, &name);
            
            MIDIObjectSetStringProperty(endpoint, kMIDIPropertyName, QString("%1-%2")
                                        .arg(QString::fromCFString(name))
                                        .arg(dstid).toCFString());
            CFRelease(name);
            
            MIDIObjectGetStringProperty(endpoint, kMIDIPropertyName, &name);
                        
            pcbmididst->addItem(QString::fromCFString(name));
            pcbmididst->setItemData(pcbmididst->count()-1, dstid);
            CFRelease(name);
        }
    }
    
    // destination channels
    if(pcbmididstchan){
        for(int i = 0 ; i < 16 ; i++){
            pcbmididstchan->addItem(QString("channel %1").arg(i+1));
            pcbmididstchan->setItemData(i, i+1);
        }
    }
};





/* \fn fillup combobox with available sampler
 * \param pcb pointer to combobox subject to fillup. */
//AudioUnit samplerUnit;

void fillupSampleCombobox(QComboBox * pcb)
{
    OSStatus result = noErr;

    AudioComponentDescription desc = {
           kAudioUnitType_MusicDevice,
           kAudioUnitSubType_Sampler,
           kAudioUnitManufacturer_Apple,
           0, 0 };

    AudioComponent samplerComponent = AudioComponentFindNext(nullptr, &desc);

    while (samplerComponent != nullptr) {

        // Get the component name
        CFStringRef nameCFString = nullptr;

        AudioComponentCopyName(samplerComponent, &nameCFString);

        QString::fromCFString(nameCFString);

        pcb->addItem(QString::fromCFString(nameCFString));

        CFRelease(nameCFString);

        samplerComponent = AudioComponentFindNext(samplerComponent, &desc);
    }
};

// ***************************************************************************************************************
// ***************************************************************************************************************
// list of instrument from default set
OSStatus LoadDLSSoundBank(const char *dlsPath, AudioUnit samplerUnit)
{
    CFURLRef dlsURL = CFURLCreateFromFileSystemRepresentation(
        kCFAllocatorDefault,
        reinterpret_cast<const UInt8 *>(dlsPath),
        strlen(dlsPath),
        false
    );

    if (!dlsURL) {
        std::cerr << "Error: Could not create CFURL from path.\n";
        return -1;
    }

    AUSamplerInstrumentData instrumentData = {
        dlsURL, kInstrumentType_DLSPreset,
        128, 0, 0
    };

    OSStatus result = AudioUnitSetProperty(
        samplerUnit,
        kAUSamplerProperty_LoadInstrument,
        kAudioUnitScope_Global,
        0,
        &instrumentData,
        sizeof(instrumentData)
    );

    CFRelease(dlsURL);
    return result;
}

void fillupInstrumentCombobox(QComboBox * pcb)
{
    // Initialize Audio Unit
     AudioComponentDescription desc = {};
     desc.componentType = kAudioUnitType_MusicDevice;
     desc.componentSubType = kAudioUnitSubType_Sampler;
     desc.componentManufacturer = kAudioUnitManufacturer_Apple;

     AudioComponent comp = AudioComponentFindNext(nullptr, &desc);
     if (!comp) {
         std::cerr << "Error: Could not find AUSampler.\n";
         return -1;
     }

    OSStatus result = noErr;
};

// Function to parse an SF2 file

bool isSoundFont(QString filename)
{
    std::ifstream file(filename.toStdWString(), std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file.\n";
        return;
    }
    
    RIFFHeader riff;
    
    file.read(reinterpret_cast<char*>(&riff), sizeof(RIFFHeader));
    
    if(strncmp(riff.chunkID, "RIFF", 4) != 0 ||
       (strncmp(riff.format, "DLS ", 4) != 0  && strncmp(riff.format, "sfbk", 4) != 0)){
        return false;
    }
    
    return true;
};

// function to read a 16-bit usinged integer from a file
uint16_t readU16(std::ifstream& file) {
    uint16_t value;
    file.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
};

// function to read a 32-bit unsigned integer from a file
uint32_t readU32(std::ifstream& file) {
    uint32_t value;
    file.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
};

std::list<SF2PresetHeader> sf2Instruments(QString filepath)
{
    std::list<SF2PresetHeader> list;
    
    std::ifstream file(filepath.toStdString(), std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << filepath.toStdString() << std::endl;
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
        std::cerr << "Preset Name: " << preset.presetName << std::endl;
        std::cerr << "Preset Number: " << preset.presetNumber << std::endl;
        std::cerr << "Bank Number: " << preset.bankNumber << std::endl;
        std::cerr << "-------------------------" << std::endl;
        
        if( preset.presetNumber == 0xff && preset.bankNumber == 0xff)
            break; // end of presets
        
        list.push_back(preset);
    }

    file.close();
    
    return  list;
}


