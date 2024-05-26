#include "backend.h"

#include <cmath>

#include <QDebug>
#include <QGuiApplication>
#include <QTemporaryFile>
#include <QQmlFile>
#include <QDataStream>

#include <inton-core/include/intoncore.h>
#include <inton-core/include/utils.h>

#include "applicationconfig.h"
#include "qml/qmlpoint.h"
#include <limits>


const int WAVE_LENGTH = 1000;

Backend::Backend(QObject *parent)
    : QObject(parent),
    core(nullptr),
    sound(nullptr)
{
    this->path = "";

    this->initializeRecorder();
    this->initializeAudio();
}

Backend::~Backend()
{
    qDebug() << "~Backend";

    if(this->core != nullptr) delete this->core;
}

bool Backend::isMobile()
{
#ifdef ANDROID
    return true;
#else
    return false;
#endif
}

void Backend::reinitialize()
{
    this->path = "";
    this->template_path = "";
    this->initializeCore(true);
}

QVariantList Backend::getWaveData(QString path)
{
    this->initializeRecordCore(path);

    auto storage = this->core->getRecord();

    auto data = storage->getWaveNormalized();
    auto resized_data = IntonCore::resizeVectorByMinMax(data, WAVE_LENGTH);
    qDebug() << "getWaveData: file size: " << data.size();
    qDebug() << "getWaveData: file size: " << resized_data.size();

    QVariantList waveData;

    for (ulong i=0; i<resized_data.size(); i++) {
        waveData.append(QVariant::fromValue(QmlPoint(i, resized_data[i])));
    }

    return waveData;
}

QVariant Backend::getWaveLength(QString path, bool isTemplate)
{
    IntonCore::Storage* storage = nullptr;
    if (isTemplate) {
        qDebug() << "getWaveLength: template";
        this->initializeTemplateCore(path);
        storage = this->core->getTemplate();
    } else {
        qDebug() << "getWaveLength: record";
        this->initializeRecordCore(path);
        storage = this->core->getRecord();
    }

    auto wave = storage->getWave();

    return QVariant::fromValue(1.0 * wave.size() / ApplicationConfig::RecordingFrequency);
}

QVariantList Backend::getSpectrumData(QString path, bool isTemplate)
{
    IntonCore::Storage* storage = nullptr;
    if (isTemplate) {
        qDebug() << "getSpectrumData: template";
        storage = this->core->getTemplate(path.toStdString());
    } else {
        qDebug() << "getSpectrumData: record";
        this->initializeRecordCore(path);
        storage = this->core->getRecord();
    }

    auto fftLength = this->getSettingValue(ApplicationConfig::SETTINGS_SPECTRUM_FFT_LENGTH).toUInt();

    this->core->setSpectrumConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_SPECTRUM_FRAME_LENGTH).toUInt(),
        fftLength,
        this->getSettingValue(ApplicationConfig::SETTINGS_SPECTRUM_OUTPUT_FORMAT).toUInt()
    );

    qDebug() << "getSpectrumData: getSpectrum";
    std::vector<double> data = storage->getSpectrumNormalized();

    QVariantList result;

    qDebug() << "getSpectrumData: getSpectrum " << data.size();
    for (ulong i=0; i<data.size(); i++) {
        result.append(QVariant::fromValue(QmlPoint(
            (double) i * (8000 / fftLength / 2.),
            data[i]
        )));
    }

    return result;
}

QVariantList Backend::getPitchData(QString path, bool cutted)
{
    this->initializeRecordCore(path);

    this->core->setPitchConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_ALGORITHM).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_OUTPUT_FORMAT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_FRAME_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_RAPT_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SWIPE_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SAMPLING_FREQUENCY).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MINIMUM_F0).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MAXIMUM_F0).toDouble()
    );

    qDebug() << "getPitchData: try getRecord";
    auto storage = this->core->getRecord();
    qDebug() << "getPitchData: try getPitch";
    std::vector<double> data;
    if (cutted)
    {
        data = storage->getPitchCutted();
    } else {
        data = storage->getPitch();
    }
    qDebug() << "getPitchData: try resizeVector";
    auto resized_data = IntonCore::resizeVector(data, WAVE_LENGTH);
    qDebug() << "getPitchData: file size: " << data.size();
    qDebug() << "getPitchData: file size: " << resized_data.size();

    QVariantList pitchData;

    for (ulong i=0; i<resized_data.size(); i++) {
        pitchData.append(QVariant::fromValue(QmlPoint(i, resized_data[i])));
    }

    return pitchData;
}

QVariantList Backend::getPitchOcatavesNormalizedData(QString path, bool cutted)
{
    this->initializeRecordCore(path);

    this->core->setPitchConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_ALGORITHM).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_OUTPUT_FORMAT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_FRAME_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_RAPT_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SWIPE_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SAMPLING_FREQUENCY).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MINIMUM_F0).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MAXIMUM_F0).toDouble()
    );

    this->core->setOctavesConfig(
        this->getOctavesSetting()
    );

    qDebug() << "getPitchNormalizedData: try getTemplate";
    auto storage = this->core->getRecord();
    qDebug() << "getPitchNormalizedData: try getPitch";
    std::vector<double> data;
    if (cutted)
    {
        data = storage->getPitchOctavesNormalizedCutted();
    } else {
        data = storage->getPitchOctavesNormalized();
    }
    qDebug() << "getPitchNormalizedData: try resizeVector";
    auto resized_data = IntonCore::resizeVector(data, WAVE_LENGTH);
    qDebug() << "getPitchNormalizedData: file size: " << data.size();
    qDebug() << "getPitchNormalizedData: file size: " << resized_data.size();

    QVariantList pitchData;

    for (ulong i=0; i<resized_data.size(); i++) {
        pitchData.append(QVariant::fromValue(QmlPoint(i, resized_data[i])));
    }

    return pitchData;
}

