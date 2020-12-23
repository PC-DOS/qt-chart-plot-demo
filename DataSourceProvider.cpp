#include "DataSourceProvider.h"
#include <iostream>
#include <QVector>
#include <QString>
#include <math.h>
#include <cstdlib>
#include <ctime>

#define UNIT_G 1000000000
#define UNIT_M 1000000
#define UNIT_K 1000

using namespace std;

DataSourceProvider::DataSourceProvider(){
    cerr<<"DataSourceProvider: Initializing with default values: SamplingRate=16 MHz, Timespan=25 ms, Gain=6.02 dB (x2)."<<endl;
    _iCurrentSamplingRate=16000000;
    _dCurrentGain=2;
    _iCurrentDisplayTimespan=25;
    _iPointsPerPlot=(double(_iCurrentDisplayTimespan)/double(1000))*_iCurrentSamplingRate;
    //TODO: Drivers
}

DataSourceProvider::DataSourceProvider(int iSamplingRateInHz, double dGainInMultiple, int iDisplayTimespanInMillisecond){
    cerr<<"DataSourceProvider: Initializing with default values."<<endl;
    if (iSamplingRateInHz<=0){
        cerr<<"DataSourceProvider: Invalid sampling rate, using default value 16 MHz."<<endl;
        iSamplingRateInHz=16000000;
    }
    if (dGainInMultiple<=0){
        cerr<<"DataSourceProvider: Invalid gain, using default value 6.02 dB (x2)."<<endl;
        dGainInMultiple=2;
    }
    if (iDisplayTimespanInMillisecond<=0){
        cerr<<"DataSourceProvider: Invalid display timespan, using default value 25 ms."<<endl;
        iDisplayTimespanInMillisecond=25;
    }
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
    return double(20)*log10(dValueInMultiple);
}

double DataSourceProvider::DbToMultiple(double dValueInDb){
    return pow(10,dValueInDb/double(20));
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

QString DataSourceProvider::SamplingRateToString(bool IsUnitTranslationEnabled){
    if (IsUnitTranslationEnabled){
        if (_iCurrentSamplingRate>=UNIT_G){
            return QString::number(double(_iCurrentSamplingRate)/double(UNIT_G),'g',5)+QString(" GHz");
        }
        else if (_iCurrentSamplingRate>=UNIT_M){
            return QString::number(double(_iCurrentSamplingRate)/double(UNIT_M),'g',5)+QString(" MHz");
        }
        else if (_iCurrentSamplingRate>=UNIT_K){
            return QString::number(double(_iCurrentSamplingRate)/double(UNIT_K),'g',5)+QString(" kHz");
        }
        else{
            return QString::number(_iCurrentSamplingRate)+QString(" Hz");
        }
    }
    else{
        return QString::number(_iCurrentSamplingRate)+QString(" Hz");
    }
}

QString DataSourceProvider::GainToString(bool IsDbEnabled, bool IsDbOnly){
    if (IsDbOnly){
        return QString::number(DataSourceProvider::MultipleToDb(_dCurrentGain),'g',4)+QString(" dB");
    }
    if (IsDbEnabled){
        return QString("x")+QString::number(_dCurrentGain,'g',2)+QString(" (")+QString::number(DataSourceProvider::MultipleToDb(_dCurrentGain),'g',4)+QString(" dB)");
    }
    else{
        return QString("x")+QString::number(_dCurrentGain,'g',2);
    }
}

QString DataSourceProvider::DisplayTimespanToString(){
    return QString::number(_iCurrentDisplayTimespan)+QString(" ms");
}

const QVector<double> & DataSourceProvider::GeneratePlotForTesting(){
    //srand(time(0));
    _arrData.clear();
    for (int i=1; i<=_iPointsPerPlot;++i){
        _arrData.push_back((double(rand())/double(__INT_MAX__)*_dCurrentGain));
    }
    return _arrData;
}
