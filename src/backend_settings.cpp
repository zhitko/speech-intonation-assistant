#include "backend.h"

#include <QDebug>
#include <QSettings>

#include "applicationconfig.h"
#include "qml/qmlsetting.h"

QVariant Backend::getSettingValue(QString id)
{
    QSettings settings(ApplicationConfig::GetFullSettingsPath(), QSettings::IniFormat);
    return settings.value(ApplicationConfig::SettingsValueKey(id));
}

QVariantList Backend::getSettings()
{
    QSettings settings(ApplicationConfig::GetFullSettingsPath(), QSettings::IniFormat);
    auto groups = settings.childGroups();

    auto version = settings.value(ApplicationConfig::SETTINGS_VERSION_KEY, "").toString() == ApplicationConfig::SETTINGS_VERSION_VALUE;

    QVariantList settingsList;
    for (const auto& kv : ApplicationConfig::DEFAULT_SETTINGS) {
        if (!groups.contains(kv.first) || !version)
        {
            qDebug() << "getSettings: missing setting = " << kv;
            settings.setValue(
                ApplicationConfig::SettingsValueKey(kv.first),
                kv.second.at(ApplicationConfig::SETTINGS_VAL)
            );
            settings.setValue(
                ApplicationConfig::SettingsNameKey(kv.first),
                kv.second.at(ApplicationConfig::SETTINGS_NAME)
            );
            settings.setValue(
                ApplicationConfig::SettingsDescKey(kv.first),
                kv.second.at(ApplicationConfig::SETTINGS_DESC)
            );
            settings.setValue(
                ApplicationConfig::SettingsTypeKey(kv.first),
                kv.second.at(ApplicationConfig::SETTINGS_TYPE)
            );
            settings.setValue(
                ApplicationConfig::SettingsVisibleKey(kv.first),
                kv.second.at(ApplicationConfig::SETTINGS_VISIBLE)
            );
            settings.setValue(
                ApplicationConfig::SettingsEditableKey(kv.first),
                kv.second.at(ApplicationConfig::SETTINGS_EDITABLE)
            );
        }

        qDebug() << kv.first << " " << settings.value(kv.first + "/" + ApplicationConfig::SETTINGS_EDITABLE).toString();
        settingsList.append(QVariant::fromValue(QmlSetting(
            kv.first,
            settings.value(ApplicationConfig::SettingsTypeKey(kv.first)).toString(),
            settings.value(ApplicationConfig::SettingsNameKey(kv.first)).toString(),
            settings.value(ApplicationConfig::SettingsValueKey(kv.first)).toString(),
            settings.value(ApplicationConfig::SettingsDescKey(kv.first)).toString(),
            settings.value(ApplicationConfig::SettingsVisibleKey(kv.first)).toBool(),
            settings.value(ApplicationConfig::SettingsEditableKey(kv.first)).toBool()
        )));
    }

    settings.setValue(ApplicationConfig::SETTINGS_VERSION_KEY, ApplicationConfig::SETTINGS_VERSION_VALUE);
    settings.sync();

    return settingsList;
}

void pitchOctavesSeriesHook(QSettings* settings)
{
    auto min = settings->value(
                           ApplicationConfig::SettingsValueKey(ApplicationConfig::SETTINGS_HISTOGRAM_F0MIN)
                           ).toInt();
    auto max = settings->value(
                           ApplicationConfig::SettingsValueKey(ApplicationConfig::SETTINGS_HISTOGRAM_F0MAX)
                           ).toInt();
    auto d = settings->value(
                         ApplicationConfig::SettingsValueKey(ApplicationConfig::SETTINGS_HISTOGRAM_DF0)
                         ).toInt();

    int count = (max-min)/d;

    settings->setValue(ApplicationConfig::SettingsValueKey(
                           ApplicationConfig::SETTINGS_HISTOGRAM_COMPONENTS_NUMBER
                           ), count + 1);

    QString value = QString::number(min);
    qDebug() << "pitchOctavesSeriesHook min: " << min;
    qDebug() << "pitchOctavesSeriesHook max: " << max;
    qDebug() << "pitchOctavesSeriesHook d: " << d;

    for (int i=2; i<=count; i++) {
        value += QString(";%1").arg(1.0 * i * d + min);
    }

    settings->setValue(ApplicationConfig::SettingsValueKey(
                           ApplicationConfig::SETTINGS_PITCH_OCTAVES
                           ), value);
}

