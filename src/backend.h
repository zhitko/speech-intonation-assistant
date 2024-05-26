#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

#include <QList>
#include <QVariant>
#include <QVariantList>

namespace IntonCore {
class Core;
}

class Recorder;
class PcmRecorder;
class Settings;

class QSound;

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    virtual ~Backend();

    Q_PROPERTY(QString path READ getPath WRITE setPath)

    Q_INVOKABLE bool isMobile();
    Q_INVOKABLE void reinitialize();

    // Settings API
    Q_INVOKABLE QVariant getSettingValue(QString id);
    Q_INVOKABLE QVariantList getSettings();
    Q_INVOKABLE QString setSettings(QString id, QString value);
    Q_INVOKABLE bool validateSettings(QString id, QString value);

    std::list<double> getOctavesSetting(bool isFull = false);

    // Files API
    Q_INVOKABLE QVariantList getWaveFilesList(QString subpath);
    Q_INVOKABLE QVariantList getTestsWaveFilesList();
    Q_INVOKABLE void copyToFiles(QString filePath, QString name);
    Q_INVOKABLE void copyToTemplates(QString filePath, QString subPath, QString name);
    Q_INVOKABLE void deleteWaveFile(QString path);
    Q_INVOKABLE void playWaveFile(QString path, bool stop);
    Q_INVOKABLE QString startStopRecordWaveFile(bool isTemplate, bool isSetPath);
    Q_INVOKABLE QString openFileDialog();

    Q_INVOKABLE QString getSavedResults();
    Q_INVOKABLE void saveResult(QVariantMap result);

    // WAVE file API
    Q_INVOKABLE QVariantList getWaveData(QString path);
    Q_INVOKABLE QVariant getWaveLength(QString path, bool isTemplate);

    // Pitch API
    Q_INVOKABLE QVariantList getPitchData(QString path, bool cutted = false);
    Q_INVOKABLE QVariantList getPitchNormalizedData(QString path, bool cutted = false);
    Q_INVOKABLE QVariantList getPitchOcatavesNormalizedData(QString path, bool cutted = false);
    Q_INVOKABLE QVariantList getPitchFullData(QString path);
    Q_INVOKABLE QVariantList getPitchDataCutted(QString path);
    Q_INVOKABLE QVariant getPitchMinMax();

    Q_INVOKABLE QVariantList getPitchFrequencySegmentsCount(QString path, bool isTemplate);
    Q_INVOKABLE QVariantList getPitchFrequencyCount(QString path);
    Q_INVOKABLE QVariantList getTemplatePitchFrequencyCount(QString path);

    Q_INVOKABLE QVariantList getPitchDerivativeCount(QString path, bool isFull);
    Q_INVOKABLE QVariantList getTemplatePitchDerivativeCount(QString path, bool isFull);

    Q_INVOKABLE QVariantList getOctavesCategories(bool isFull);
    Q_INVOKABLE QVariantList getOctavesOptimazedCategories();

    // Spectrum API
    Q_INVOKABLE QVariantList getSpectrumData(QString path, bool isTemplate);

    // Segments API
    Q_INVOKABLE QVariantList getSegmentsByIntensity(QString path);

    // Metrics API
    Q_INVOKABLE QVariantList getPitchOcavesMetrics(QString path, bool isTemplate);
    Q_INVOKABLE QVariantList getMetrics(QString path, QString maxKey, QString minKey);
    Q_INVOKABLE QVariantList getMetricsSingle(QString path, QString minKey, bool isMaleTemplate);

    // Articulation API
    Q_INVOKABLE QVariant getArticulationRate(QString path, bool isTemplate);
    Q_INVOKABLE QVariant getSpeechRate(QString path, bool isTemplate);
    Q_INVOKABLE QVariant getConsonantsAndSilenceMedianValue(QString path, bool isTemplate);
    Q_INVOKABLE QVariant getMeanDurationOfPauses(QString path, bool isTemplate);

    // System API
    Q_INVOKABLE QString openTemplate(QString key, QString path);
    Q_INVOKABLE void moveTemplateToRecord();
    Q_INVOKABLE void moveRecordToTemplate();
public:
    QString getPath();
    void setPath(const QString& path);

private:
    IntonCore::Core *core;

    QString path;
    QString template_path;

    QSound *sound;

    PcmRecorder *recorder;

private:
    void initializeCore(bool reinit = false);
    void initializeRecordCore(const QString& path);
    void initializeTemplateCore(const QString& path);

    void initializeRecorder();
    void initializeAudio();
signals:

};

#endif // BACKEND_H