QVariantList Backend::getPitchNormalizedData(QString path, bool cutted)
{
    this->initializeRecordCore(path);

    this->core->setPitchConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_ALGORITHM).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_OUTPUT_FORMAT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_FRAME_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_RAPT_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SWIPE_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SAMPLING_FREQUENCY).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MINIMUM_F0).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MAXIMUM_F0).toDouble()
    );

    qDebug() << "getPitchNormalizedData: try getTemplate";
    auto storage = this->core->getRecord();
    qDebug() << "getPitchNormalizedData: try getPitch";
    std::vector<double> data;
    if (cutted)
    {
        data = storage->getPitchNormalizedCutted();
    } else {
        data = storage->getPitchNormalized();
    }
    qDebug() << "getPitchNormalizedData: try resizeVector";
    auto resized_data = IntonCore::resizeVector(data, WAVE_LENGTH);
    qDebug() << "getPitchNormalizedData: file size: " << data.size();
    qDebug() << "getPitchNormalizedData: file size: " << resized_data.size();

    QVariantList pitchData;

    for (ulong i=0; i<resized_data.size(); i++) {
        pitchData.append(QVariant::fromValue(QmlPoint(i, resized_data[i])));
    }

    return pitchData;
}

QVariant Backend::getPitchMinMax()
{
    if (this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_NORMALIZE_BY_OCTAVES).toBool())
    {
        return QVariant::fromValue(QPoint(0,this->getOctavesSetting().size()));
    } else if (this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_NORMALIZED).toBool())
    {
        return QVariant::fromValue(QPoint(0,1));
    } else {
        return QVariant::fromValue(QPoint(
           this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MINIMUM_F0).toDouble(),
           this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MAXIMUM_F0).toDouble()
        ));
    }
}

QVariantList Backend::getPitchFullData(QString path)
{
    QVariantList pitch;

    if (this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_NORMALIZE_BY_OCTAVES).toBool())
    {
        pitch = getPitchOcatavesNormalizedData(path);
    } else if (this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_NORMALIZED).toBool())
    {
        pitch = getPitchNormalizedData(path);
    } else {
        pitch = getPitchData(path);
    }

    return pitch;
}

QVariantList Backend::getPitchDataCutted(QString path)
{
    qDebug() << this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_FRAME);
    qDebug() << this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SHIFT);
    qDebug() << this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SMOOTH_FRAME);
    qDebug() << this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SEGMENTS_LENGTH_LIMIT);

    this->core->setIntensityConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_FRAME).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SMOOTH_FRAME).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SMOOTH_FRAME).toUInt()
    );

    this->core->setSegmentsConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SEGMENTS_LENGTH_LIMIT).toUInt()
    );

    QVariantList pitch;

    if (this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_NORMALIZE_BY_OCTAVES).toBool())
    {
        pitch = getPitchOcatavesNormalizedData(path, true);
    } else if (this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_NORMALIZED).toBool())
    {
        pitch = getPitchNormalizedData(path, true);
    } else {
        pitch = getPitchData(path, true);
    }

    return pitch;
}

QVariantList getDerivativeCount(IntonCore::Storage *storage,
                                bool isNormalizedByOctaves,
                                bool isNormalized,
                                bool isCutted)
{
    qDebug() << "getDerivativeCount " << isCutted;
    std::vector<double> data;

    if (isNormalizedByOctaves && !isCutted)
    {
        qDebug() << "getDerivativeCount: getPitchOctavesNormalizedDerivative";
        data = storage->getPitchOctavesNormalizedDerivative();
    } else if (isNormalized && !isCutted)
    {
        qDebug() << "getDerivativeCount: getPitchNormalizedDerivative";
        data = storage->getPitchNormalizedDerivative();
    } else if (isNormalizedByOctaves && isCutted)
    {
        qDebug() << "getDerivativeCount: getPitchOctavesNormalizedCuttedDerivative";
        data = storage->getPitchOctavesNormalizedCuttedDerivative();
    } else if (isNormalized && isCutted)
    {
        qDebug() << "getDerivativeCount: getPitchNormalizedCuttedDerivative";
        data = storage->getPitchNormalizedCuttedDerivative();
    } else {
        qDebug() << "getDerivativeCount: getPitchDerivative";
        data = storage->getPitchDerivative();
    }

    int less_zero = 0;
    int eq_zero = 0;
    int more_zero = 0;

    for (auto &it: data)
    {
        if (it < 0)
        {
            less_zero++;
        } else if (it > 0)
        {
            more_zero++;
        } else
        {
            eq_zero++;
        }
    }

    //double max = (less_zero>eq_zero) ? ((less_zero>more_zero) ? less_zero : more_zero) : ((eq_zero>more_zero) ? eq_zero : more_zero) ;
    double max = more_zero>less_zero ? more_zero : less_zero;

    qDebug() << "getDerivativeCount less_zero" << less_zero;
    qDebug() << "getDerivativeCount more_zero" << more_zero;
    qDebug() << "getDerivativeCount eq_zero" << eq_zero;
    qDebug() << "getDerivativeCount max" << max;

    QVariantList count;
    //count.append(eq_zero / max);
    count.append(more_zero / max);
    count.append(less_zero / max);

    return count;
}

