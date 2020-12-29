#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "StateMachine.h"
#include "DataSourceProvider.h"
#include <QVector>
#include <QString>
#include <QStringBuilder>
#include <QTimer>
#include <math.h>

DataSourceProvider * datUltrasoud;
StateMachine * smCurrentState;
QTimer *tmrDataGenerationTimer;

QVector<double> arrXAxis;

#ifdef TIMER_PERFORMANCE_TESTING
QTimer *tmrFrameCounter;
unsigned long long iTotalFrames=0;
unsigned int iCurrentFrames=0;
unsigned int iFps=1;
#endif

void MainWindow::RegenerateXAxisData(){
    arrXAxis.clear();
    for (int i=0; i<datUltrasoud->GetCurrentPointsPerPlot();++i){
        arrXAxis.push_back(double(i)*double(1000)/double(datUltrasoud->GetCurrentSamlingRate()));
    }
}

void MainWindow::UpdateAxisData(){
    ui->chrtData->xAxis->setRange(0,datUltrasoud->GetCurrentDisplayTimespan());
    ui->chrtData->yAxis->setRange(0,50);
}

void MainWindow::tmrDataGenerationTimer_Tick(){
    QVector<double> arrData=datUltrasoud->GeneratePlotForTesting();
    QVector<QCPGraphData> *mData;
    mData = ui->chrtData->graph(0)->data()->coreData();
    mData->clear();
    QCPGraphData gpdDataPoint;
    for (int i=0; i<datUltrasoud->GetCurrentPointsPerPlot(); ++i){
        gpdDataPoint.key=arrXAxis[i];
        gpdDataPoint.value=arrData[i];
        mData->append(gpdDataPoint);
    }
    ui->chrtData->replot(QCustomPlot::rpQueuedReplot);
#ifdef TIMER_PERFORMANCE_TESTING
    ++iCurrentFrames;
    ++iTotalFrames;
    ui->lblCounter->setText(QString::number(iTotalFrames)+QString(" Plot(s)")+QString("\r\n")+QString::number(iFps)+QString(" fps"));
#endif
    //ui->chrtData->layer("main")->replot();
    //qApp->processEvents();
    //QApplication::processEvents();
    return;
}

#ifdef TIMER_PERFORMANCE_TESTING
void MainWindow::tmrFrameCounter_Tick(){
    iFps=iCurrentFrames;
    iCurrentFrames=0;
    ui->lblCounter->setText(QString::number(iTotalFrames)+QString(" Plot(s)")+QString("\r\n")+QString::number(iFps)+QString(" fps"));
    return;
}

