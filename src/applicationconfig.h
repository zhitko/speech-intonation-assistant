#ifndef APPLICATIONCONFIG_H
#define APPLICATIONCONFIG_H

#include <QString>
#include <QStringList>
#include <QGuiApplication>
#include <QDir>
#include <QStandardPaths>

#ifdef ANDROID
#include <QtAndroid>
#endif

namespace ApplicationConfig {
#ifdef ANDROID
    static const QString DataPath = "SpeechIntonationAssistant";
#else
    static const QString DataPath = "data";
#endif
    static const QString RecordsPath = "records";
    static const QString TestsPath = "tests";
    static const QString PatternsPath = "patterns";

    static QString getDataPath()
    {
#ifdef ANDROID
        return QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
        return QGuiApplication::applicationDirPath();
#endif
    }

#ifdef ANDROID
    static bool requestAndroidExternalStoragePermissions() {
        const QVector<QString> permissions(
            {"android.permission.WRITE_EXTERNAL_STORAGE",
             "android.permission.READ_EXTERNAL_STORAGE",
             "android.permission.READ_MEDIA_AUDIO"}
            );
        for (const QString &permission : permissions) {
            auto result = QtAndroid::checkPermission(permission);
            if (result == QtAndroid::PermissionResult::Denied) {
                auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
                if (resultHash[permission] == QtAndroid::PermissionResult::Denied) {
                    return false;
                }
            }
        }
        return true;
    }
#endif

    static QString checkAndCreatePath(QString path)
    {
#ifdef ANDROID
        ApplicationConfig::requestAndroidExternalStoragePermissions();
#endif
        QDir dir(path);
        if (!dir.exists())
        {
            dir.mkpath(".");
        }
        return path;
    }

    static QString GetFullDataPath()
    {
        QString basePath = ApplicationConfig::getDataPath();
        QString dataPath = QDir(basePath).absoluteFilePath(ApplicationConfig::DataPath);
        QString path = QDir(dataPath).absoluteFilePath(ApplicationConfig::RecordsPath);
        return checkAndCreatePath(path);
    }

    static QString GetFullTestsPath()
    {
        QString basePath = ApplicationConfig::getDataPath();
        QString dataPath = QDir(basePath).absoluteFilePath(ApplicationConfig::DataPath);
        QString path = QDir(dataPath).absoluteFilePath(ApplicationConfig::TestsPath);
        return checkAndCreatePath(path);
    }

    static QString GetFullPatternsPath(QString subpath = "")
    {
        QString basePath = ApplicationConfig::getDataPath();
        QString dataPath = QDir(basePath).absoluteFilePath(ApplicationConfig::DataPath);
        QString path = QDir(dataPath).absoluteFilePath(ApplicationConfig::PatternsPath);
        QString result = QDir(path).absoluteFilePath(subpath);
        return checkAndCreatePath(result);
    }

    static const QString WaveFileExtension = "*.wav";

    static const QStringList WaveFileFilter = { WaveFileExtension };

    static const QString SettingsPath = "settings.ini";

    static QString GetFullSettingsPath()
    {
        QString basePath = ApplicationConfig::getDataPath();
        QString dataPath = QDir(basePath).absoluteFilePath(ApplicationConfig::SettingsPath);
        return dataPath;
    }

    static const QString ResultsPath = "results.csv";

    static QString GetFullResultsPath()
    {
        QString basePath = ApplicationConfig::getDataPath();
        QString dataPath = QDir(basePath).absoluteFilePath(ApplicationConfig::ResultsPath);
        return dataPath;
    }

    static const QString SETTINGS_VERSION_KEY = "version";
    static const QString SETTINGS_VERSION_VALUE = "35";

    static const int RecordingFrequency =  8000;
    static const int RecordingBitsPerSample =  16;
    static const int RecordingChannelsCount =  1;
    static const QString RecordingAudioFormat = "audio/pcm";
    static const QString RecordingFileFormat = ".wav";
    static const QString RecordingContainerFormat = "audio/x-wav";
    static const QString RecordingFileNameTemplate = "dd.MM.yyyy.hh.mm.ss.zzz";

    static const QString SETTINGS_NAME = "name";
    static const QString SETTINGS_DESC = "description";
    static const QString SETTINGS_VAL = "value";
    static const QString SETTINGS_TYPE = "type";
    static const QString SETTINGS_VISIBLE = "visible";
    static const QString SETTINGS_EDITABLE = "editable";