QString Backend::setSettings(QString id, QString value)
{
    qDebug() << "setSettings: " << id << " " << value;

    QSettings settings(ApplicationConfig::GetFullSettingsPath(), QSettings::IniFormat);

    if (!this->validateSettings(id, value))
    {
        qDebug() << "validateSettings: failed";
        return settings.value(ApplicationConfig::SettingsValueKey(id)).toString();
    }

    if (value.isEmpty()) return value;

    settings.setValue(ApplicationConfig::SettingsValueKey(id), value);

    // ----------------------
    // SETTINGS_PITCH_OCTAVES

    auto settingsPitchOctavesEnabled = settings.value(
                                                   ApplicationConfig::SettingsValueKey(ApplicationConfig::SETTINGS_HISTOGRAM_OCTAVES_ENABLED)
                                                   ).toString();
    qDebug() << "setSettings: PITCH_OCTAVES_ENABLED " << settingsPitchOctavesEnabled;

    if (settingsPitchOctavesEnabled == ApplicationConfig::SETTINGS_FALSE)
    {
        if (id == ApplicationConfig::SETTINGS_HISTOGRAM_F0MIN
            || id == ApplicationConfig::SETTINGS_HISTOGRAM_F0MAX
            || id == ApplicationConfig::SETTINGS_HISTOGRAM_DF0)
        {
            pitchOctavesSeriesHook(&settings);
        }

        if (id == ApplicationConfig::SETTINGS_HISTOGRAM_OCTAVES_ENABLED
            && value == ApplicationConfig::SETTINGS_FALSE)
        {
            pitchOctavesSeriesHook(&settings);
        }
    }

    // SETTINGS_PITCH_OCTAVES
    // ----------------------

    settings.sync();

    return value;
}

bool Backend::validateSettings(QString id, QString value)
{
    qDebug() << "validateSettings: " << id << " " << value;

    if (value.isEmpty()) return true;

    QSettings settings(ApplicationConfig::GetFullSettingsPath(), QSettings::IniFormat);

    auto type =settings.value(ApplicationConfig::SettingsTypeKey(id)).toString();

    bool result = true;
    if (type == ApplicationConfig::SETTINGS_TYPE_DOUBLE)
    {
        value.toDouble(&result);
    } else if (type == ApplicationConfig::SETTINGS_TYPE_INTEGER)
    {
        value.toInt(&result);
    } else if (type == ApplicationConfig::SETTINGS_TYPE_BOOL) {
        result = value == "true" || value == "false";
    } else if (type == ApplicationConfig::SETTINGS_TYPE_DOUBLE_LIST) {
        auto values = value.split(ApplicationConfig::SETTINGS_TYPE_LIST_DELIMETER);
        for (auto& v : values)
        {
            v.toDouble(&result);
            if (!result) return result;
        }
    }

    return result;
}

std::list<double> Backend::getOctavesSetting(bool isFull)
{
    std::list<double> octaves;

    auto actaves = this->getSettingValue(ApplicationConfig::SETTINGS_PITCH_OCTAVES);
    auto actavesStr = actaves.toString().split(ApplicationConfig::SETTINGS_TYPE_LIST_DELIMETER);

    qDebug() << "actavesStr" << actavesStr;

    for ( const auto& i : actavesStr  )
    {
        octaves.push_back(i.toDouble());
    }

    return octaves;
}
