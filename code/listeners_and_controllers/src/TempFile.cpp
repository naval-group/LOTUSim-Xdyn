#include <cstdlib> // For mkstemp
#include "TempFile.hpp"

/**
 * @brief Generate a temporary file and return its name
 * 
 * @return Name of the generated temporary file.
 */
std::string get_temp_filename();
std::string get_temp_filename()
{
    char filename[] = "xdyn-temp-file-XXXXXX";
    // When cross-compiling for Windows, the temp file seems to get destroyed before
    // it can be used: as I don't have the time to see why, I only "properly" generate a temp
    // file for Linux gcc. Please note that this only impacts the test code: xdyn itself does
    // not use this TempFile class.
    #if !defined(__MINGW32__)
    // Under gcc linux, we replace the Xs with random characters so the generated file is indeed unique.
    mkstemp(filename);
    #endif
    return std::string(filename);
}


TempFile::TempFile() : filename(get_temp_filename()), csv(std::ofstream(filename, std::ios::binary))
{
}

TempFile::~TempFile()
{
    remove(filename.c_str());
}

std::string TempFile::get_filename() const
{
    return filename;
}

void TempFile::close()
{
    csv.close();
}