void MainWindow::chrtData_afterReplot(){
    //++iCurrentFrames;
    //++iTotalFrames;
    //ui->lblCounter->setText(QString::number(iTotalFrames)+QString(" Plot(s)")+QString("\r\n")+QString::number(iFps)+QString(" fps"));
}
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set fake battery percentage
    ui->prgBattPct->setValue(100);

    //Initialize DataSourceProvider
    datUltrasoud=new DataSourceProvider();
    smCurrentState=new StateMachine();
    ui->lblSamplingRate->setText(datUltrasoud->SamplingRateToString());
    ui->lblGain->setText(datUltrasoud->GainToString());
    ui->lblTimespan->setText(datUltrasoud->DisplayTimespanToString());

    //QCustomPlot style
    ui->chrtData->axisRect()->setupFullAxesBox();
    QFont fntAxisFont(ui->chrtData->xAxis->tickLabelFont());
    fntAxisFont.setStyleStrategy(QFont::NoAntialias);
    ui->chrtData->xAxis->setTickLabelFont(fntAxisFont);
    fntAxisFont=ui->chrtData->yAxis->tickLabelFont();
    fntAxisFont.setStyleStrategy(QFont::NoAntialias);
    ui->chrtData->yAxis->setTickLabelFont(fntAxisFont);
    ui->chrtData->setBackground(QBrush(Qt::black));
    ui->chrtData->xAxis->setBasePen(QPen(Qt::white));
    ui->chrtData->xAxis->setTickPen(QPen(Qt::white));
    ui->chrtData->xAxis->setSubTickPen(QPen(Qt::white));
    ui->chrtData->xAxis->setLabelColor(Qt::white);
    ui->chrtData->xAxis->setTickLabelColor(Qt::white);
    ui->chrtData->yAxis->setBasePen(QPen(Qt::white));
    ui->chrtData->yAxis->setTickPen(QPen(Qt::white));
    ui->chrtData->yAxis->setSubTickPen(QPen(Qt::white));
    ui->chrtData->yAxis->setLabelColor(Qt::white);
    ui->chrtData->yAxis->setTickLabelColor(Qt::white);
    ui->chrtData->xAxis2->setBasePen(QPen(Qt::white));
    ui->chrtData->xAxis2->setTickPen(QPen(Qt::white));
    ui->chrtData->xAxis2->setSubTickPen(QPen(Qt::white));
    ui->chrtData->xAxis2->setLabelColor(Qt::white);
    ui->chrtData->xAxis2->setTickLabelColor(Qt::white);
    ui->chrtData->yAxis2->setBasePen(QPen(Qt::white));
    ui->chrtData->yAxis2->setTickPen(QPen(Qt::white));
    ui->chrtData->yAxis2->setSubTickPen(QPen(Qt::white));
    ui->chrtData->yAxis2->setLabelColor(Qt::white);
    ui->chrtData->yAxis2->setTickLabelColor(Qt::white);
    ui->chrtData->xAxis->setLabel("Time / ms");
    ui->chrtData->yAxis->setLabel("Value");
    ui->chrtData->xAxis->setRange(0,datUltrasoud->GetCurrentDisplayTimespan());
    ui->chrtData->yAxis->setRange(0,50);
    if (0==ui->chrtData->graphCount()){
        ui->chrtData->addGraph();
    }
    ui->chrtData->graph(0)->setAntialiased(false);
    ui->chrtData->graph(0)->setAntialiasedFill(false);
    ui->chrtData->graph(0)->setAntialiasedScatters(false);

    //QCustomPlot performance workarounds
    //ui->chrtData->setOpenGl(true);
    ui->chrtData->setNotAntialiasedElements(QCP::aeAll);
    ui->chrtData->setNoAntialiasingOnDrag(true);
    ui->chrtData->setPlottingHints(QCP::phCacheLabels);
    ui->chrtData->graph(0)->setPen(QPen(QColor(0,150,245),1));
    ui->chrtData->graph(0)->setLineStyle(QCPGraph::lsLine);
    //ui->chrtData->layer("main")->setMode(QCPLayer::lmBuffered);

    //Start Timer
    RegenerateXAxisData();
    tmrDataGenerationTimer = new QTimer(this);
    connect(tmrDataGenerationTimer, SIGNAL(timeout()), this, SLOT(tmrDataGenerationTimer_Tick()));
    tmrDataGenerationTimer->start(datUltrasoud->GetCurrentDisplayTimespan());

#ifdef TIMER_PERFORMANCE_TESTING
    //Frame Counter
    ui->lblCounter->setText(QString::number(iTotalFrames)+QString(" Plot(s)")+QString("\r\n")+QString::number(iFps)+QString(" fps"));
    connect(ui->chrtData, SIGNAL(afterReplot()), this, SLOT(chrtData_afterReplot()));
    tmrFrameCounter=new QTimer(this);
    connect(tmrFrameCounter, SIGNAL(timeout()), this, SLOT(tmrFrameCounter_Tick()));
    tmrFrameCounter->start(1000);
#else
    ui->lblCounter->setVisible(false);
#endif
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
                ui->lblSamplingRate->setText(datUltrasoud->SamplingRateToString());
                RegenerateXAxisData();
                UpdateAxisData();
            }
            break;
        case SM_STATE_GAIN_ADJUSTING:
            if (datUltrasoud->GetCurrentGainInMultiple()<=4){
                datUltrasoud->SetCurrentGainInMultiple(datUltrasoud->GetCurrentGainInMultiple()*2);
                ui->lblGain->setText(datUltrasoud->GainToString());
            }
            break;
        case SM_STATE_TIMESPAN_ADJUSTING:
            if (datUltrasoud->GetCurrentDisplayTimespan()<=__INT_MAX__){
                datUltrasoud->SetCurrentDisplayTimespan(datUltrasoud->GetCurrentDisplayTimespan()*2);
                ui->lblTimespan->setText(datUltrasoud->DisplayTimespanToString());
                RegenerateXAxisData();
                UpdateAxisData();
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
                ui->lblSamplingRate->setText(datUltrasoud->SamplingRateToString());
                RegenerateXAxisData();
                UpdateAxisData();
            }
            break;
        case SM_STATE_GAIN_ADJUSTING:
            if (datUltrasoud->GetCurrentGainInMultiple()>=0.5){
                datUltrasoud->SetCurrentGainInMultiple(datUltrasoud->GetCurrentGainInMultiple()/2);
                ui->lblGain->setText(datUltrasoud->GainToString());
            }
            break;
        case SM_STATE_TIMESPAN_ADJUSTING:
            if (datUltrasoud->GetCurrentDisplayTimespan()>=2){
                datUltrasoud->SetCurrentDisplayTimespan(datUltrasoud->GetCurrentDisplayTimespan()/2);
                ui->lblTimespan->setText(datUltrasoud->DisplayTimespanToString());
                RegenerateXAxisData();
                UpdateAxisData();
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
