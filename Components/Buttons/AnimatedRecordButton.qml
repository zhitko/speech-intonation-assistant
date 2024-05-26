import QtQuick 2.4

import "../../App"

AnimatedRecordButtonForm {
    id: root

    property int frame: 0
    property bool isTemplate: false
    property bool isSetPath: true

    signal recordingStarted(path: string)
    signal recordingStopped(path: string)

    Timer {
        id: timer
        interval: 500;
        repeat: true
        onTriggered: {
            root.icon.source = "qrc:/Assets/Images/speech-active-%1-icon.png".arg(frame)
            frame ++
            if (frame > 3) frame = 0
        }
    }

    function getDatetime() {
        var today = new Date()
        var date = today.getFullYear()+'-'+(today.getMonth()+1)+'-'+today.getDate()
        var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds()
        return date+' '+time
    }

    function startRecording() {
        console.log("Bus.startRecording")
        const dateTime = getDatetime()

        const path = Bus.startRecord(isTemplate, false)
        timer.start()
        root.icon.source = "qrc:/Assets/Images/speech-active-0-icon.png"
        Bus.setResultItem("1", "Recording started: " + dateTime)
        console.log("Bus.startRecording emit recordingStarted")
        root.recordingStarted(path)

        Bus.customEvent("AnimatedRecordButton:startRecording")
    }

    function stopRecording() {
        console.log("Bus.stopRecording")
        const dateTime = getDatetime()

        const path = Bus.stopRecord(isTemplate, isSetPath)
        timer.stop()
        root.icon.source = "qrc:/Assets/Images/speech-icon.png"
        Bus.setResultItem("2", "End of the recording: " + dateTime)
        console.log("Bus.startRecording emit recordingStopped")
        root.recordingStopped(path)

        Bus.customEvent("AnimatedRecordButton:stopRecording")
    }

    onCheckedChanged: {
        if (checked) {
            startRecording()
        } else {
            stopRecording()
        }
    }
}