QVariantList Backend::getPitchDerivativeCount(QString path, bool isFull)
{
    this->initializeRecordCore(path);

    this->core->setPitchConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_ALGORITHM).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_OUTPUT_FORMAT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_FRAME_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_RAPT_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SWIPE_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SAMPLING_FREQUENCY).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MINIMUM_F0).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MAXIMUM_F0).toDouble()
    );

    qDebug() << "getPitchNormalizedData: try getTemplate";
    auto storage = this->core->getRecord();

    auto data = getDerivativeCount(storage,
                                   this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_NORMALIZE_BY_OCTAVES).toBool(),
                                   this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_NORMALIZED).toBool(),
                                   !isFull);

    return data;
}

QVariantList Backend::getTemplatePitchDerivativeCount(QString path, bool isFull)
{
    this->initializeTemplateCore(path);

    this->core->setPitchConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_ALGORITHM).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_OUTPUT_FORMAT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_FRAME_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_RAPT_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SWIPE_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SAMPLING_FREQUENCY).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MINIMUM_F0).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MAXIMUM_F0).toDouble()
    );

    qDebug() << "getPitchNormalizedData: try getTemplate";
    auto storage = this->core->getTemplate();

    auto data = getDerivativeCount(storage,
                                   this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_NORMALIZE_BY_OCTAVES).toBool(),
                                   this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_NORMALIZED).toBool(),
                                   !isFull);

    return data;
}

QVariantList calculatePitchSegmentsCount(
        IntonCore::Storage * storage,
        std::list<double> actaves,
        bool mean,
        double threshold)
{
    auto pitch = storage->getPitch();

    qDebug() << "calculatePitchSegmentsCount: threshold " << threshold;

    auto intensity = storage->getIntensity();
    qDebug() << "getPitchDataCutted intensity " << intensity.size();
    auto data = storage->getAutoSegmentsByIntensitySmoothed();
    qDebug() << "getPitchDataCutted count " << data.size();

    uint32_t pos = 0;
    for (auto &it: data)
    {
        auto to = IntonCore::normalizeValue(it.first, intensity.size(), pitch.size());
        for (auto i = pos; i <= to && i < pitch.size(); i++)
        {
            pitch[i] = 0;
        }
        pos = to + IntonCore::normalizeValue(it.second, intensity.size(), pitch.size());
    }

    for (auto i = pos; i < pitch.size(); i++)
    {
        pitch[i] = 0;
    }

    QMap<int, int> counts;

    int max = 0;

    qDebug() << "calculatePitchSegmentsCount: mean " << mean;
    if (!mean)
    {
        for (ulong i=0; i<pitch.size(); i++)
        {
            int j = 0;
            for (double &actave : actaves)
            {
                if (pitch[i] != 0 && pitch[i] < actave)
                {
                    counts[j] = counts.value(j, 0) + 1;
                    if (max < counts[j]) max = counts[j];
                    break;
                }
                j ++;
            }
        }
    }

    if (mean)
    {
        double min = *min_element(pitch.begin(), pitch.end());
        qDebug() << "calculatePitchSegmentsCount: min " << min;

        int count = 0;
        double sum = 0;

        for (double &element : pitch)
        {
            if (element == min) {
                if (count != 0) {
                    double mean_value = sum / (count+1);
                    int j = -1;
                    for (double &actave : actaves)
                    {
                        j++;
                        if (actave > mean_value) break;
                    }
                    counts[j] = counts.value(j, 0) + 1;
                    if (max < counts[j]) max = counts[j];
                    count = 0;
                    sum = 0;
                }
            } else {
                sum += element;
                count++;
            }
        }
    }

    QVariantList result;

    for (int i=0; i<actaves.size(); i++)
    {
        double value = 1.0 * counts.value(i, 0) / max;
        if (value < threshold)
        {
            result.append(0);
        } else {
            value = (value-threshold)/(1.0-threshold);
            result.append(value);
        }


    }

    result.append(max);

    return result;
}

QVariantList Backend::getPitchFrequencySegmentsCount(QString path, bool isTemplate)
{
    IntonCore::Storage* storage = nullptr;
    if (isTemplate) {
        qDebug() << "getSpectrumData: template";
        storage = this->core->getTemplate(path.toStdString());
    } else {
        qDebug() << "getSpectrumData: record";
        this->initializeRecordCore(path);
        storage = this->core->getRecord();
    }

    this->core->setPitchConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_ALGORITHM).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_OUTPUT_FORMAT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_FRAME_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_RAPT_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SWIPE_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SAMPLING_FREQUENCY).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MINIMUM_F0).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MAXIMUM_F0).toDouble()
    );

    this->core->setIntensityConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_FRAME).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SMOOTH_FRAME).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SMOOTH_FRAME).toUInt()
    );

    this->core->setSegmentsConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SEGMENTS_LENGTH_LIMIT).toUInt()
    );

    auto threshold = this->getSettingValue(ApplicationConfig::SETTINGS_HISTOGRAM_THRESHOLD).toDouble();

    auto actaves = this->getOctavesSetting(true);

    auto mean = this->getSettingValue(ApplicationConfig::SETTINGS_HISTOGRAM_MEAN).toBool();

    auto result = calculatePitchSegmentsCount(storage, actaves, mean, threshold);

    return result;
}

