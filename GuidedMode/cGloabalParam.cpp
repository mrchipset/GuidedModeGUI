#include "cGloabalParam.h"


//System Setting parameters.
int CGloabalParam::COCURRENT;
int CGloabalParam::FREQ_ACCURACY;
int CGloabalParam::SAVE_DATA_PTS;
int CGloabalParam::SYSTEM_LANG;
int CGloabalParam::ZERO_THRESHOLD;

//Use strings to store the data preparing for the parametric sweep implementation.
//Dielectric parameters.
int CGloabalParam::DIELEC_PARAM_TYPE;
QString CGloabalParam::DIELEC_PARAM_1;
QString CGloabalParam::DIELEC_PARAM_2;
QString CGloabalParam::DIELEC_PARAM_3;

//Geometry Parameters.
QString CGloabalParam::GEOMETRY_WAVEGUIDE_D;
QString CGloabalParam::GEOMETRY_GRATING_P;

//Frequency Parameters.
int CGloabalParam::FREQ_PARAM_TYPE;
QString CGloabalParam::FREQ_PARAM_VALUE;

//Global Progress
int CGloabalParam::GLOBAL_PROG_VALUE;

//Global Parameter Mutex
QMutex CGloabalParam::GLOBAL_PARAM_MUTEX;


CGloabalParam::CGloabalParam()
{

}

void CGloabalParam::saveParam(QString path)
{
    QFile saveData(path);
    try {
        saveData.open(QIODevice::WriteOnly|QIODevice::Truncate);
        QJsonObject jsonData;
        GLOBAL_PARAM_MUTEX.lock();
        jsonData["DIELEC_PARAM_TYPE"] = DIELEC_PARAM_TYPE;
        jsonData["DIELEC_PARAM_1"] = DIELEC_PARAM_1;
        jsonData["DIELEC_PARAM_2"] = DIELEC_PARAM_2;
        jsonData["DIELEC_PARAM_3"] = DIELEC_PARAM_3;
        jsonData["GEOMETRY_GRATING_P"] = GEOMETRY_GRATING_P;
        jsonData["GEOMETRY_WAVEGUIDE_D"] = GEOMETRY_WAVEGUIDE_D;
        jsonData["FREQ_PARAM_VALUE"] = FREQ_PARAM_VALUE;
        jsonData["FREQ_PARAM_TYPE"] = FREQ_PARAM_TYPE;
        GLOBAL_PARAM_MUTEX.unlock();
        saveData.write(QJsonDocument(jsonData).toBinaryData());
    } catch (...) {
        qDebug()<<"Save System Setting error occured.";
    }
    if(saveData.isOpen())
        saveData.close();
}

void CGloabalParam::loadParam(QString path)
{
    QFile loadData(path);
    try {
        loadData.open(QIODevice::ReadOnly);
        QJsonObject jsonData(QJsonDocument::fromBinaryData(loadData.readAll()).object());
        GLOBAL_PARAM_MUTEX.lock();
        DIELEC_PARAM_TYPE = jsonData["DIELEC_PARAM_TYPE"].toInt();
        DIELEC_PARAM_1 = jsonData["DIELEC_PARAM_1"].toString();
        DIELEC_PARAM_2 = jsonData["DIELEC_PARAM_2"].toString();
        DIELEC_PARAM_3 = jsonData["DIELEC_PARAM_3"].toString();
        GEOMETRY_GRATING_P = jsonData["GEOMETRY_GRATING_P"].toString();
        GEOMETRY_WAVEGUIDE_D = jsonData["GEOMETRY_WAVEGUIDE_D"].toString();
        FREQ_PARAM_VALUE = jsonData["FREQ_PARAM_VALUE"].toString();
        FREQ_PARAM_TYPE = jsonData["FREQ_PARAM_TYPE"].toInt();
        GLOBAL_PARAM_MUTEX.unlock();
    } catch (...) {
        qDebug()<<"Load System Setting error occured.";
    }
    if(loadData.isOpen())
        loadData.close();
}

void CGloabalParam::saveSysSetting()
{
    QFile saveData(QDir::currentPath()+"/config.dat");
    try {
        saveData.open(QIODevice::WriteOnly|QIODevice::Truncate);
        QJsonObject jsonData;
        GLOBAL_PARAM_MUTEX.lock();
        jsonData["COCURRENT"] = COCURRENT;
        jsonData["FREQ_ACCURACY"] = FREQ_ACCURACY;
        jsonData["SAVE_DATA_PTS"] = SAVE_DATA_PTS;
        jsonData["SYSTEM_LANG"] = SYSTEM_LANG;
        jsonData["ZERO_THRESHOLD"] = ZERO_THRESHOLD;
        GLOBAL_PARAM_MUTEX.unlock();
        saveData.write(QJsonDocument(jsonData).toBinaryData());
    } catch (...) {
        qDebug()<<"Save System Setting error occured.";
    }
    if(saveData.isOpen())
        saveData.close();
}

void CGloabalParam::loadSysSetting()
{
    QFile loadData(QDir::currentPath()+"/config.dat");
    try {
        loadData.open(QIODevice::ReadOnly);
        QJsonObject jsonData(QJsonDocument::fromBinaryData(loadData.readAll()).object());
        GLOBAL_PARAM_MUTEX.lock();
        COCURRENT = jsonData["COCURRENT"].toInt();
        FREQ_ACCURACY = jsonData["FREQ_ACCURACY"].toInt();
        SAVE_DATA_PTS = jsonData["SAVE_DATA_PTS"].toInt();
        SYSTEM_LANG = jsonData["SYSTEM_LANG"].toInt();
        ZERO_THRESHOLD = jsonData["ZERO_THRESHOLD"].toInt();
        GLOBAL_PARAM_MUTEX.unlock();
    } catch (...) {
        qDebug()<<"Load System Setting error occured.";
    }
    if(loadData.isOpen())
        loadData.close();
}
