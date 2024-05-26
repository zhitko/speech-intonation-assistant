import QtQuick 2.14
import QtQuick.Controls 2.14

import "../../App"

RecordingForm {
    id: root
    objectName: Enum.pageRecording

    property bool startRecording: false
    property var recordingStartTime: 0

    property int count: 0

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

    recordButton.onCheckedChanged: {
        if (recordButton.checked) {
            root.recordingStartTime = new Date()
            animationTimer.start()
        } else {
            animationTimer.stop()
        }
    }

    StackView.onActivated: {
        console.log("RecordingForm.StackView.onActivated", root.startRecording)
        Bus.hideAllBottomActions()

        if (root.startRecording == true) {
            recordButton.checked = true
        }
    }

    Connections {
        target: Bus
        function onNewRecordPath() {
            console.log("RecordingForm.onNewRecordPath goBack")
            Bus.goBack()
        }
    }
}
