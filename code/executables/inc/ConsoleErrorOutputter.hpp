#ifndef CONSOLEERROROUTPUTTERHPP
#define CONSOLEERROROUTPUTTERHPP

#include "ErrorOutputter.hpp"

class ConsoleErrorOutputter : public ErrorOutputter
{
    private:
        void output() const;

};

#endif // CONSOLEERROROUTPUTTERHPP