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

//#include <Foundation/Foundation.h>

#include <QString>

#include <QComboBox>

#include <Qthread>

#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>
#include <iostream>
#include <thread>
#include <chrono>

// Global audio graph
AUGraph audioGraph;
AudioUnit samplerUnit;

// Function to create an AUGraph and connect AUSampler to the output
OSStatus CreateAudioGraph() {
    OSStatus status;
    
    // 1️⃣ Create a new AUGraph
    status = NewAUGraph(&audioGraph);
    if (status != noErr) return status;

    // 2️⃣ Create AUSampler node
    AudioComponentDescription samplerDesc = {};
    samplerDesc.componentType = kAudioUnitType_MusicDevice;
    samplerDesc.componentSubType = kAudioUnitSubType_Sampler;
    samplerDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
    
    AUNode samplerNode;
    status = AUGraphAddNode(audioGraph, &samplerDesc, &samplerNode);
    if (status != noErr) return status;

    // 3️⃣ Create output node
    AudioComponentDescription outputDesc = {};
    outputDesc.componentType = kAudioUnitType_Output;
    outputDesc.componentSubType = kAudioUnitSubType_DefaultOutput;
    outputDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
    
    AUNode outputNode;
    status = AUGraphAddNode(audioGraph, &outputDesc, &outputNode);
    if (status != noErr) return status;

    // 4️⃣ Open the graph and get unit references
    status = AUGraphOpen(audioGraph);
    if (status != noErr) return status;

    status = AUGraphNodeInfo(audioGraph, samplerNode, nullptr, &samplerUnit);
    if (status != noErr) return status;

    AudioUnit outputUnit;
    status = AUGraphNodeInfo(audioGraph, outputNode, nullptr, &outputUnit);
    if (status != noErr) return status;

    // 5️⃣ Connect sampler to output
    status = AUGraphConnectNodeInput(audioGraph, samplerNode, 0, outputNode, 0);
    if (status != noErr) return status;

    // 6️⃣ Start the audio graph
    status = AUGraphInitialize(audioGraph);
    if (status != noErr) return status;

    status = AUGraphStart(audioGraph);
    return status;
}

int playtestnote() {
    OSStatus result = CreateAudioGraph();
    if (result != noErr) {
        std::cerr << "Error: Could not set up audio graph.\n";
        return -1;
    }

    std::cout << "Audio graph started!\n";
    
    
    
    const char *sf2Path = "/System/Library/Components/CoreAudio.component/Contents/Resources/gs_instruments.dls";
    
    CFURLRef soundFontURL = CFURLCreateFromFileSystemRepresentation( kCFAllocatorDefault, (const UInt8 *)sf2Path, strlen(sf2Path), false );
    
    // fill out a instrument data structure
     AUSamplerInstrumentData instdata;
    instdata.fileURL  = soundFontURL; //(CFURLRef) bankURL;
     instdata.instrumentType = kInstrumentType_DLSPreset;
    instdata.bankMSB  = 121; //kAUSampler_DefaultMelodicBankMSB;
    //instdata.bankMSB  = kAUSampler_DefaultPercussionBankMSB;
    
    instdata.bankLSB  = 0; //kAUSampler_DefaultBankLSB;
    UInt8 presetNumber = 1;
     instdata.presetID = (UInt8) presetNumber;
  
     // set the kAUSamplerProperty_LoadPresetFromBank property
     result = AudioUnitSetProperty(samplerUnit,
                                   kAUSamplerProperty_LoadInstrument,
                                   kAudioUnitScope_Global,
                                   0,
                                   &instdata,
                                   sizeof(instdata));

    // 7️⃣ Play a MIDI note (Middle C)
    UInt8 note = 60;   // Middle C
    UInt8 velocity = 100;  // Volume

    std::cout << "Playing note: " << (int)note << std::endl;
    MusicDeviceMIDIEvent(samplerUnit, 0x90, note, velocity, 0);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    MusicDeviceMIDIEvent(samplerUnit, 0x80, note, 0, 0);
    std::cout << "Note Off.\n";

    return 0;
}

void assignInstrument();


/* \fn fillup combobox with available sampler
 * \param pcb pointer to combobox subject to fillup. */
//AudioUnit samplerUnit;

