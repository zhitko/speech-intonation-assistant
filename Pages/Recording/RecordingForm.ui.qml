import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12

import "../../App"
import "../../Components/"
import "../../Components/Buttons"
import "../../Components/FontAwesome"

Page {
    id: root
    title: qsTr("Recording")

    property alias recordButton: recordButton
    property alias durationValue: durationValue
    property alias durationLabel: durationLabel

    RowLayout {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        Label {
            id: durationLabel
            text: qsTr("Speech Duration (sec)")
            font.pointSize: 16
        }
        Label {
            id: durationValue

            Layout.leftMargin: 20
            font.pixelSize: durationLabel.font.pixelSize
            font.bold: true

            text: "-"
        }
    }

    AnimatedRecordButton {
        id: recordButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        height: 200
        width: 200
    }

    Label {
        id: recordButtonHint
        anchors.top: recordButton.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        text: recordButton.checked ? qsTr("Click to stop recording") : qsTr("Click to start recording")
        font.pointSize: 15
        wrapMode: Text.WordWrap
        horizontalAlignment: "AlignHCenter"
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