QVariantList Backend::getPitchFrequencyCount(QString path)
{
    this->initializeRecordCore(path);

    this->core->setPitchConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_ALGORITHM).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_OUTPUT_FORMAT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_FRAME_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_RAPT_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SWIPE_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SAMPLING_FREQUENCY).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MINIMUM_F0).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MAXIMUM_F0).toDouble()
    );

    this->core->setIntensityConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_FRAME).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SMOOTH_FRAME).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SMOOTH_FRAME).toUInt()
    );

    this->core->setSegmentsConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SEGMENTS_LENGTH_LIMIT).toUInt()
    );

    auto threshold = this->getSettingValue(ApplicationConfig::SETTINGS_HISTOGRAM_THRESHOLD).toDouble();

    auto storage = this->core->getRecord();

    auto actaves = this->getOctavesSetting();

    auto mean = this->getSettingValue(ApplicationConfig::SETTINGS_HISTOGRAM_MEAN).toBool();

    auto result = calculatePitchSegmentsCount(storage, actaves, mean, threshold);

    return result;
}

QVariantList Backend::getTemplatePitchFrequencyCount(QString path)
{
    this->initializeTemplateCore(path);

    this->core->setPitchConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_ALGORITHM).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_OUTPUT_FORMAT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_FRAME_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_RAPT_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SWIPE_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SAMPLING_FREQUENCY).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MINIMUM_F0).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MAXIMUM_F0).toDouble()
    );

    this->core->setIntensityConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_FRAME).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SMOOTH_FRAME).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SMOOTH_FRAME).toUInt()
    );

    this->core->setSegmentsConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SEGMENTS_LENGTH_LIMIT).toUInt()
    );

    auto threshold = this->getSettingValue(ApplicationConfig::SETTINGS_HISTOGRAM_THRESHOLD).toDouble();

    auto storage = this->core->getTemplate();

    auto actaves = this->getOctavesSetting();

    auto mean = this->getSettingValue(ApplicationConfig::SETTINGS_HISTOGRAM_MEAN).toBool();

    auto result = calculatePitchSegmentsCount(storage, actaves, mean, threshold);

    return result;
}

QVariantList Backend::getOctavesOptimazedCategories()
{
    auto actaves = this->getOctavesSetting();

    const int MAX_CATEGORIES = 1;
    int actavesSize = actaves.size();
    int count = 1;

    qDebug() << "actavesSize = " << actavesSize;

    if (actavesSize <= MAX_CATEGORIES) {
        return this->getOctavesCategories(false);
    }

    for (int i=MAX_CATEGORIES; i>0; i--)
    {
        count = actavesSize / i;
        if (actavesSize % i == 0) break;
    }

    qDebug() << "count = " << count;

    QVariantList result;

    QString value = "";
    QString currentValue = QString::number(actaves.front());
    int index = 0;
    bool start = false;


    for (auto &it: actaves)
    {
        index++;

        if (!start) {
            value = currentValue;// + " ... ";
            start = true;
        }

        if (index == count)
        {
            //value += QString::number(it);
            currentValue = QString::number(it);
            result.append(value);
            start = false;
            index = 0;
        }
    }

    return result;
}

QVariantList Backend::getOctavesCategories(bool isFull)
{
    auto actaves = this->getOctavesSetting(isFull);

    QVariantList result;

    for (auto &it: actaves)
    {
        result.append(it);
    }

    return result;
}

double CG(QVariantList data, int min, int max)
{
    auto sumNF0 = 0.0;
    auto sumF0 = 0.0;
    auto CG = 0.0;
    auto x = 0.0;
    for (int i=min; i<(max-1); i++)
    {
        x = data.at(i).toDouble();
        sumF0 += x;
        sumNF0 += x * (i+1);
    }
    qDebug() << "GC sumF0 " << sumF0;
    if (sumF0 != 0 && sumF0 == sumF0)
    {
        CG = sumNF0 / sumF0;
    }

    return CG;
}

int CG(QVariantList data, int min, int max, int powK)
{
    auto sumNF0 = 0.0;
    auto sumF0 = 0.0;
    auto CG = min;
    for (int i=min; i<(max-1); i++)
    {
        auto val = pow(data.at(i).toDouble(), powK);
        sumF0 += val;
        sumNF0 += val * (i+1);
    }
    qDebug() << "GC sumF0 " << sumF0;
    if (sumF0 != 0 && sumF0 == sumF0)
    {
        CG = round(sumNF0 / sumF0);
    }

    return CG;
}

