import QtQuick 2.14
import QtQuick.Controls 2.14

import "../../App"

TrainingForm {
    id: root
    objectName: Enum.pageTraining
    property bool startRecording: false

    property var startRecordingTime: 0

    property double minSR: 0
    property double maxSR: 100
    property double templateSR: 0
    property double minSE: 0
    property double maxSE: 100
    property double templateSE: 0
    property var values: []
    property var value: []

    property bool isTemplateReady: false
    property string recordPath: ''

    StackView.onActivated: {
        console.log("TrainingForm.StackView.onActivated");
        Bus.hideAllBottomActions();

        Bus.showPlayButton = true;
        Bus.showOpenButton = true;
        Bus.showOpenTemplateButton = true;
        Bus.showRecordOneButton = true;
        Bus.showSaveResultsButton = true;

        root.metrics.valueXMin = root.minSR = Bus.getSettingValue('SpeechRateMin');
        root.metrics.valueXMax = root.maxSR = Bus.getSettingValue('SpeechRateMax');
        root.metrics.valueYMin = root.minSE = Bus.getSettingValue('PitchVariabilityMin');
        root.metrics.valueYMax = root.maxSE = Bus.getSettingValue('PitchVariabilityMax');

        if (Bus.recordPath != root.recordPath) {
            root.recordPath = Bus.recordPath;
            processRecord();
        }
    }

    Connections {
        target: Bus
        function onNewRecordPath() {
            if (!Bus.isTrainingPage()) return;
            processRecord()
        }
        function onNewTemplatePath() {
            if (!Bus.isTrainingPage()) return;
            processTemplate()
        }
    }

    function processTemplate() {
        root.isTemplateReady = true;
        console.log("TrainingForm:onNewTemplatePath");
        root.templateSE = getMetricSE(true);
        console.log("TrainingForm:onNewTemplatePath templateSE:", templateSE);
        root.templateSR = getMetricSR(true);
        console.log("TrainingForm:onNewTemplatePath templateSR:", templateSR);

        root.metrics.value = [root.templateSR, root.templateSE];

        root.sampleDurationValue.text = Bus.getLength(true).toFixed(1)
    }

    function processRecord() {
        console.log("TrainingForm:onNewRecordPath");
        const valueSE = getMetricSE(false);
        console.log("TrainingForm:onNewRecordPath valueSE:", valueSE);
        const valueSR = getMetricSR(false);
        console.log("TrainingForm:onNewRecordPath valueSR:", valueSR);

        root.value = [valueSR, valueSE];
        root.values.push(value);
        root.metrics.values = root.values;

        root.spokenDurationValue.text = Bus.getLength(false).toFixed(1);

        calculateScores();
    }

    function calculateScores() {
        if (!root.isTemplateReady) return;

        let path = Bus.templatePath.split('data');

        Bus.setResultItem("1", "Sample for training:  - data" + path[path.length-1])

        const xScore = calculateScore(root.value[0], root.templateSR, root.minSR, root.maxSR);
        const yScore = calculateScore(root.value[1], root.templateSE, root.minSE, root.maxSE);
        const xScore10 = 10 * (1 - xScore);
        const yScore10 = 10 * (1 - yScore);
        const xyScore10 = 10 * (1 - (xScore+yScore)/2);
        root.xScoreLabel.text = xScore10.toFixed(0);
        Bus.setResultItem("2", "Speech Rate: X-score - " + xScore10.toFixed(0))
        root.yScoreLabel.text = yScore10.toFixed(0);
        Bus.setResultItem("3", "Pitch Variability: Y-score - " + yScore10.toFixed(0))
        root.xyScoreLabel.text = xyScore10.toFixed(0);
        Bus.setResultItem("4", "General: XY-score - " + yScore10.toFixed(0))
    }

    function calculateScore(record, template, min, max) {
        const rNorm = normValue(record, min, max);
        const tNorm = normValue(template, min, max);
        return Math.abs(rNorm - tNorm);
    }

    function normValue(value, min, max) {
        if (value <= min) return 0;
        if (value >= max) return 1;
        return (value-min)/(max-min);

    }

    function getMetricSE(isTemplate) {        
        const value = Bus.getMetricSE(isTemplate);
        console.log("getMetricSE isTemplate", isTemplate);
        console.log("getMetricSE value", value);
        console.log("getMetricSE minSE", minSE);
        console.log("getMetricSE maxSE", maxSE);
        if (value < root.minSE) return minSE;
        if (value > root.maxSE) return maxSE;
        return value;
    }

    function getMetricSR(isTemplate) {
        const value = Bus.getMetricSR(isTemplate);
        console.log("getMetricSR isTemplate", isTemplate);
        console.log("getMetricSR value", value);
        console.log("getMetricSR minSR", minSR);
        console.log("getMetricSR maxSR", maxSR);
        if (value < root.minSR) return minSR;
        if (value > root.maxSR) return maxSR;
        return value;
    }
}
