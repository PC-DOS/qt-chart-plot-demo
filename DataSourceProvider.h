#ifndef DATASOURCEPROVIDER_H
#define DATASOURCEPROVIDER_H

#include <QVector>
#include <QString>

class DataSourceProvider{
private:
    int _iCurrentSamplingRate;
    double _dCurrentGain; //In multiples, to generate value in dB, use: dB = 20lg(Multiples)
    int _iCurrentDisplayTimespan;
    int _iPointsPerPlot;

    QVector<double> _arrData;

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

    QString SamplingRateToString(bool IsUnitTranslationEnabled=true);
    QString GainToString(bool IsDbEnabled=true, bool IsDbOnly=false);
    QString DisplayTimespanToString();

    const QVector<double> &GeneratePlotForTesting();
};

#endif // DATASOURCEPROVIDER_H