int CG2(QVariantList data, int min, int max)
{
    auto sumNF0 = 0.0;
    auto sumF0 = 0.0;
    auto CG = 0;
    for (int i=min; i<(max-1); i++)
    {
        sumF0 += data.at(i).toDouble()*data.at(i).toDouble();
        sumNF0 += data.at(i).toDouble()*data.at(i).toDouble() * (i+1);
    }
    qDebug() << "GC sumF0 " << sumF0;
    if (sumF0 != 0 && sumF0 == sumF0)
    {
        CG = round(sumNF0 / sumF0);
    }

    return CG;
}

QVariantList getOcavesMetrics(
    QVariantList data,
    int F0Min,
    int F0Step,
    int powK)
{
    qDebug() << "getOcavesMetrics F0Min: " << F0Min;
    qDebug() << "getOcavesMetrics F0Step: " << F0Step;
    qDebug() << "getOcavesMetrics data: " << data.size();

    int N0 = CG2(data, 0, data.size());
    int N0m = CG(data, 0, N0, powK);
    int N0p = CG(data, N0, data.size(), powK);

    qDebug() << "getOcavesMetrics N0: " << N0;
    qDebug() << "getOcavesMetrics N0-: " << N0m;
    qDebug() << "getOcavesMetrics N0+: " << N0p;

    int RF0 = N0*F0Step+F0Min;

    int DN = N0p - N0m;
    int DF0 = (N0p - N0m) * F0Step;

    int AN = (N0p + N0m) - 2*N0;
    int AF0 = AN*F0Step;

    qDebug() << "getOcavesMetrics RF0: " << RF0;
    qDebug() << "getOcavesMetrics DN: " << DN;
    qDebug() << "getOcavesMetrics DF0: " << DF0;
    qDebug() << "getOcavesMetrics AN: " << AN;
    qDebug() << "getOcavesMetrics AF0: " << AF0;

    QVariantList result;
    result.append(N0);  // 0
    result.append(N0m); // 1
    result.append(N0p); // 2
    result.append(RF0);  // 3 C
    result.append(DN);  // 4
    result.append(DF0);  // 5 B
    result.append(AN);  // 6
    result.append(AF0);  // 7 A
    return result;
}

QVector<double> QVariantList2QVector(QVariantList input)
{
    QVector<double> result;
    bool ok = false;
    for(auto el: input)
    {
        auto doubleValue = el.toDouble(&ok);
        if (ok)
        {
            result.append(doubleValue);
            continue;
        }
        ok = el.canConvert<QmlPoint>();
        if (ok)
        {
            result.append(el.value<QmlPoint>().getY());
            continue;
        }
    }

    return result;
}

double distance(QVector<double> a, QVector<double> b)
{
    //sqrt((A1 - B1)^2 + (A2 - B2)^2 + ... + (An - Bn)^2)
    auto sum = 0.0;
    for (auto i=0; i<a.count() && i<b.count(); i++)
    {
        sum += pow(a.at(i) - b.at(i), 2);
    }
    return sqrt(sum);
}

double euclidean_distance(const double& pt1x, const double& pt1y, const double& pt2x, const double& pt2y) {
    double dx = pt1x - pt2x;
    double dy = pt1y - pt2y;
    return std::sqrt(dx * dx + dy * dy);
}

// Define a function to compute the minimum distance between two points on two curves
double compute_min_distance(const QVector<double>& curve1, const QVector<double>& curve2, int i, int j, std::vector<std::vector<double>>& min_distances) {
    // Check if the minimum distance has already been computed
    if (min_distances[i][j] > -1) {
        return min_distances[i][j];
    }

    // Compute the minimum distance recursively
    double dist = euclidean_distance(i, curve1[i], j, curve2[j]);
    if (i == 0 && j == 0) {
        // Base case
        min_distances[i][j] = dist;
    } else if (i > 0 && j == 0) {
        // Handle first column
        min_distances[i][j] = std::max(dist, compute_min_distance(curve1, curve2, i-1, 0, min_distances));
    } else if (i == 0 && j > 0) {
        // Handle first row
        min_distances[i][j] = std::max(dist, compute_min_distance(curve1, curve2, 0, j-1, min_distances));
    } else {
        // Handle other cases
        min_distances[i][j] = std::max(dist, std::min(std::min(compute_min_distance(curve1, curve2, i-1, j, min_distances),
                                                               compute_min_distance(curve1, curve2, i, j-1, min_distances)),
                                                       compute_min_distance(curve1, curve2, i-1, j-1, min_distances)));
    }
    return min_distances[i][j];
}

// Compute the Frechet distance between the two curves using the compute_min_distance function
double compute_frechet_distance(const QVector<double>& curve1, const QVector<double>& curve2) {
    // Initialize the minimum distance matrix
    std::vector<std::vector<double>> min_distances(curve1.size(), std::vector<double>(curve2.size(), -1));

    // Compute the minimum distance between points on the two curves
    return compute_min_distance(curve1, curve2, curve1.size()-1, curve2.size()-1, min_distances);
}

