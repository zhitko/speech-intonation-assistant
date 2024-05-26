import QtQuick 2.14

import intondemo.backend 1.0

import "../../App"

RecordButtonForm {
    id: root

    property bool isRecording: false
    property bool isTemplate: false
    property var recordingStartTime: 0
    property bool showLabel: false

    Timer {
        id: animationTimer
        interval: 100;
        repeat: true
        onTriggered: {
            root.opacity -= 0.05
            if (root.opacity <= 0.5) {
                root.opacity = 1
            }
            let time = new Date()
            if (showLabel) {
                root.text = ((time - root.recordingStartTime)/1000).toFixed(1)
            }
        }
    }

    Timer {
        id: timeout
        function setTimeout(cb, delayTime) {
            timeout.interval = delayTime;
            timeout.repeat = false;
            timeout.triggered.connect(cb);
            timeout.triggered.connect(function release () {
                timeout.triggered.disconnect(cb); // This is important
                timeout.triggered.disconnect(release); // This is important as well
            });
            timeout.start();
        }
    }

    Timer {
        id: timer
        interval: 1
        repeat: true
        onTriggered: {
            if (root.isRecording) {
                Bus.stopRecord(root.isTemplate, true);
                timeout.setTimeout(function(){ Bus.startRecord(root.isTemplate, false);; }, 5);
            }
        }
    }

    onClicked: {
        if (root.isRecording) {
            timer.stop();
            animationTimer.stop();
            root.opacity = 1
            Bus.stopRecord(root.isTemplate, false);
            Bus.customEvent("RecordButton::stopRecording");
        } else {
            root.recordingStartTime = new Date()
            console.log(Bus.getSettingValue('MonitoringRecordDuration'));
            timer.interval = 1000*Bus.getSettingValue('MonitoringRecordDuration');
            timer.start();
            animationTimer.start();
            Bus.startRecord(root.isTemplate, false);
            Bus.customEvent("RecordButton::startRecord");
        }
        root.isRecording = !root.isRecording;
    }
}
