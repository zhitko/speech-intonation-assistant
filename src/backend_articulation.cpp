#include "backend.h"

#include <QDebug>
#include <QGuiApplication>

#include <inton-core/include/intoncore.h>
#include <inton-core/include/utils.h>

#include "applicationconfig.h"

QVariant Backend::getSpeechRate(QString path, bool isTemplate)
{
    qDebug() << "getSpeechRate: isTemplate " << isTemplate;
    IntonCore::Storage* storage = nullptr;
    if (isTemplate) {
        qDebug() << "getSpeechRate: template";
        this->initializeTemplateCore(path);
        storage = this->core->getTemplate();
    } else {
        qDebug() << "getSpeechRate: record";
        this->initializeRecordCore(path);
        storage = this->core->getRecord();
    }

    auto k = this->getSettingValue(ApplicationConfig::SETTINGS_ARTICULATION_SPEECH_K1).toDouble();
    auto nv = storage->getVowelsCount();
    auto ts = this->getWaveLength(path, isTemplate).toDouble();
    double speechRate = k * nv * 60 / ts;

    qDebug() << "getSpeechRate:" << speechRate;
    qDebug() << "speechRate: " << k;
    qDebug() << "speechRate: " << nv;
    qDebug() << "speechRate: " << ts;
    qDebug() << "speechRate: " << speechRate;

    return QVariant::fromValue(speechRate);
}

QVariant Backend::getConsonantsAndSilenceMedianValue(QString path, bool isTemplate)
{
    qDebug() << "getConsonantsAndSilenceMedianValue: isTemplate " << isTemplate;
    IntonCore::Storage* storage = nullptr;
    if (isTemplate) {
        qDebug() << "getConsonantsAndSilenceMedianValue: template";
        this->initializeTemplateCore(path);
        storage = this->core->getTemplate();
    } else {
        qDebug() << "getConsonantsAndSilenceMedianValue: record";
        this->initializeRecordCore(path);
        storage = this->core->getRecord();
    }

    auto segments = storage->getAutoSegmentsByIntensitySmoothedInverted();

    auto segments_mask = storage->getAutoSegmentsByIntensitySmoothedMask();

    std::vector<int> selected_segments;

    for (auto &it: segments)
    {
        selected_segments.push_back(it.second);
    }

    sort(selected_segments.begin(), selected_segments.end());

    uint32_t median = IntonCore::getMedianValue(selected_segments);

    qDebug() << "getConsonantsAndSilenceMedianValue median: " << median;

    return QVariant::fromValue( storage->convertIntensityPointsToSec(median));
}

QVariant Backend::getArticulationRate(QString path, bool isTemplate)
{
    qDebug() << "getArticulationRate: isTemplate " << isTemplate;
    IntonCore::Storage* storage = nullptr;
    if (isTemplate) {
        qDebug() << "getArticulationRate: template";
        this->initializeTemplateCore(path);
        storage = this->core->getTemplate();
    } else {
        qDebug() << "getArticulationRate: record";
        this->initializeRecordCore(path);
        storage = this->core->getRecord();
    }

    auto k2 = this->getSettingValue(ApplicationConfig::SETTINGS_ARTICULATION_K2).toDouble();
    qDebug() << "getArticulationRate K2: " << k2;

    auto rs = this->getSpeechRate(path, isTemplate).toDouble();
    auto ts = this->getWaveLength(path, isTemplate).toDouble();
    auto tv = storage->convertIntensityPointsToSec(storage->getVowelsLength());
    auto tcpm = this->getConsonantsAndSilenceMedianValue(path, isTemplate).toDouble();
    auto nc = storage->getConsonantsAndSilenceCount();
    double articulationRate = rs * ts / (tv + k2 * tcpm * nc);

    qDebug() << "getArticulationRate: " << articulationRate;

    if (rs > articulationRate) return rs;

    return QVariant::fromValue(articulationRate);
}

QVariant Backend::getMeanDurationOfPauses(QString path, bool isTemplate)
{
    qDebug() << "getMeanDurationOfPauses: isTemplate " << isTemplate;
    IntonCore::Storage* storage = nullptr;
    if (isTemplate) {
        qDebug() << "getMeanDurationOfPauses: template";
        this->initializeTemplateCore(path);
        storage = this->core->getTemplate();
    } else {
        qDebug() << "getMeanDurationOfPauses: record";
        this->initializeRecordCore(path);
        storage = this->core->getRecord();
    }

    auto k3 = this->getSettingValue(ApplicationConfig::SETTINGS_ARTICULATION_PAUSES_K3).toDouble();
    qDebug() << "getMeanDurationOfPauses K3:" << k3;
    auto mvd = this->getSettingValue(ApplicationConfig::SETTINGS_ARTICULATION_PAUSES_MEAN_DEGRY).toDouble();

    auto tcm = storage->convertIntensityPointsToSec(storage->getConsonantsAndSilenceLengthGeneralizedMean(mvd));
    auto tcd = this->getConsonantsAndSilenceMedianValue(path, isTemplate).toDouble();
    if (tcm < tcd) return QVariant::fromValue(0.0);

    double meanDurationOfPauses = k3 * (tcm - tcd) / tcd;

    qDebug() << "getMeanDurationOfPauses:" << meanDurationOfPauses;

    return QVariant::fromValue(meanDurationOfPauses);
}
