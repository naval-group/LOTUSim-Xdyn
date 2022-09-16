#ifndef TEMPFILEHPP
#define TEMPFILEHPP

#include <fstream>
#include <string>

class TempFile
{
    public:
        TempFile();
        ~TempFile();

        template <typename T> TempFile& operator<<(const T& rhs)
        {
            csv << rhs;
            return *this;
        }

        std::string get_filename() const;

        void close();

    private:
        std::string filename;
        std::ofstream csv;
};

#endif