pragma Singleton
import QtQuick 2.14

import intondemo.backend 1.0

Item {
    id: root

    property var stackView
    property var settings

    property bool showRecordButton: false
    property bool showRecordOneButton: false
    property bool canRecordButton: true
    property bool showPlayButton: false
    property bool showSaveResultsButton: false
    property bool canPlayButton: true
    property bool showOpenButton: false
    property bool showOpenTemplateButton: false
    property bool canOpenButton: true
    property bool canOpenTemplateButton: true

    property string currentPage: ""

    property string recordPath: ""
    property string templatePath: ""
    property var results: ({})

    signal newRecordPath(path: string)
    signal newTemplatePath(path: string)
    signal customEvent(event: string)

    Backend {
        id: backend
    }

    function getSettings() {
        console.log("Action.getSettings")
        return backend.getSettings()
    }

    function getSettingValue(id) {
        console.log("Action.getSettingValue")
        return backend.getSettingValue(id)
    }

    function getSettingsMap() {
        console.log("Action.getSettingsMap")
        let settings = getSettings()
        let settingsMap = {}

        settings.forEach(setting => {
            settingsMap[setting.key] = setting.value
        });

        return settingsMap
    }

    function setSettings(name, value) {
        console.log("Action.setSettings", name, value)
        return backend.setSettings(name, value)
    }

    function getSavedResults() {
        console.log("Action.getSavedResults")
        return backend.getSavedResults()
    }

    function saveResults() {
        console.log("Action.getSavedResults")
        return backend.saveResult(results)
    }

    function setResultItem(key, value) {
        console.log("Action.addResult", key, value)
        results[key] = value
    }

    function getWaveFilesList() {
        console.log("Action.getWaveFilesList")
        return backend.getWaveFilesList()
    }

    function copyToFiles(filePath, name) {
        console.log("Action.copyToFiles: ", filePath, name)
        return backend.copyToFiles(filePath, name)
    }

    /*---------------------------------------------------------
      Actions
      ---------------------------------------------------------*/

    function cleanRecord() {
        root.recordPath = ""
    }

    function setRecordPath(path) {
        console.log("Bus.setRecordPath", path)
        root.recordPath = path
        console.log("Bus.setRecordPath emit newRecordPath")
        newRecordPath(path)
    }

    function cleanTemplate() {
        root.templatePath = ""
    }

    function setTemplatePath(path) {
        console.log("Bus.setTemplatePath", path)
        root.templatePath = path
        console.log("Bus.setTemplatePath emit newRecordPath")
        newTemplatePath(path)
    }

    function startRecord(isTemplate = false, setPath = false) {
        console.log("Bus.startRecord")

        let path = backend.startStopRecordWaveFile(isTemplate, setPath)

        if (setPath) {
            console.log("Bus.startRecord setRecordPath")
            setRecordPath(path)
        }

        root.canPlayButton = false
        root.canOpenTemplateButton = false
        root.canOpenButton = false
        return path
    }

    function stopRecord(isTemplate = false, setPath = false) {
        console.log("Bus.stopRecord")
        let path = backend.startStopRecordWaveFile(isTemplate, setPath)

        if (setPath) {
            console.log("Bus.stopRecord setRecordPath", path)
            setRecordPath(path)
        }

        root.canRecordButton = true
        root.canOpenTemplateButton = true
        root.canPlayButton = true
        root.canOpenButton = true
        return path
    }

    function play(playing, isTemplate) {
        console.log("Action.play : ", playing)
        if (!isTemplate && root.recordPath !== "") {
            console.log("Try play file : ", root.recordPath)
            backend.playWaveFile(root.recordPath, playing)
        } else
        if (isTemplate && root.templatePath !== "") {
            console.log("Try play file : ", root.templatePath)
            backend.playWaveFile(root.templatePath, playing)
        }
    }

    function openFileDialog() {
        console.log("Action.openFileDialog")
        let newPath = backend.openFileDialog()
        console.log("Action.openFileDialog: newPath = ", newPath)
        let isChanged = root.recordPath !== newPath
        console.log("Action.openFileDialog: isChanged = ", isChanged)
        if (isChanged) setRecordPath(newPath)
    }

    function openTemplateFileDialog() {
        console.log("Action.openTemplateFileDialog")
        let newPath = backend.openFileDialog()
        console.log("Action.openTemplateFileDialog: newPath = ", newPath)
        let isChanged = root.templatePath !== newPath
        console.log("Action.openTemplateFileDialog: isChanged = ", isChanged)
        if (isChanged) setTemplatePath(newPath)
    }

    /*---------------------------------------------------------
      Data processing
      ---------------------------------------------------------*/

    function getLength(isTemplate) {
        if (!isTemplate && root.recordPath !== "") {
            return backend.getWaveLength(root.recordPath, isTemplate)
        } else
        if (isTemplate && root.templatePath !== "") {
            return backend.getWaveLength(root.templatePath, isTemplate)
        } else {
            return 0
        }
    }

    function getWaveData() {
        if (root.recordPath !== "") {
            return backend.getWaveData(root.recordPath)
        } else {
            return []
        }
    }

    /*---------------------------------------------------------
      Navigation
      ---------------------------------------------------------*/

    function isPage(page) {
        return currentPage === page
    }

    function goToPage(page, params = {}, force = false) {
        console.log("Bus.goToPage stackView.depth: ", stackView.depth)
        if (currentPage !== page || force) {
            currentPage = page
            stackView.push(page, params)
            console.warn("Bus.goToPage currentPage: ", currentPage)
            validatePage()
        }
    }

    function goBack() {
        if (stackView.depth > 1)
        {
            stackView.pop()
            currentPage = stackView.currentItem.objectName
            console.warn("Bus.goBack currentPage: ", currentPage)
            validatePage()
        }
    }

    function validatePage() {
        if (!currentPage) {
            console.warn("Current page do not have objectName: ", stackView.currentItem)
        }
    }

    function isHomePage() {
        return isPage(Enum.pageHome)
    }

    function goHome() {
        goToPage(Enum.pageHome)
    }

    function isSettingsPage() {
        return isPage(Enum.pageSettings)
    }

    function goSettings() {
        goToPage(Enum.pageSettings)
    }

    function isPolicyPage() {
        return isPage(Enum.pagePolicy)
    }

    function goPolicy() {
        goToPage(Enum.pagePolicy)
    }

    function isResultsPage() {
        return isPage(Enum.pageResults)
    }

    function goResults() {
        goToPage(Enum.pageResults)
    }

    function goRecording() {
        goToPage(Enum.pageRecording)
    }

    function isRecordingPage() {
        return isPage(Enum.pageRecording)
    }

    /*---------------------------------------------------------
      Helpers
      ---------------------------------------------------------*/

    function isMobile() {
        return backend.isMobile()
    }

    function hideAllBottomActions() {
        console.log("Bus.hideAllBottomActions")
        root.showRecordButton = false
        root.showRecordOneButton = false
        root.showPlayButton = false
        root.showOpenButton = false
        root.showOpenTemplateButton = false
        root.showSaveResultsButton = false
    }

    function getTimer(interval, parent) {
        let timer = Qt.createQmlObject("import QtQuick 2.0; Timer {}", parent)
        timer.interval = interval
        return timer
    }

    function reinitialize() {
        console.log("Bus::reinitialize")
        cleanRecord()
        cleanTemplate()
        backend.reinitialize()
    }

    /*---------------------------------------------------------
      Application logic
      ---------------------------------------------------------*/

    readonly property string applicationModeAnalyzer: "Analyzer"
    readonly property string applicationModeMonitor: "Monitor"
    readonly property string applicationModeTrainer: "Trainer"
    property string applicationMode: ""

    function setApplicationMode(mode) {
        root.applicationMode = mode;
    }

    function goRecord(startRecording = false) {
        if (root.applicationMode === root.applicationModeAnalyzer) {
            return goAnalyzer(startRecording);
        } else if(root.applicationMode === root.applicationModeAnalyzer) {
            return goAnalyzer(startRecording);
        } else if(root.applicationMode === root.applicationModeAnalyzer) {
            return goAnalyzer(startRecording);
        } else {
            console.log("Bus.goRecord Error: unknown application mode: ", applicationMode);
        }
    }

    function isAnalyzerPage() {
        return isPage(Enum.pageAnalyser)
    }

    function goAnalyzer(startRecording = false) {
        root.applicationMode = root.applicationModeAnalyzer;
        goToPage(Enum.pageAnalyser, {
            startRecording: startRecording
        })
    }

    function isMonitorPage() {
        return isPage(Enum.pageMonitor)
    }

    function goMonitor(startRecording = false) {
        root.applicationMode = root.applicationModeMonitor;
        goToPage(Enum.pageMonitor, {
            startRecording: startRecording
        })
    }

    function isTrainingPage() {
        return isPage(Enum.pageTraining)
    }

    function goTraining(startRecording = false) {
        root.applicationMode = root.applicationModeTrainer;
        goToPage(Enum.pageTraining, {
            startRecording: startRecording
        })
    }

    function getMetricSE(isTemplate) {
        console.log("Bus::getMetricSE")
        if (!isTemplate && root.recordPath !== "") {
            return backend.getPitchOcavesMetrics(root.recordPath, isTemplate)[5]
        } else
        if (isTemplate && root.templatePath !== "") {
            return backend.getPitchOcavesMetrics(root.templatePath, isTemplate)[5]
        } else {
            return 0
        }
    }

    function getMetricSR(isTemplate) {
        console.log("Bus::getMetricSR")
        console.log("Bus::getMetricSE")
        if (!isTemplate && root.recordPath !== "") {
            return backend.getSpeechRate(root.recordPath, isTemplate)
        } else
        if (isTemplate && root.templatePath !== "") {
            return backend.getSpeechRate(root.templatePath, isTemplate)
        } else {
            return 0
        }
    }

    function getMetricSPD(isTemplate) {
        console.log("Bus::getMetricSPD")
        if (root.recordPath !== "") {
            return backend.getMeanDurationOfPauses(root.recordPath, isTemplate)
        } else {
            return 0;
        }
    }

}
