#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "StateMachine.h"
#include "DataSourceProvider.h"
#include <QVector>
#include <QString>
#include <QStringBuilder>
#include <math.h>

DataSourceProvider * datUltrasoud;
StateMachine * smCurrentState;
QTimer *tmrDataGenerationTimer;

void MainWindow::tmrDataGenerationTimer_Time(){
    QVector<double> arrSampleData;
    arrSampleData=datUltrasoud->GeneratePlotForTesting();
    QVector<double> arrXAxis;
    for (int i=0; i<datUltrasoud->GetCurrentPointsPerPlot();++i){
        arrXAxis.push_back(double(i)*double(1000)/double(datUltrasoud->GetCurrentSamlingRate()));
    }
    if (0==ui->chrtData->graphCount()){
        ui->chrtData->addGraph();
    }
    ui->chrtData->graph(0)->setData(arrXAxis,arrSampleData);
    ui->chrtData->xAxis->setLabel("Time");
    ui->chrtData->yAxis->setLabel("Value");
    ui->chrtData->xAxis->setRange(0,datUltrasoud->GetCurrentDisplayTimespan());
    ui->chrtData->yAxis->setRange(0,50);
    ui->chrtData->replot();
    return;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->prgBattPct->setValue(100);
    datUltrasoud=new DataSourceProvider(100,2,2000);
    smCurrentState=new StateMachine();
    ui->lblSamplingRate->setText(QString::number(datUltrasoud->GetCurrentSamlingRate()) + QString(" Hz"));
    ui->lblGain->setText(QString("x") + QString::number(datUltrasoud->GetCurrentGainInMultiple(),'g',2));
    ui->lblTimespan->setText(QString::number(datUltrasoud->GetCurrentDisplayTimespan()) + QString(" ms"));
    tmrDataGenerationTimer = new QTimer(this);
    connect(tmrDataGenerationTimer, SIGNAL(timeout()), this, SLOT(tmrDataGenerationTimer_Time()));
    tmrDataGenerationTimer->start(1000);
    ui->chrtData->setBackground(QBrush(Qt::black));
    ui->chrtData->xAxis->setBasePen(QPen(Qt::white));
    ui->chrtData->xAxis->setLabelColor(Qt::white);
    ui->chrtData->xAxis->setTickLabelColor(Qt::white);
    ui->chrtData->yAxis->setBasePen(QPen(Qt::white));
    ui->chrtData->yAxis->setLabelColor(Qt::white);
    ui->chrtData->yAxis->setTickLabelColor(Qt::white);
    ui->chrtData->xAxis->setLabel("Time");
    ui->chrtData->yAxis->setLabel("Value");
    ui->chrtData->xAxis->setRange(0,datUltrasoud->GetCurrentDisplayTimespan());
    ui->chrtData->yAxis->setRange(0,50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *KeyEventArgs){
    QWidget::keyPressEvent(KeyEventArgs);
}

void MainWindow::keyReleaseEvent(QKeyEvent *KeyEventArgs){
    QPalette pltSamplingRate(ui->wgtSamplingRate->palette());
    QPalette pltGain(ui->wgtGain->palette());
    QPalette pltTimespan(ui->wgtTimespan->palette());
    switch (KeyEventArgs->key()){
    case Qt::Key_1: //Finish
        smCurrentState->SetCurrentState(SM_STATE_NOTMAL);
        pltSamplingRate.setColor(QPalette::Background,QColor(75,75,75));
        pltGain.setColor(QPalette::Background,QColor(75,75,75));
        pltTimespan.setColor(QPalette::Background,QColor(75,75,75));
        ui->wgtSamplingRate->setPalette(pltSamplingRate);
        ui->wgtGain->setPalette(pltGain);
        ui->wgtTimespan->setPalette(pltTimespan);
        break;
    case Qt::Key_2: //Value plus
        switch (smCurrentState->GetCurrentState()){
        case SM_STATE_NOTMAL:
            break;
        case SM_STATE_SAMPLINGRATE_ADJUSTING:
            if (datUltrasoud->GetCurrentSamlingRate()<=__INT_MAX__){
                datUltrasoud->SetCurrentSamplingRate(datUltrasoud->GetCurrentSamlingRate()*2);
                ui->lblSamplingRate->setText(QString::number(datUltrasoud->GetCurrentSamlingRate()) + QString(" Hz"));
            }
            break;
        case SM_STATE_GAIN_ADJUSTING:
            if (datUltrasoud->GetCurrentGainInMultiple()<=4){
                datUltrasoud->SetCurrentGainInMultiple(datUltrasoud->GetCurrentGainInMultiple()*2);
                ui->lblGain->setText(QString("x") + QString::number(datUltrasoud->GetCurrentGainInMultiple(),'g',2));
            }
            break;
        case SM_STATE_TIMESPAN_ADJUSTING:
            if (datUltrasoud->GetCurrentDisplayTimespan()<=__INT_MAX__){
                datUltrasoud->SetCurrentDisplayTimespan(datUltrasoud->GetCurrentDisplayTimespan()*2);
                ui->lblTimespan->setText(QString::number(datUltrasoud->GetCurrentDisplayTimespan()) + QString(" ms"));
                tmrDataGenerationTimer->setInterval(datUltrasoud->GetCurrentDisplayTimespan());
            }
            break;
        default:
            break;
        }

        break;
    case Qt::Key_3: //Value minus
        switch (smCurrentState->GetCurrentState()){
        case SM_STATE_NOTMAL:
            break;
        case SM_STATE_SAMPLINGRATE_ADJUSTING:
            if (datUltrasoud->GetCurrentSamlingRate()>=50){
                datUltrasoud->SetCurrentSamplingRate(datUltrasoud->GetCurrentSamlingRate()/2);
                ui->lblSamplingRate->setText(QString::number(datUltrasoud->GetCurrentSamlingRate()) + QString(" Hz"));
            }
            break;
        case SM_STATE_GAIN_ADJUSTING:
            if (datUltrasoud->GetCurrentGainInMultiple()>=0.5){
                datUltrasoud->SetCurrentGainInMultiple(datUltrasoud->GetCurrentGainInMultiple()/2);
                ui->lblGain->setText(QString("x") + QString::number(datUltrasoud->GetCurrentGainInMultiple(),'g',2));
            }
            break;
        case SM_STATE_TIMESPAN_ADJUSTING:
            if (datUltrasoud->GetCurrentDisplayTimespan()>=2){
                datUltrasoud->SetCurrentDisplayTimespan(datUltrasoud->GetCurrentDisplayTimespan()/2);
                ui->lblTimespan->setText(QString::number(datUltrasoud->GetCurrentDisplayTimespan()) + QString(" ms"));
                tmrDataGenerationTimer->setInterval(datUltrasoud->GetCurrentDisplayTimespan());
            }
            break;
        default:
            break;
        }

        break;
    case Qt::Key_4: //Set Timespan
        smCurrentState->SetCurrentState(SM_STATE_TIMESPAN_ADJUSTING);
        pltSamplingRate.setColor(QPalette::Background,QColor(75,75,75));
        pltGain.setColor(QPalette::Background,QColor(75,75,75));
        pltTimespan.setColor(QPalette::Background,QColor(150,150,150));
        ui->wgtSamplingRate->setPalette(pltSamplingRate);
        ui->wgtGain->setPalette(pltGain);
        ui->wgtTimespan->setPalette(pltTimespan);
        break;
    case Qt::Key_8: //Set Gain
        smCurrentState->SetCurrentState(SM_STATE_GAIN_ADJUSTING);
        pltSamplingRate.setColor(QPalette::Background,QColor(75,75,75));
        pltGain.setColor(QPalette::Background,QColor(150,150,150));
        pltTimespan.setColor(QPalette::Background,QColor(75,75,75));
        ui->wgtSamplingRate->setPalette(pltSamplingRate);
        ui->wgtGain->setPalette(pltGain);
        ui->wgtTimespan->setPalette(pltTimespan);
        break;
    case Qt::Key_B: //Set Sampling Rate
        smCurrentState->SetCurrentState(SM_STATE_SAMPLINGRATE_ADJUSTING);
        pltSamplingRate.setColor(QPalette::Background,QColor(150,150,150));
        pltGain.setColor(QPalette::Background,QColor(75,75,75));
        pltTimespan.setColor(QPalette::Background,QColor(75,75,75));
        ui->wgtSamplingRate->setPalette(pltSamplingRate);
        ui->wgtGain->setPalette(pltGain);
        ui->wgtTimespan->setPalette(pltTimespan);
        break;
    case Qt::Key_F:

        break;
    default:
        break;
    }
    QWidget::keyReleaseEvent(KeyEventArgs);
}
