#ifndef CGLOABALPARAM_H
#define CGLOABALPARAM_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QMutex>
#include <QFile>
#include <QDir>
#include <QDebug>

class CGloabalParam
{
public:
    CGloabalParam();
    static void saveParam(QString path);
    static void loadParam(QString path);
    static void saveSysSetting();
    static void loadSysSetting();

public:
    //System Setting parameters.
    static int COCURRENT;
    static int FREQ_ACCURACY;
    static int SAVE_DATA_PTS;
    static int SYSTEM_LANG;
    static int ZERO_THRESHOLD;

    //Use strings to store the data preparing for the parametric sweep implementation.
    //Dielectric parameters.
    static int DIELEC_PARAM_TYPE;
    static QString DIELEC_PARAM_1;
    static QString DIELEC_PARAM_2;
    static QString DIELEC_PARAM_3;

    //Geometry Parameters.
    static QString GEOMETRY_WAVEGUIDE_D;
    static QString GEOMETRY_GRATING_P;

    //Frequency Parameters.
    static int FREQ_PARAM_TYPE;
    static QString FREQ_PARAM_VALUE;

    //Global Progress
    static int GLOBAL_PROG_VALUE;

    //Global Parameter Mutex
    static QMutex GLOBAL_PARAM_MUTEX;

};

#endif // CGLOABALPARAM_H
