import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../../App"
import "../../Components/"
import "../../Components/Buttons"
import "../../Components/FontAwesome"
import "../../Components/Material"

Page {
    property alias metrics: metrics
    property alias sampleDurationValue: sampleDurationValue
    property alias spokenDurationValue: spokenDurationValue
    property alias xScoreLabel: xScoreLabel
    property alias yScoreLabel: yScoreLabel
    property alias xyScoreLabel: xyScoreLabel


    id: root
    title: qsTr("Intonation Parameters Training")

    ColumnLayout {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        RowLayout {
            Layout.margins: 10
            Label {
                id: sampleDurationLabel
                text: qsTr("Reference speech sample duration (sec)")
                font.pointSize: 16
            }
            Label {
                id: sampleDurationValue

                Layout.leftMargin: 22
                font.pixelSize: sampleDurationLabel.font.pixelSize
                font.bold: true

                text: "-"
            }
        }

        RowLayout {
            Layout.margins: 10
            Layout.topMargin: 0
            Label {
                id: spokenDurationLabel
                text: qsTr("Spoken speech sample duration (sec)")
                font.pointSize: 16
                font.italic: true
            }
            Label {
                id: spokenDurationValue

                Layout.leftMargin: 22
                font.pixelSize: spokenDurationLabel.font.pixelSize
                font.bold: true
                font.italic: true

                text: "-"
            }
        }

        GraphItem {
            id: metrics
            titleX: qsTr("Speech Rate [wpm]")
            titleY: qsTr("Pitch Variability [Hz]")
            values: []
            value: [0,0]
            valueXMin: 0
            valueXMax: 100
            valueYMin: 0
            valueYMax: 100
        }

        RowLayout {
            Layout.margins: 10

            Rectangle {
                color: Colors.blue50

                Layout.fillWidth: true
                height: 100

                Label {
                    font.pointSize: 14
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    horizontalAlignment: "AlignHCenter"
                    text: qsTr("X \nScore")
                }

                Label {
                    id: xScoreLabel
                    font.pointSize: 22
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    horizontalAlignment: "AlignHCenter"
                    text: qsTr("-")
                    color: Colors.blue900
                }
            }

            Rectangle {
                color: Colors.green50

                Layout.fillWidth: true
                height: 100

                Label {
                    font.pointSize: 14
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    horizontalAlignment: "AlignHCenter"
                    text: qsTr("Y \nScore")
                }

                Label {
                    id: yScoreLabel
                    font.pointSize: 22
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    horizontalAlignment: "AlignHCenter"
                    text: qsTr("-")
                    color: Colors.green900
                }
            }

            Rectangle {
                color: Colors.red50

                Layout.fillWidth: true
                height: 100

                Label {
                    font.pointSize: 14
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    horizontalAlignment: "AlignHCenter"
                    text: qsTr("X, Y \nScore")
                }

                Label {
                    id: xyScoreLabel
                    font.pointSize: 22
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    horizontalAlignment: "AlignHCenter"
                    text: qsTr("-")
                    color: Colors.red900
                }
            }
        }
    }
}
