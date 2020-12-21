#include "DataSourceProvider.h"
#include "StateMachineStates.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <ctime>

using namespace std;

DataSourceProvider::DataSourceProvider(){
    cerr<<"DataSourceProvider: Initializing with default values: SamplingRate=100Hz, Timespan=1000ms, Gain=6.02dB (x2)."<<endl;
    _iCurrentState=SM_STATE_NOTMAL;
    _iCurrentSamplingRate=100;
    _dCurrentGain=2;
    _iCurrentDisplayTimespan=1000;
    _iPointsPerPlot=(double(_iCurrentDisplayTimespan)/double(1000))*_iCurrentSamplingRate;
    //TODO: Drivers
}

DataSourceProvider::DataSourceProvider(int iSamplingRateInHz, double dGainInMultiple, int iDisplayTimespanInMillisecond){
    cerr<<"DataSourceProvider: Initializing with default values."<<endl;
    if (iSamplingRateInHz<=0){
        cerr<<"DataSourceProvider: Invalid sampling rate, using default value 100 Hz."<<endl;
        iSamplingRateInHz=100;
    }
    if (dGainInMultiple<=0){
        cerr<<"DataSourceProvider: Invalid gain, using default value 6.02 dB (x2)."<<endl;
        dGainInMultiple=2;
    }
    if (iDisplayTimespanInMillisecond<=0){
        cerr<<"DataSourceProvider: Invalid display timespan, using default value 1000 ms."<<endl;
        iDisplayTimespanInMillisecond=1000;
    }
    _iCurrentState=SM_STATE_NOTMAL;
    _iCurrentSamplingRate=iSamplingRateInHz;
    _dCurrentGain=dGainInMultiple;
    _iCurrentDisplayTimespan=iDisplayTimespanInMillisecond;
    _iPointsPerPlot=(double(_iCurrentDisplayTimespan)/double(1000))*_iCurrentSamplingRate;
    //TODO: Drivers
}

DataSourceProvider::~DataSourceProvider(){
    //TODO: Drivers
    cerr<<"DataSourceProvider: Exiting..."<<endl;
}

double DataSourceProvider::MultipleToDb(double dValueInMultiple){
    if (dValueInMultiple<=0){
        return 0;
    }
    return double(20)*log(dValueInMultiple);
}

double DataSourceProvider::DbToMultiple(double dValueInDb){
    return exp(dValueInDb/double(20));
}

void DataSourceProvider::SetCurrentSamplingRate(int iSamplingRateInHz){
    if (iSamplingRateInHz<=0){
        cerr<<"DataSourceProvider: Invalid sampling rate, using default value 100 Hz."<<endl;
        iSamplingRateInHz=100;
    }
    _iCurrentSamplingRate=iSamplingRateInHz;
    _iPointsPerPlot=(double(_iCurrentDisplayTimespan)/double(1000))*_iCurrentSamplingRate;
}

int DataSourceProvider::GetCurrentSamlingRate(){
    return _iCurrentSamplingRate;
}

void DataSourceProvider::SetCurrentGainInMultiple(double dGainInMultiple){
    if (dGainInMultiple<=0){
        cerr<<"DataSourceProvider: Invalid gain, using default value 6.02 dB (x2)."<<endl;
        dGainInMultiple=2;
    }
    _dCurrentGain=dGainInMultiple;
}

double DataSourceProvider::GetCurrentGainInMultiple(){
    return _dCurrentGain;
}

void DataSourceProvider::SetCurrentGainInDb(double dGainInDb){
    _dCurrentGain=DbToMultiple(dGainInDb);
}

double DataSourceProvider::GetCurrentGainInDb(){
    return MultipleToDb(_dCurrentGain);
}

void DataSourceProvider::SetCurrentDisplayTimespan(int iDisplayTimespanInMillisecond){
    if (iDisplayTimespanInMillisecond<=0){
        cerr<<"DataSourceProvider: Invalid display timespan, using default value 1000 ms."<<endl;
        iDisplayTimespanInMillisecond=1000;
    }
    _iCurrentDisplayTimespan=iDisplayTimespanInMillisecond;
    _iPointsPerPlot=(double(_iCurrentDisplayTimespan)/double(1000))*_iCurrentSamplingRate;
}

int DataSourceProvider::GetCurrentDisplayTimespan(){
    return _iCurrentDisplayTimespan;
}

int DataSourceProvider::GetCurrentPointsPerPlot(){
    return _iPointsPerPlot;
}

vector<double> DataSourceProvider::GeneratePlotForTesting(){
    vector<double> arrData;
    srand(time(0));
    for (int i=1; i<=_iPointsPerPlot;++i){
        arrData.push_back((double(rand())/double(__INT_MAX__)*_dCurrentGain));
    }
    return arrData;
}