void fillupSampleCombobox(QComboBox * pcb)
{
    //playtestnote();
    
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
        
       
        
        // load sound fonr to sampler
        result = AudioComponentInstanceNew(samplerComponent, &samplerUnit);
        if(result == noErr){
            qDebug() << "audio component instance success";
        }else{
            qDebug() << "audio component instance error: " << result;
        }
        
        result = AudioUnitInitialize(samplerUnit);
        if(result == noErr){
            qDebug() << "audio unit initialize success";
        }else{
            qDebug() << "audio unit initialize error: " << result;
        }
      
        
        
        
        AUSamplerInstrumentData auPreset;
        const char *sf2Path = "/System/Library/Components/CoreAudio.component/Contents/Resources/gs_instruments.dls";
        
        CFURLRef soundFontURL = CFURLCreateFromFileSystemRepresentation( kCFAllocatorDefault, (const UInt8 *)sf2Path, strlen(sf2Path), false );
        
        // fill out a instrument data structure
         AUSamplerInstrumentData instdata;
        instdata.fileURL  = soundFontURL; //(CFURLRef) bankURL;
        instdata.instrumentType = kInstrumentType_DLSPreset;
        //instdata.bankMSB  = 121; //kAUSampler_DefaultMelodicBankMSB;
        instdata.bankLSB  = kAUSampler_DefaultBankLSB;
        UInt8 presetNumber = 0;
         instdata.presetID = (UInt8) presetNumber;
      
         // set the kAUSamplerProperty_LoadPresetFromBank property
         result = AudioUnitSetProperty(samplerUnit,
                                       kAUSamplerProperty_LoadInstrument,
                                       kAudioUnitScope_Global,
                                       0,
                                       &instdata,
                                       sizeof(instdata));
      
        qDebug () << "AutoUnitSetProperty ressult: "  << result;
           
         // check for errors
        
        UInt32 instrumentCount = 0;
        UInt32 dataSize = sizeof(instrumentCount);
           
        result = AudioUnitGetProperty(
               samplerUnit,                          // The AudioUnit instance
               kMusicDeviceProperty_InstrumentCount, // Property ID
               kAudioUnitScope_Global,               // Scope
               0,                                    // Element (always 0 for global scope)
               &instrumentCount,                     // Output value
               &dataSize                             // Size of output value
        );

        if (result == noErr) {
            printf("Number of instruments loaded: %u\n", instrumentCount);
        } else {
            printf("Error getting instrument count: %d\n", (int)result);
        }
        
        UInt32 noteOnCommand = 0x90; // midi note on
        UInt32 noteOffCommand = 0x80; //
        UInt32 note          = 60;   // Middle C
        UInt32 velocity      = 100;  // Strength of the note
        MusicDeviceMIDIEvent(samplerUnit, noteOnCommand, note, velocity, 0);
        QThread::msleep(1000);
        MusicDeviceMIDIEvent(samplerUnit, noteOffCommand, note, velocity, 0);
        
        samplerComponent = AudioComponentFindNext(samplerComponent, &desc);
        
    }
    
    playtestnote();
};

// ***************************************************************************************************************
// ***************************************************************************************************************
// list of instrument from default set
OSStatus LoadDLSSoundBank(const char *dlsPath) {
    
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

// List available instruments in the loaded DLS file
void ListInstruments() {
    UInt32 instrumentCount = 0;
    UInt32 size = sizeof(instrumentCount);
    
    OSStatus result = AudioUnitGetProperty(
        samplerUnit,
        kMusicDeviceProperty_InstrumentCount,
        kAudioUnitScope_Global,
        0,
        &instrumentCount,
        &size
    );

    if (result != noErr) {
        std::cerr << "Error: Could not retrieve instrument count. Code: " << result << std::endl;
        return;
    }

    std::cout << "Number of Instruments: " << instrumentCount << std::endl;

    // Print all instruments
    for (UInt32 i = 0; i < instrumentCount; i++) {
        std::cout << "Instrument ID: " << i << std::endl;
    }
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
     //OSStatus result = AudioComponentInstanceNew(comp, &samplerUnit);
     //if (result != noErr) {
     //    std::cerr << "Error: Could not create Audio Unit.\n";
     //    return -1;
     //}

     //result = AudioUnitInitialize(samplerUnit);
     //if (result != noErr) {
     //    std::cerr << "Error: Could not initialize Audio Unit.\n";
     //    return -1;
     //}

     // Load DLS file
     const char *dlsPath = "/System/Library/Components/CoreAudio.component/Contents/Resources/gs_instruments.dls";
     result = LoadDLSSoundBank(dlsPath);
     if (result != noErr) {
         std::cerr << "Error: Could not load DLS sound bank.\n";
         return -1;
     }

     // List available instruments
     ListInstruments();
};



// ************************************************************************************
// parse sf2 file
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

struct RIFFHeader {
    char chunkID[4];    // "RIFF"
    uint32_t chunkSize; // File size - 8 bytes
    char format[4];     // "sfbk"
};

struct SF2Chunk {
    char id[4];         // Chunk identifier (e.g., "INFO", "sdta", "pdta")
    uint32_t size;      // Chunk size
};

// Helper function to read a string safely
std::string ReadString(std::ifstream &file, size_t length) {
    std::vector<char> buffer(length + 1, '\0');
    file.read(buffer.data(), length);
    return std::string(buffer.data());
}

// Function to parse an SF2 file
void ParseSF2(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file.\n";
        return;
    }

    // Read RIFF Header
    RIFFHeader riff;
    file.read(reinterpret_cast<char*>(&riff), sizeof(RIFFHeader));
    if (strncmp(riff.chunkID, "RIFF", 4) != 0 || strncmp(riff.format, "sfbk", 4) != 0) {
        std::cerr << "Error: Not a valid SF2 file.\n";
        return;
    }

    std::cout << "Valid SF2 file detected.\n";

    // Read Chunks
    while (file.read(reinterpret_cast<char*>(&riff), sizeof(SF2Chunk))) {
        std::string chunkID(riff.chunkID, 4);
        std::cout << "Found Chunk: " << chunkID << " (Size: " << riff.chunkSize << " bytes)\n";

        if (chunkID == "INFO") {
            std::cout << "Reading INFO chunk (Metadata)...\n";
        } else if (chunkID == "sdta") {
            std::cout << "Reading SDTA chunk (Sample Data)...\n";
        } else if (chunkID == "pdta") {
            std::cout << "Reading PDTA chunk (Preset Data)...\n";
        }

        // Skip to the next chunk
        file.seekg(riff.chunkSize, std::ios::cur);
    }

    std::cout << "SF2 Parsing Completed.\n";
}

int parseSF2(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <soundfont.sf2>\n";
        return 1;
    }

    ParseSF2(argv[1]);
    return 0;
}