double crossCorrelationMax(const QVector<double>& x, const QVector<double>& y) {
    auto maxdelay = std::max(x.size(), y.size()) / 4;
    double mx = 0;
    double my = 0;
    auto n = std::min(x.size(), y.size());
    for (auto i=0;i<n;i++) {
       mx += x[i];
       my += y[i];
    }
    mx /= n;
    my /= n;

    /* Calculate the denominator */
    double sx = 0;
    double sy = 0;
    for (auto i=0;i<n;i++) {
       sx += (x[i] - mx) * (x[i] - mx);
       sy += (y[i] - my) * (y[i] - my);
    }
    auto denom = sqrt(sx*sy);

    auto max = -99999.0;
    /* Calculate the correlation series */
    for (auto delay=-maxdelay;delay<maxdelay;delay++) {
       double sxy = 0;
       for (auto i=0;i<n;i++) {
          auto j = i + delay;
          while (j < 0)
             j += n;
          j %= n;
          sxy += (x[i] - mx) * (y[j] - my);
       }
       auto r = sxy / denom;
       if (max < r) max = r;
    }

    return max;
}

QVariantList Backend::getMetrics(QString path, QString maxKey, QString minKey)
{
    QVariantList result;

    auto recordSegments = this->getPitchFrequencySegmentsCount(path, false);
    auto maxTemplateSegments = this->getPitchFrequencySegmentsCount(maxKey, true);
    auto minTemplateSegments = this->getPitchFrequencySegmentsCount(minKey, true);

    auto actaves = this->getOctavesSetting(true);
    double F0Min = actaves.front();
    double F0Step = (actaves.back() - actaves.front())/(actaves.size()-1);

    auto recordN0 = CG2(recordSegments, 0, recordSegments.size());
    auto recordD0m = CG(recordSegments, 0, round(recordN0)-1);
    auto recordD0p = CG(recordSegments, round(recordN0)+1, recordSegments.size());
    auto recordDN = recordD0p - recordD0m;

    auto maxN0 = CG2(maxTemplateSegments, 0, maxTemplateSegments.size());
    auto maxD0m = CG(maxTemplateSegments, 0, round(maxN0)-1);
    auto maxD0p = CG(maxTemplateSegments, round(maxN0)+1, maxTemplateSegments.size());
    auto maxDN = maxD0p - maxD0m;

    auto minN0 = CG2(minTemplateSegments, 0, minTemplateSegments.size());
    auto minD0m = CG(minTemplateSegments, 0, round(minN0)-1);
    auto minD0p = CG(minTemplateSegments, round(minN0)+1, minTemplateSegments.size());
    auto minDN = minD0p - minD0m;

    auto minN0Hz = minN0*F0Step+F0Min;
    auto recordN0Hz = recordN0*F0Step+F0Min;
    auto maxN0Hz = maxN0*F0Step+F0Min;
    auto minDNHz = minDN*F0Step;
    auto recordDNHz = recordDN*F0Step;
    auto maxDNHz = maxDN*F0Step;

    QString pattern = "%1 (min/record/max): %2/%3/%4 Hz";
    result.append(pattern.arg("C")
                  .arg(minN0Hz, 0, 'f', 0)
                  .arg(recordN0Hz, 0, 'f', 0)
                  .arg(maxN0Hz, 0, 'f', 0));
    result.append(pattern.arg("B")
                  .arg(minDNHz, 0, 'f', 0)
                  .arg(recordDNHz, 0, 'f', 0)
                  .arg(maxDNHz, 0, 'f', 0));

    if (recordN0Hz < minN0Hz) recordN0Hz = minN0Hz;
    if (recordN0Hz > maxN0Hz) recordN0Hz = maxN0Hz;
    if (recordDNHz < minDNHz) recordDNHz = minDNHz;
    if (recordDNHz > maxDNHz) recordDNHz = maxDNHz;

    auto vemN0DN = ((recordN0Hz + recordDNHz) - (minN0Hz + minDNHz )) / ((maxN0Hz + maxDNHz) - (minN0Hz + minDNHz));
    result.append(vemN0DN);

    return result;
}

QVariantList Backend::getMetricsSingle(QString path, QString minKey, bool isMaleTemplate)
{
    QVariantList result;

    auto recordSegments = this->getPitchFrequencySegmentsCount(path, false);
    auto minTemplateSegments = this->getPitchFrequencySegmentsCount(minKey, true);

    auto actaves = this->getOctavesSetting(true);
    double F0Min = actaves.front();
    double F0Step = (actaves.back() - actaves.front())/(actaves.size()-1);

    auto recordN0 = CG2(recordSegments, 0, recordSegments.size());
    auto recordD0m = CG(recordSegments, 0, round(recordN0)-1);
    auto recordD0p = CG(recordSegments, round(recordN0)+1, recordSegments.size());
    auto recordDN = recordD0p - recordD0m;

    auto minN0 = CG2(minTemplateSegments, 0, minTemplateSegments.size());
    auto minD0m = CG(minTemplateSegments, 0, round(minN0)-1);
    auto minD0p = CG(minTemplateSegments, round(minN0)+1, minTemplateSegments.size());
    auto minDN = minD0p - minD0m;

    auto minN0Hz = minN0*F0Step+F0Min;
    auto recordN0Hz = recordN0*F0Step+F0Min;
    auto minDNHz = minDN*F0Step;
    auto recordDNHz = recordDN*F0Step;

    auto maxN0Hz = minN0Hz * (isMaleTemplate ?
                this->getSettingValue(ApplicationConfig::SETTINGS_MALE_DC).toDouble() :
                this->getSettingValue(ApplicationConfig::SETTINGS_FEMALE_DC).toDouble());
    auto maxDNHz = minDNHz * (isMaleTemplate ?
                this->getSettingValue(ApplicationConfig::SETTINGS_MALE_DB).toDouble() :
                this->getSettingValue(ApplicationConfig::SETTINGS_FEMALE_DB).toDouble());

    QString pattern = "%1 (min/record/max): %2/%3/%4 Hz";
    result.append(pattern.arg("C")
                  .arg(minN0Hz, 0, 'f', 0)
                  .arg(recordN0Hz, 0, 'f', 0)
                  .arg(maxN0Hz, 0, 'f', 0));
    result.append(pattern.arg("B")
                  .arg(minDNHz, 0, 'f', 0)
                  .arg(recordDNHz, 0, 'f', 0)
                  .arg(maxDNHz, 0, 'f', 0));

    if (recordN0Hz < minN0Hz) recordN0Hz = minN0Hz;
    if (recordN0Hz > maxN0Hz) recordN0Hz = maxN0Hz;
    if (recordDNHz < minDNHz) recordDNHz = minDNHz;
    if (recordDNHz > maxDNHz) recordDNHz = maxDNHz;

    auto vemN0DN = ((recordN0Hz + recordDNHz) - (minN0Hz + minDNHz )) / ((maxN0Hz + maxDNHz) - (minN0Hz + minDNHz));
    result.append(vemN0DN);

    return result;
}

