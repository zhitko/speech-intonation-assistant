import QtQuick 2.4
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "Buttons"
import "Material"
import "../App"

ToolBar {
    id: root

    GridLayout {
        anchors.fill: parent

        PlayButton {
            id: playTemplateButton
            visible: Bus.showPlayButton && Bus.canPlayButton && Bus.templatePath != ""
            Layout.alignment: Qt.AlignCenter
            isTemplate: true            
            color: "#a1a0aa"
        }

        OpenTemplateFileButton {
            id: openTemplateButton
            visible: Bus.showOpenTemplateButton && Bus.canOpenButton
            Layout.alignment: Qt.AlignCenter
        }

        RecordButton {
            id: recordButton
            visible: Bus.showRecordButton && Bus.canRecordButton
            Layout.alignment: Qt.AlignCenter

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 6
                color: Theme.secondaryLight.color
                visible: !recordButton.isRecording
            }
        }

        RecordOneButton {
            id: recordOneButton
            visible: Bus.showRecordOneButton && Bus.canRecordButton
            Layout.alignment: Qt.AlignCenter

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 6
                color: Theme.secondaryLight.color
            }
        }

        PlayButton {
            id: playRecordButton
            visible: Bus.showPlayButton && Bus.canPlayButton && Bus.recordPath !== ""
            Layout.alignment: Qt.AlignCenter
            isTemplate: false

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 6
                color: Theme.secondaryLight.color
            }
        }

        OpenFileButton {
            id: openButton
            visible: Bus.showOpenButton && Bus.canOpenButton
            Layout.alignment: Qt.AlignCenter

            Rectangle {
                visible: Bus.showPlayButton
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 6
                color: Theme.secondaryLight.color
            }
        }

        SaveResultButton {
            id: saveResultsButton
            visible: Bus.showSaveResultsButton
            Layout.alignment: Qt.AlignCenter
        }
    }
}
