import QtQuick 2.14
import QtQuick.Controls 2.14

import "../../App"

MonitorForm {
    id: root
    objectName: Enum.pageMonitor
    property bool startRecording: false

    property bool isRecording: false

    property var valuesSE: []
    property var valuesSR: []

    property double minSE: 0
    property double minSR: 0
    property double maxSE: 0
    property double maxSR: 0

    property double valuesMaxCount: 20
    property int valuesSmoothing: 1
    property var recordingStartTime: 0

    Timer {
        id: animationTimer
        interval: 100;
        repeat: true
        onTriggered: {
            root.durationLabel.opacity -= 0.05
            if (root.durationLabel.opacity <= 0.5) {
                root.durationLabel.opacity = 1
            }
            let time = new Date()
            root.durationValue.text = ((time - root.recordingStartTime)/1000).toFixed(1)
        }
    }

    StackView.onActivated: {
        console.log("MonitorForm.StackView.onActivated");
        Bus.hideAllBottomActions();

        Bus.showRecordButton = true;
//        Bus.showOpenButton = true

        root.valuesSE  = [];
        root.valuesSR  = [];

        root.minSE  = Bus.getSettingValue('PitchVariabilityMin');
        root.minSR  = Bus.getSettingValue('SpeechRateMin');
        root.maxSE  = Bus.getSettingValue('PitchVariabilityMax');
        root.maxSR  = Bus.getSettingValue('SpeechRateMax');

        root.valuesMaxCount = Bus.getSettingValue('MonitoringPointsMaxCount');
        root.valuesSmoothing = Bus.getSettingValue('MonitoringRecordMedianSmoothing');

        root.metricSE.valuesMaxCount = root.valuesMaxCount;
        root.metricSR.valuesMaxCount = root.valuesMaxCount;

        draw()
    }

    Connections {
        target: Bus
        function onNewRecordPath() {
            if (!Bus.isMonitorPage()) return;
            let length = Bus.getLength();
            console.log("MonitorForm:onNewRecordPath length", length);
            if (!root.isRecording) {
                setHeaderResults();
                root.durationValue.text = length.toFixed(1)
            }
            console.log("MonitorForm:onNewRecordPath");            
            push(root.valuesSE, root.getMetricSE());
            push(root.valuesSR, root.getMetricSR());
            draw();
        }
    }

    Connections {
        target: Bus
        function onCustomEvent(event) {
            console.log("MonitorForm:onCustomEvent", event);
            if (event === "RecordButton::stopRecording") {
                setHeaderResults();
                animationTimer.stop()
                root.durationLabel.opacity = 1
                Bus.showSaveResultsButton = true;
                Bus.showPlayButton = true;
                root.isRecording = false;
            } else if (event === "RecordButton::startRecord") {
                root.recordingStartTime = new Date()
                animationTimer.start()
                Bus.showSaveResultsButton = false;
                Bus.showPlayButton = false;
                root.isRecording = true;
            }
        }
    }

    function setHeaderResults() {
        let midSE = mid(root.valuesSE).toFixed(0);
        let rmsSE = rmsd(root.valuesSE, midSE).toFixed(0);
        root.headerSE.mean = midSE;
        root.headerSE.rms = rmsSE;
        Bus.setResultItem("3", "Average pitch Variability [Hz]: " + midSE);
        Bus.setResultItem("4", "Root-mean-square deviation Variability [Hz]: " + midSE);
        let midSR = mid(root.valuesSR).toFixed(0);
        let rmsSR = rmsd(root.valuesSR, midSR).toFixed(0);
        root.headerSR.mean = midSR;
        root.headerSR.rms = rmsSR;
        Bus.setResultItem("5", "Average articulation Rate [wpm]: " + midSR);
        Bus.setResultItem("6", "Root-mean-square deviation articulation Rate [wpm]: " + midSR);
    }

    function mid(values) {
        const sum = values.reduce((partialSum, a) => partialSum + a, 0);
        return sum / values.length
    }

    function rmsd(values, value) {
        const sum = values.reduce((partialSum, a) => partialSum + Math.pow(a - value, 2), 0);
        return Math.sqrt(sum / values.length)
    }

    function push(values, value) {
        values.push(value);
        if (values.length > root.valuesMaxCount) values.shift();
    }

    function median(arr) {
      const arrSorted = arr.sort((a, b) => a - b);
      return arrSorted.length % 2 === 0 ? (arrSorted[arrSorted.length/2 - 1] + arrSorted[arrSorted.length/2]) / 2 : arrSorted[Math.floor(arrSorted.length/2)];
    }

    function smoothing(values) {
        if (root.valuesSmoothing == 1) return values;
        if (values.length < root.valuesSmoothing) return [];
        let smoothed = [];
        for (let i=0; i<=(values.length-root.valuesSmoothing); i++) {
            smoothed.push(median(values.slice(i, i+root.valuesSmoothing)));
        }
        return smoothed;
    }

    function draw() {
        setMetric(root.metricSE,  smoothing(root.valuesSE),  minSE,  maxSE);
        setMetric(root.metricSR,  smoothing(root.valuesSR),  minSR,  maxSR);
    }

    function getMetricSE() {
        return getMetric(Bus.getMetricSE(false),  minSE,  maxSE);
    }

    function getMetricSR() {
        return getMetric(Bus.getMetricSR(false),  minSR,  maxSR);
    }

    function getMetric(value, min, max) {
        if (value < min) return min;
        else if (value > max) return max;
        else return value;
    }

    function setMetric(metric, values, valueMin, valueMax) {
        console.log(metric, values, valueMin, valueMax);
        metric.values   = values;
        metric.valueMin = valueMin;
        metric.valueMax = valueMax;
    }
}