    static const QString SETTINGS_TYPE_INTEGER = "int";
    static const QString SETTINGS_TYPE_STRING = "str";
    static const QString SETTINGS_TYPE_DOUBLE = "double";
    static const QString SETTINGS_TYPE_BOOL = "bool";
    static const QString SETTINGS_TYPE_LIST_DELIMETER = ";";
    static const QString SETTINGS_TYPE_DOUBLE_LIST = "double_list";

    static const QString SETTINGS_PITCH_ALGORITHM = "PitchAlgorithm";
    static const QString SETTINGS_PITCH_OUTPUT_FORMAT = "PitchOutputFormat";
    static const QString SETTINGS_PITCH_FRAME_SHIFT = "PitchFrameShift";
    static const QString SETTINGS_PITCH_RAPT_THRESHOLD = "PitchRaptThrehold";
    static const QString SETTINGS_PITCH_SWIPE_THRESHOLD = "PitchSwipeThrehold";
    static const QString SETTINGS_PITCH_SAMPLING_FREQUENCY = "PitchSamplingFrequency";
    static const QString SETTINGS_PITCH_MINIMUM_F0 = "PitchMinimumF0";
    static const QString SETTINGS_PITCH_MAXIMUM_F0 = "PitchMaximumF0";
    static const QString SETTINGS_PITCH_NORMALIZED = "PitchNormalized";
    static const QString SETTINGS_PITCH_OCTAVES = "PitchOctavesSeries";
    static const QString SETTINGS_PITCH_NORMALIZE_BY_OCTAVES = "PitchNormalizeOctavesSeries";

    static const QString SETTINGS_HISTOGRAM_MEAN = "OctavesHistagramMean";
    static const QString SETTINGS_HISTOGRAM_THRESHOLD = "HistogramsThreshold";
    static const QString SETTINGS_HISTOGRAM_F0MIN = "OctavesHistagramF0Min";
    static const QString SETTINGS_HISTOGRAM_F0MAX = "OctavesHistagramF0Max";
    static const QString SETTINGS_HISTOGRAM_F0POWK = "OctavesHistagramF0PowK";
    static const QString SETTINGS_HISTOGRAM_DF0 = "OctavesHistagramDF0";
    static const QString SETTINGS_HISTOGRAM_OCTAVES_ENABLED = "OctavesHistagramOctavesEnabled";
    static const QString SETTINGS_HISTOGRAM_COMPONENTS_NUMBER = "HistogramComponentsNumber";

    static const QString SETTINGS_SEGMENTS_FRAME = "SegmentsFrame";
    static const QString SETTINGS_SEGMENTS_SHIFT = "SegmentsShift";
    static const QString SETTINGS_SEGMENTS_SMOOTH_FRAME = "SegmentsSmoothFrame";
    static const QString SETTINGS_SEGMENTS_SEGMENTS_LENGTH_LIMIT = "SegmentsLengthLimit";

    static const QString SETTINGS_SPECTRUM_FRAME_LENGTH = "SpectrumFrameLength";
    static const QString SETTINGS_SPECTRUM_FFT_LENGTH = "SpectrumFftLength";
    static const QString SETTINGS_SPECTRUM_OUTPUT_FORMAT = "SpectrumOutputFormat";

    static const QString SETTINGS_MALE_DC = "MaleDC";
    static const QString SETTINGS_MALE_DB = "MaleDB";
    static const QString SETTINGS_FEMALE_DC = "FemaleDC";
    static const QString SETTINGS_FEMALE_DB = "FemaleDB";

    static const QString SETTINGS_PITCH_VARIABILITY_MIN = "PitchVariabilityMin";
    static const QString SETTINGS_PITCH_VARIABILITY_MAX = "PitchVariabilityMax";
    static const QString SETTINGS_SPEECH_RATE_MIN = "SpeechRateMin";
    static const QString SETTINGS_SPEECH_RATE_MAX = "SpeechRateMax";
    static const QString SETTINGS_PAUSES_DURATION_MIN = "PausesDurationMin";
    static const QString SETTINGS_PAUSES_DURATION_MAX = "PausesDurationMax";

    static const QString SETTINGS_ARTICULATION_SPEECH_K1 = "ArticulationSpeechK1";
    static const QString SETTINGS_ARTICULATION_K2 = "ArticulationK2";
    static const QString SETTINGS_ARTICULATION_PAUSES_K3 = "ArticulationPausesK3";
    static const QString SETTINGS_ARTICULATION_PAUSES_MEAN_DEGRY = "ArticulationPausesMeanDegry";

