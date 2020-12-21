#ifndef DATASOURCEPROVIDER_H
#define DATASOURCEPROVIDER_H

#include "StateMachineStates.h"
#include <vector>

class DataSourceProvider{
private:
    int _iCurrentState;
    int _iCurrentSamplingRate;
    double _dCurrentGain; //In multiples, to generate value in dB, use: dB = 20lg(Multiples)
    int _iCurrentDisplayTimespan;
    int _iPointsPerPlot;

public:
    DataSourceProvider();
    DataSourceProvider(int iSamplingRateInHz, double dGainInMultiple, int iDisplayTimespanInMillisecond);
    ~DataSourceProvider();

    double MultipleToDb(double dValueInMultiple);
    double DbToMultiple(double dValueInDb);

    void SetCurrentSamplingRate(int iSamplingRateInHz);
    int GetCurrentSamlingRate();
    void SetCurrentGainInMultiple(double dGainInMultiple);
    double GetCurrentGainInMultiple();
    void SetCurrentGainInDb(double dGainInDb);
    double GetCurrentGainInDb();
    void SetCurrentDisplayTimespan(int iDisplayTimespanInMillisecond);
    int GetCurrentDisplayTimespan();
    int GetCurrentPointsPerPlot();

    std::vector<double> GeneratePlotForTesting();
};

#endif // DATASOURCEPROVIDER_H