QVariantList Backend::getPitchOcavesMetrics(QString path, bool isTemplate)
{
    qDebug() << "getPitchOcavesMetrics: isTemplate " << isTemplate;
    IntonCore::Storage* storage = nullptr;
    if (isTemplate) {
        qDebug() << "getSpectrumData: template";
        this->initializeTemplateCore(path);
        storage = this->core->getTemplate();
    } else {
        qDebug() << "getSpectrumData: record";
        this->initializeRecordCore(path);
        storage = this->core->getRecord();
    }

    this->core->setPitchConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_ALGORITHM).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_OUTPUT_FORMAT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_FRAME_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_RAPT_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SWIPE_THRESHOLD).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_SAMPLING_FREQUENCY).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MINIMUM_F0).toDouble(),
        this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_MAXIMUM_F0).toDouble()
    );

    this->core->setIntensityConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_FRAME).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SMOOTH_FRAME).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SMOOTH_FRAME).toUInt()
    );

    this->core->setSegmentsConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SEGMENTS_LENGTH_LIMIT).toUInt()
    );

    auto actaves = this->getOctavesSetting(true);

    auto mean = this->getSettingValue(ApplicationConfig::SETTINGS_HISTOGRAM_MEAN).toBool();
    auto threshold = this->getSettingValue(ApplicationConfig::SETTINGS_HISTOGRAM_THRESHOLD).toDouble();
    auto DF0 = this->getSettingValue(ApplicationConfig::SETTINGS_HISTOGRAM_F0MIN).toInt();
    auto powK = this->getSettingValue(ApplicationConfig::SETTINGS_HISTOGRAM_F0POWK).toInt();
    auto DF0ind = this->getSettingValue(ApplicationConfig::SETTINGS_HISTOGRAM_DF0).toInt();
    if (this->getSettingValue(ApplicationConfig::SETTINGS_HISTOGRAM_OCTAVES_ENABLED).toString() == ApplicationConfig::SETTINGS_TRUE ) {
        DF0ind = (actaves.back() - actaves.front()) / actaves.size();
    }

    auto data = calculatePitchSegmentsCount(storage, actaves, mean, threshold);

    auto result = getOcavesMetrics(data, DF0, DF0ind, powK);

    return result;
}

QVariantList Backend::getSegmentsByIntensity(QString path)
{
    this->initializeRecordCore(path);

    this->core->setIntensityConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_FRAME).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SHIFT).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SMOOTH_FRAME).toUInt(),
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SMOOTH_FRAME).toUInt()
    );

    this->core->setSegmentsConfig(
        this->getSettingValue(ApplicationConfig::SETTINGS_SEGMENTS_SEGMENTS_LENGTH_LIMIT).toUInt()
    );

    auto storage = this->core->getRecord();
    auto intensity = storage->getIntensity();

    auto data = storage->getAutoSegmentsByIntensitySmoothed();
//    qDebug() << "Segments by intensity count " << data.size();

    QVariantList segments;

    for (auto &it: data)
    {
        it.first = IntonCore::normalizeValue(it.first, intensity.size(), WAVE_LENGTH);
        it.second = IntonCore::normalizeValue(it.second, intensity.size(), WAVE_LENGTH);
        segments.append(QVariant::fromValue(QmlPoint(it.first, it.first + it.second)));
    }

    return segments;
}

