import QtQuick 2.14
import QtQuick.Controls 2.14

import "../../App"

AnalyserForm {
    id: root
    objectName: Enum.pageAnalyser
    property bool startRecording: false

    property var startRecordingTime: 0

    StackView.onActivated: {
        console.log("AnalyserForm.StackView.onActivated")
        Bus.hideAllBottomActions()

        Bus.showPlayButton = true
        Bus.showOpenButton = true
        Bus.showRecordOneButton = true
        Bus.showSaveResultsButton = true

        setMetric(root.metricSE, root.getMetricSE(), Bus.getSettingValue('PitchVariabilityMin'), Bus.getSettingValue('PitchVariabilityMax'))
        setMetric(root.metricSR, root.getMetricSR(), Bus.getSettingValue('SpeechRateMin'), Bus.getSettingValue('SpeechRateMax'))

        root.durationValue.text = Bus.getLength().toFixed(1)
    }

    Connections {
        target: Bus
        function onNewRecordPath() {
            if (!Bus.isAnalyzerPage()) return;
            console.log("AnalyserForm:onNewRecordPath");
            setMetric(root.metricSE, root.getMetricSE(), Bus.getSettingValue('PitchVariabilityMin'), Bus.getSettingValue('PitchVariabilityMax'))
            setMetric(root.metricSR, root.getMetricSR(), Bus.getSettingValue('SpeechRateMin'), Bus.getSettingValue('SpeechRateMax'))

            root.durationValue.text = Bus.getLength().toFixed(1)
        }
        function onCustomEvent(event) {
            console.log("AnalyserForm:onCustomEvent", event);
            if (event === "AnimatedRecordButton:startRecording") {
                console.log("AnalyserForm:startRecording")
            } else if (event === "AnimatedRecordButton:stopRecording") {
                console.log("AnalyserForm:stopRecording")
            }
        }
    }

    function getMetricSE() {
        const value = Bus.getMetricSE(false)
        Bus.setResultItem("3", "Pitch Variability [Hz]: " + value.toFixed(0))
        return value
    }

    function getMetricSR() {
        const value = Bus.getMetricSR(false)
        Bus.setResultItem("4", "Articulation Rate [wpm]: " + value.toFixed(0))
        return value
    }

    function setMetric(metric, value, valueMin, valueMax) {
        console.log(metric, value, valueMin, valueMax)
        metric.value = value;
        metric.valueMin = valueMin;
        metric.valueMax = valueMax;
    }
}
