/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   wscript_build;
    const int            wscript_buildSize = 1615;

    extern const char*   Makefile_am;
    const int            Makefile_amSize = 2804;

    extern const char*   SoundTouch_sln;
    const int            SoundTouch_slnSize = 1393;

    extern const char*   SoundTouch_vcxproj;
    const int            SoundTouch_vcxprojSize = 20000;

    extern const char*   soundtouch_config_h_in;
    const int            soundtouch_config_h_inSize = 131;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 5;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
