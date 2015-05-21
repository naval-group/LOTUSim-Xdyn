
#define MODEL_IDENTIFIER Simulator
#include "fmiModelFunctions.h"
#include "fmiModelTypes.h"

#include <stdio.h>
#include <stdlib.h>


void logger(fmiComponent c, fmiString instanceName, fmiStatus status, fmiString category, fmiString message, ...);
void logger(fmiComponent c, fmiString instanceName, fmiStatus status, fmiString category, fmiString message, ...)
{
    (void)c;
    (void)instanceName;
    (void)status;
    (void)category;
    printf("%s\n", message);
}

void* allocate(size_t nobj, size_t size);
void* allocate(size_t nobj, size_t size)
{
    return malloc(nobj*size);
}

fmiComponent get_model();
fmiComponent get_model()
{
    fmiCallbackFunctions functions;
    functions.logger = logger;
    functions.allocateMemory = allocate;
    functions.freeMemory = free;
    return fmiInstantiateModel("blah", "", functions, fmiTrue);
}

void simulate(fmiComponent c, const double dt, const size_t n);
void simulate(fmiComponent c, const double dt, const size_t n)
{
    fmiEventInfo eventInfo;
    double x[13];
    size_t i, j;
    double t;
    double der_x[13];
    fmiBoolean callEventUpdate = fmiFalse;
    fmiSetTime(c, 0.);
    fmiInitialize(c, fmiFalse, 0.0, &eventInfo);
    printf("t\tx\ty\tz\tu\tv\tw\tp\tq\tr\n");
    fmiGetContinuousStates(c, x, 13);
    for (i = 0 ; i < n ; ++i)
    {
        t = (double)i*dt;
        fmiGetDerivatives(c, der_x, 13);
        for (j = 0 ; j < 13 ; ++j) x[j] = x[j] + dt*der_x[j];
        fmiSetTime(c, t);
        printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",t,x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8]);
        fmiSetContinuousStates(c, x, 13);
        fmiCompletedIntegratorStep(c, &callEventUpdate);
    }
    fmiTerminate(c);
}

int main()
{
    fmiComponent c = get_model();
    simulate(c, 0.1, 10);
    fmiFreeModelInstance(c);    
    return 0;
}
