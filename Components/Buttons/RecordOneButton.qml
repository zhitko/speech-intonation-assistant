import QtQuick 2.14

import intondemo.backend 1.0

import "../../App"

RecordOneButtonForm {
    id: root

    property bool isRecording: false
    property bool isTemplate: false

    button.onClicked: {
        Bus.goRecording()
    }
}