    static const QString SETTINGS_MONITORING_POINTS_MAX_COUNT = "MonitoringPointsMaxCount";
    static const QString SETTINGS_MONITORING_RECORD_DURATION = "MonitoringRecordDuration";
    static const QString SETTINGS_MONITORING_MEDIAN_SMOOTHING = "MonitoringRecordMedianSmoothing";

    static const QString SETTINGS_TRUE = "true";
    static const QString SETTINGS_FALSE = "false";

    const static std::map<QString, std::map<QString, QString> > DEFAULT_SETTINGS = {
        {SETTINGS_SEGMENTS_FRAME,{
             {SETTINGS_NAME, "Intensity Frame"},
             {SETTINGS_DESC, "Intensity frame length used to calculate segments"},
             {SETTINGS_VAL, "240"},
             {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_SEGMENTS_SHIFT,{
             {SETTINGS_NAME, "Intensity Shift"},
             {SETTINGS_DESC, "Intensity frame shift used to calculate segments"},
             {SETTINGS_VAL, "120"},
             {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_SEGMENTS_SMOOTH_FRAME,{
             {SETTINGS_NAME, "Intensity Smooth Frame"},
             {SETTINGS_DESC, "Frame length used for smoothing intensity to segments calculation"},
             {SETTINGS_VAL, "120"},
             {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_SEGMENTS_SEGMENTS_LENGTH_LIMIT,{
             {SETTINGS_NAME, "Segments Length Limit"},
             {SETTINGS_DESC, "Segments length limit in millisec"},
             {SETTINGS_VAL, "15"},
             {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},

        {SETTINGS_PITCH_ALGORITHM,{
             {SETTINGS_NAME, "Pitch Algorithm"},
             {SETTINGS_DESC, "Algorithm used for extraction of pitch. \n[0: RAPT, 1: SWIPE, 2: Reaper, 3: World, 4: NumAlgorithms]"},
             {SETTINGS_VAL, "0"},
             {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_PITCH_OUTPUT_FORMAT,{
             {SETTINGS_NAME, "Pitch Output format"},
             {SETTINGS_DESC, "Output format. \n[0:pitch, 1:F0, 2:log(F0), 3: NumOutputFormats]"},
             {SETTINGS_VAL, "1"},
             {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_PITCH_FRAME_SHIFT,{
             {SETTINGS_NAME, "Pitch Frame shift"},
             {SETTINGS_DESC, "Frame shift"},
             {SETTINGS_VAL, "60"},
             {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_PITCH_RAPT_THRESHOLD,{
             {SETTINGS_NAME, "Pitch RAPT threhold"},
             {SETTINGS_DESC, "Voiced/unvoiced threhold (used only for RAPT algorithm)"},
             {SETTINGS_VAL, "0"},
             {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_PITCH_SWIPE_THRESHOLD,{
             {SETTINGS_NAME, "Pitch SWIPE threhold"},
             {SETTINGS_DESC, "Voiced/unvoiced threhold (used only for SWIPE algorithm)"},
             {SETTINGS_VAL, "0"},
             {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_PITCH_SAMPLING_FREQUENCY,{
             {SETTINGS_NAME, "Pitch Sampling frequency"},
             {SETTINGS_DESC, "Sampling frequency (kHz)"},
             {SETTINGS_VAL, "8.0"},
             {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_PITCH_MINIMUM_F0,{
             {SETTINGS_NAME, "Pitch Minimum F0"},
             {SETTINGS_DESC, "Minimum fundamental frequency to search for (Hz)"},
             {SETTINGS_VAL, "60.0"},
             {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_PITCH_MAXIMUM_F0,{
             {SETTINGS_NAME, "Pitch Maximum F0"},
             {SETTINGS_DESC, "Maximum fundamental frequency to search for (Hz)"},
             {SETTINGS_VAL, "500.0"},
             {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_PITCH_NORMALIZED,{
             {SETTINGS_NAME, "Pitch Normalization"},
             {SETTINGS_DESC, "Normalize Pitch data"},
             {SETTINGS_VAL, "true"},
             {SETTINGS_TYPE, SETTINGS_TYPE_BOOL},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_PITCH_NORMALIZE_BY_OCTAVES,{
             {SETTINGS_NAME, "Pitch Normalization By Octaves"},
             {SETTINGS_DESC, "Normalize Pitch data by ocataves"},
             {SETTINGS_VAL, "false"},
             {SETTINGS_TYPE, SETTINGS_TYPE_BOOL},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},

        {SETTINGS_PITCH_OCTAVES,{
             {SETTINGS_NAME, "Pitch Octaves Series"},
             {SETTINGS_DESC, "Pitch frequency segments"},
             {SETTINGS_VAL, "60;70;80;90;100;110;120;130;140;150;160;"
              "170;180;190;200;210;220;230;240;250;260;270;280;290;300;"
              "310;320;330;340;350;360;370;380;390;400"},
             {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE_LIST},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},

        {SETTINGS_HISTOGRAM_THRESHOLD,{
             {SETTINGS_NAME, "Histograms Threshold"},
             {SETTINGS_DESC, "(0,00 – 1,00)"},
             {SETTINGS_VAL, "0.0"},
             {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_HISTOGRAM_MEAN,{
            {SETTINGS_NAME, "Histagram Mean"},
            {SETTINGS_DESC, "Apply mean values for octaves histogram"},
            {SETTINGS_VAL, "false"},
            {SETTINGS_TYPE, SETTINGS_TYPE_BOOL},
            {SETTINGS_VISIBLE, SETTINGS_FALSE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_HISTOGRAM_F0MIN,{
            {SETTINGS_NAME, "Histogram Min Value - F0min"},
            {SETTINGS_DESC, "Start histogram components Hz"},
            {SETTINGS_VAL, "1"},
            {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
            {SETTINGS_VISIBLE, SETTINGS_TRUE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_HISTOGRAM_F0MAX,{
            {SETTINGS_NAME, "Histogram Max Value - F0max"},
            {SETTINGS_DESC, "End histogram components Hz"},
            {SETTINGS_VAL, "500"},
            {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
            {SETTINGS_VISIBLE, SETTINGS_FALSE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_HISTOGRAM_F0POWK,{
            {SETTINGS_NAME, "Histogram Segments Level"},
            {SETTINGS_DESC, "Applied to Left and Right Segments"},
            {SETTINGS_VAL, "2"},
            {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
            {SETTINGS_VISIBLE, SETTINGS_FALSE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_HISTOGRAM_DF0,{
            {SETTINGS_NAME, "Histogram Scale Step - dF0"},
            {SETTINGS_DESC, "Histogram components indent Hz"},
            {SETTINGS_VAL, "1"},
            {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
            {SETTINGS_VISIBLE, SETTINGS_FALSE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_HISTOGRAM_OCTAVES_ENABLED,{
            {SETTINGS_NAME, "Histogram Octaves Series Enabled"},
            {SETTINGS_DESC, "Use manual Octaves Series"},
            {SETTINGS_VAL, "false"},
            {SETTINGS_TYPE, SETTINGS_TYPE_BOOL},
            {SETTINGS_VISIBLE, SETTINGS_FALSE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_HISTOGRAM_COMPONENTS_NUMBER,{
            {SETTINGS_NAME, "Histogram Components Number"},
            {SETTINGS_DESC, "(1 – 1000)"},
            {SETTINGS_VAL, "500"},
            {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
            {SETTINGS_VISIBLE, SETTINGS_FALSE},
            {SETTINGS_EDITABLE, SETTINGS_FALSE}
        }},

        {SETTINGS_SPECTRUM_FRAME_LENGTH,{
             {SETTINGS_NAME, "Spectrum frame length"},
             {SETTINGS_DESC, "Shoul be less than Spectrum FFT Length"},
             {SETTINGS_VAL, "256"},
             {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_SPECTRUM_FFT_LENGTH,{
             {SETTINGS_NAME, "Spectrum FFT length"},
             {SETTINGS_DESC, ""},
             {SETTINGS_VAL, "256"},
             {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_SPECTRUM_OUTPUT_FORMAT,{
             {SETTINGS_NAME, "Spectrum output format"},
             {SETTINGS_DESC, "0: Log Amplitude Spectrum In Decibels,\n1: Log Amplitude Spectrum,\n2: Amplitude Spectrum,\n3: Power Spectrum,\n4: Num Input Output Formats"},
             {SETTINGS_VAL, "0"},
             {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
             {SETTINGS_VISIBLE, SETTINGS_FALSE},
             {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_PITCH_VARIABILITY_MIN,{
            {SETTINGS_NAME, "Pitch Variation Minimum"},
            {SETTINGS_DESC, "Minimum Value [Hz]"},
            {SETTINGS_VAL, "5"},
            {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
            {SETTINGS_VISIBLE, SETTINGS_TRUE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_PITCH_VARIABILITY_MAX,{
            {SETTINGS_NAME, "Pitch Variation Maximum"},
            {SETTINGS_DESC, "Maximum Value [Hz]"},
            {SETTINGS_VAL, "105"},
            {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
            {SETTINGS_VISIBLE, SETTINGS_TRUE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_SPEECH_RATE_MIN,{
            {SETTINGS_NAME, "Speech Rate Minimum"},
            {SETTINGS_DESC, "Minimum Value [WpM]"},
            {SETTINGS_VAL, "70"},
            {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
            {SETTINGS_VISIBLE, SETTINGS_TRUE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_SPEECH_RATE_MAX,{
            {SETTINGS_NAME, "Speech Rate Maximum"},
            {SETTINGS_DESC, "Maximum Value [WpM]"},
            {SETTINGS_VAL, "210"},
            {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
            {SETTINGS_VISIBLE, SETTINGS_TRUE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_PAUSES_DURATION_MIN,{
            {SETTINGS_NAME, "Pauses Duration Minimum"},
            {SETTINGS_DESC, "Minimum Value [Sec]"},
            {SETTINGS_VAL, "0.0"},
            {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
            {SETTINGS_VISIBLE, SETTINGS_FALSE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_PAUSES_DURATION_MAX,{
            {SETTINGS_NAME, "Pauses Duration Maximum"},
            {SETTINGS_DESC, "Maximum Value [Sec]"},
            {SETTINGS_VAL, "2.0"},
            {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
            {SETTINGS_VISIBLE, SETTINGS_FALSE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_ARTICULATION_SPEECH_K1,{
            {SETTINGS_NAME, "K1 - Speech Rate"},
            {SETTINGS_DESC, "K1"},
            {SETTINGS_VAL, "0.71"},
            {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
            {SETTINGS_VISIBLE, SETTINGS_FALSE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_ARTICULATION_K2,{
            {SETTINGS_NAME, "K2 - Articulation Rate"},
            {SETTINGS_DESC, "K2"},
            {SETTINGS_VAL, "1.20"},
            {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
            {SETTINGS_VISIBLE, SETTINGS_FALSE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_ARTICULATION_PAUSES_K3,{
            {SETTINGS_NAME, "K3 - Phrase Pauses"},
            {SETTINGS_DESC, "K3"},
            {SETTINGS_VAL, "0.30"},
            {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
            {SETTINGS_VISIBLE, SETTINGS_FALSE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_ARTICULATION_PAUSES_MEAN_DEGRY,{
            {SETTINGS_NAME, "K3 - Mean value degry"},
            {SETTINGS_DESC, "K3"},
            {SETTINGS_VAL, "3.0"},
            {SETTINGS_TYPE, SETTINGS_TYPE_DOUBLE},
            {SETTINGS_VISIBLE, SETTINGS_FALSE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_MONITORING_POINTS_MAX_COUNT,{
            {SETTINGS_NAME, "Visible monitoring points number"},
            {SETTINGS_DESC, ""},
            {SETTINGS_VAL, "30"},
            {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
            {SETTINGS_VISIBLE, SETTINGS_TRUE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_MONITORING_RECORD_DURATION,{
            {SETTINGS_NAME, "Sliding monitoring window in Sec"},
            {SETTINGS_DESC, "sec"},
            {SETTINGS_VAL, "10"},
            {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
            {SETTINGS_VISIBLE, SETTINGS_TRUE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }},
        {SETTINGS_MONITORING_MEDIAN_SMOOTHING,{
            {SETTINGS_NAME, "Monitoring results median smoothing"},
            {SETTINGS_DESC, "Recommended values: 1, 3, 5"},
            {SETTINGS_VAL, "1"},
            {SETTINGS_TYPE, SETTINGS_TYPE_INTEGER},
            {SETTINGS_VISIBLE, SETTINGS_TRUE},
            {SETTINGS_EDITABLE, SETTINGS_TRUE}
        }}
    };

    static QString SettingsKey(QString group, QString key){
        return group + "/" + key;
    }

    static QString SettingsNameKey(QString group){
        return SettingsKey(group, SETTINGS_NAME);
    }

    static QString SettingsDescKey(QString group){
        return SettingsKey(group, SETTINGS_DESC);
    }

    static QString SettingsValueKey(QString group){
        return SettingsKey(group, SETTINGS_VAL);
    }

    static QString SettingsTypeKey(QString group){
        return SettingsKey(group, SETTINGS_TYPE);
    }

    static QString SettingsVisibleKey(QString group){
        return SettingsKey(group, SETTINGS_VISIBLE);
    }

    static QString SettingsEditableKey(QString group){
        return SettingsKey(group, SETTINGS_EDITABLE);
    }
}

#endif // APPLICATIONCONFIG_H