QVariantList calculateSingerMetrics(QVariantList segmentsCount,
                                    QMap<QString, QStringList> segments,
                                    QStringList voiceLables)
{
    qDebug() << "calculateSingerMetrics:segmentsCount " << segmentsCount;
    qDebug() << "calculateSingerMetrics:segmentsCount len " << segmentsCount.size();

    QVariantList metrics;
    int maxI = 0;
    double max = std::numeric_limits<double>::min();
    double min = std::numeric_limits<double>::max();

    for (int i=0; i<voiceLables.size(); i++)
    {
        auto voiceLable = voiceLables.at(i);
        auto voiceData = segments.value(voiceLable);
        qDebug() << "voiceData " << voiceData;
        qDebug() << "voiceData len " << voiceData.size();

        auto metric = 0.0;

        for (auto i=0; i<segmentsCount.size() && i<voiceData.size(); i++)
        {
            metric += voiceData.at(i).toDouble() * segmentsCount.at(i).toDouble();
        }

        qDebug() << "metric " << voiceLable << " " << metric;
        metrics.append(metric);

        if (max <= metric)
        {
            max = metric;
            maxI = i;
        }

        if (min > metric)
        {
            min = metric;
        }
    }

    metrics.append(maxI);

    for (int i=0; i<voiceLables.size(); i++)
    {
        auto metric = metrics.at(i).toDouble();
        metrics.replace(i, metric / max);
        qDebug() << "calculateSingerMetrics:metric " << i << ": " << metric;
    }

    return metrics;
}

QString Backend::getPath()
{
    return this->path;
}

void Backend::setPath(const QString &path)
{
    qDebug() << "set path: " << path;
    this->path = path;
}

void Backend::initializeCore(bool reinit)
{
    qDebug() << "Backend::initializeCore " << reinit;
    if (this->core != nullptr && !reinit) return;

    if (this->core != nullptr)
    {
        qDebug() << "Backend::initializeCore: Delete core";
        delete this->core;
        this->core = nullptr;
    }

    QString recordsPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QString logPath = QDir(recordsPath).absoluteFilePath("core.log");
    qDebug() << "Backend::initializeCore: logPath: " << logPath;

    //Settings * settings = Settings::getInstance();

    qDebug() << "Backend::initializeCore: Initialize core: " << this->path;
    this->core = new IntonCore::Core(
        this->path.toStdString()//,
        //settings->getConfig()
    );
    qDebug() << "Backend::initializeCore: Initialize core complete";
}

void Backend::initializeRecordCore(const QString& path)
{
    qDebug() << "Backend::initializeRecordCore: path" << path;
    if (this->path == path)
    {
        qDebug() << "Backend::initializeRecordCore: ok";
        return;
    }
    qDebug() << "Backend::initializeRecordCore: old path" << this->path;

    if (!path.isEmpty())
    {
        qDebug() << "Backend::initializeRecordCore: initialize core" << path;
        this->initializeCore();
        if (path.startsWith("content:"))
        {
            QFile qfile(QQmlFile::urlToLocalFileOrQrc(path));
            if (!qfile.open(QFile::ReadOnly)) {
                qDebug() << "Backend::initializeRecordCore: failed to load file" << path;
                return;
            }

            auto temporary_file = QTemporaryFile::createNativeFile(qfile);
            auto full_path = temporary_file->fileName();
            qDebug() << "initializeCore: load wav file" << full_path;
            WaveFile * file = IntonCore::Helpers::openWaveFile(full_path.toStdString());
            qDebug() << "initializeCore: reload record" << full_path;

            this->core->reloadRecord(file);
        } else {
            qDebug() << "Backend::initializeRecordCore: load wav file" << path;
            WaveFile * file = IntonCore::Helpers::openWaveFile(path.toStdString());
            qDebug() << "Backend::initializeRecordCore: reload record" << path;
            this->core->reloadRecord(file);
        }
    }

    this->setPath(path);
}

void Backend::initializeTemplateCore(const QString& path)
{
    qDebug() << "initializeTemplateCore: path" << path;
    if (this->template_path == path)
    {
        qDebug() << "initializeTemplateCore: ok";
        return;
    }
    qDebug() << "initializeTemplateCore: old path" << this->template_path;

    if (!path.isEmpty())
    {
        qDebug() << "initializeTemplateCore: initialize core" << path;
        this->initializeCore();
        qDebug() << "initializeTemplateCore: load wav file" << path;
        WaveFile * file = IntonCore::Helpers::openWaveFile(path.toStdString());
        qDebug() << "initializeTemplateCore: reload template" << path;
        this->core->reloadTemplate(file);
    }

    this->setPath(path);
}

QString Backend::openTemplate(QString key, QString path)
{
#ifdef ANDROID
    const QString basePath = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).at(0) +
            QDir::separator() + "SpeechIntonationAssistant";
#else
    const QString basePath = ApplicationConfig::GetFullPatternsPath();
#endif
    QDir directory(basePath);
    auto fullPath = directory.absoluteFilePath(path);
    qDebug() << "Backend::openTemplate: initialize core" << key << ":" << fullPath;
    this->initializeCore();
    this->core->reloadTemplate(fullPath.toStdString(), key.toStdString());
    return fullPath;
}

void Backend::moveTemplateToRecord()
{
    if (this->core == nullptr) return;
    qDebug() << "moveTemplateToRecord";
    auto storage = this->core->popTemplate();
    qDebug() << "storage: " << (storage == nullptr);
    this->core->setRecord(storage);
}

void Backend::moveRecordToTemplate()
{
    if (this->core == nullptr) return;
    qDebug() << "moveRecordToTemplate";
    auto storage = this->core->popRecord();
    qDebug() << "storage: " << (storage == nullptr);
    this->core->setTemplate(storage);
}
