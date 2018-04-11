#include "comwidget.hpp"
#include "os.hpp"
#include "log.hpp"

ComWidget::ComWidget()
{
    addSettings();
    fillCombos();
    connectSignalToSlot();
}
ComWidget::~ComWidget()
{
}

void ComWidget::onPushOpenClicked()
{
    int ret;
    string com1stName = comboComName1st->currentText().toStdString();
    string com2ndName = comboComName2nd->currentText().toStdString();
    if (com1stName == com2ndName)
    {
        msgBox.setText("ERROR: two ports can not be the same.");
        msgBox.exec();
        setEnabledAtClose();
        return;
    }

    int baudRate = comboBaudRate->itemData(comboBaudRate->currentIndex()).toInt();
    int dataBit = comboDataBit->itemData(comboDataBit->currentIndex()).toInt();
    int stopBit = comboStopBit->itemData(comboStopBit->currentIndex()).toInt();
    int parity = comboParity->itemData(comboParity->currentIndex()).toInt();
    
    ret = initComms(com1stName, com2ndName, baudRate, dataBit, stopBit, parity);
    if (COMM_SUC != ret)
    {
        msgBox.setText("ERROR: call initComs failed");
        msgBox.exec();
        clearComms();
        setEnabledAtClose();
        return;
    }
    setTextLineSend1st(OS::genVisibleString(8));
    setTextLineSend2nd(OS::genVisibleString(8));
}

void ComWidget::addSettings()
{
    hLayoutUp = new QHBoxLayout;
    labelComName1st = new QLabel(tr(" 1st COM:"));
    labelComName2nd = new QLabel(tr(" 2nd COM:"));
    comboComName1st = new QComboBox;
    comboComName2nd = new QComboBox;
    hLayoutUp->addWidget(labelComName1st);
    hLayoutUp->addWidget(comboComName1st);
    hLayoutUp->addWidget(labelComName2nd);
    hLayoutUp->addWidget(comboComName2nd);
    hLayoutUp->addWidget(comboComName2nd);
    hLayoutUp->insertStretch(-1);
    
    hLayoutDown = new QHBoxLayout;
    labelBaudRate = new QLabel(tr(" Boud Rate:"));
    labelDataBit = new QLabel(tr(" Data Bit:"));
    labelStopBit = new QLabel(tr(" Stop Bit:"));
    labelParity = new QLabel(tr(" Parity:"));
    comboBaudRate = new QComboBox;
    comboDataBit = new QComboBox;
    comboStopBit = new QComboBox;
    comboParity = new QComboBox;
    hLayoutDown->addWidget(labelBaudRate);
    hLayoutDown->addWidget(comboBaudRate);
    hLayoutDown->addWidget(labelDataBit);
    hLayoutDown->addWidget(comboDataBit);
    hLayoutDown->addWidget(labelStopBit);
    hLayoutDown->addWidget(comboStopBit);
    hLayoutDown->addWidget(labelParity);
    hLayoutDown->addWidget(comboParity);
    
    vLayoutMain = new QVBoxLayout;
    vLayoutMain->addLayout(hLayoutUp);
    vLayoutMain->addLayout(hLayoutDown);

    groupSetting->setLayout(vLayoutMain);
}

void ComWidget::fillCombos()
{
    map<string,int>::const_iterator iter;
    
    const map<string,int>& mapPort = DEF_COM::enumSettingPort();
    for(iter=mapPort.begin();iter!=mapPort.end();++iter)
    {
        comboComName1st->addItem(QString::fromStdString(iter->first), iter->second);
        comboComName2nd->addItem(QString::fromStdString(iter->first), iter->second);
    }

    const map<string,int>& mapBaudRate = DEF_COM::enumSettingBaudRate();
    for(iter=mapBaudRate.begin();iter!=mapBaudRate.end();++iter)
    {
        comboBaudRate->addItem(QString::fromStdString(iter->first), iter->second);
    }

    const map<string,int>& mapDataBit = DEF_COM::enumSettingDataBit();
    for(iter=mapDataBit.begin();iter!=mapDataBit.end();++iter)
    {
        comboDataBit->addItem(QString::fromStdString(iter->first), iter->second);
    }

    const map<string,int>& mapStopBit = DEF_COM::enumSettingStopBit();
    for(iter=mapStopBit.begin();iter!=mapStopBit.end();++iter)
    {
        comboStopBit->addItem(QString::fromStdString(iter->first), iter->second);
    }

    const map<string,int>& mapParity = DEF_COM::enumSettingParity();
    for(iter=mapParity.begin();iter!=mapParity.end();++iter)
    {
        comboParity->addItem(QString::fromStdString(iter->first), iter->second);
    }
}

void ComWidget::connectSignalToSlot()
{
    connect(this,SIGNAL(pushOpenClicked()),this,SLOT(onPushOpenClicked()));
}

int ComWidget::initComms(const string& com1stName, const string& com2ndName,
                         int baudRate, int dataBit, int stopBit, int parity)
{
    
    comm1st = new DEF_COM(com1stName,baudRate,dataBit,stopBit,parity);
    comm2nd = new DEF_COM(com2ndName,baudRate,dataBit,stopBit,parity);
    int ret;
    ret = comm1st->open();
    if (COMM_SUC != ret) return ret;
    ret = comm2nd->open();
    if (COMM_SUC != ret) return ret;
    return COMM_SUC;
}
