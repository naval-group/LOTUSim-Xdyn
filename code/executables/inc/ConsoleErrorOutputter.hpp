#ifndef CONSOLEERROROUTPUTTERHPP
#define CONSOLEERROROUTPUTTERHPP

#include "ErrorOutputter.hpp"

class ConsoleErrorOutputter : public ErrorOutputter
{
    public:
        ConsoleErrorOutputter() = default;
    private:
        void output() const;

};

#endif // CONSOLEERROROUTPUTTERHPP