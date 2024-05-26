import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../../App"
import "../../Components/"
import "../../Components/Buttons"
import "../../Components/FontAwesome"
import "../../Components/Material"

Page {
    property alias metricSE: metricSE
    property alias metricSR: metricSR
    property alias headerSE: headerSE
    property alias headerSR: headerSR
    property alias durationValue: durationValue
    property alias durationLabel: durationLabel

    id: root
    title: qsTr("Intonation Parameters Monitoring")

    ColumnLayout {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5

        RowLayout {
            Layout.margins: 10
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

        ColumnLayout {
            Header {
                id: headerSE
                text: qsTr("Pitch Variability [Hz]")
                Layout.alignment: Qt.AlignHCenter
            }

            GraphItem {
                id: metricSE
            }
        }


        ColumnLayout {
            Header {
                id: headerSR
                text: qsTr("Speech Rate [wpm]")
                Layout.alignment: Qt.AlignHCenter
            }

            GraphItem {
                id: metricSR
            }
        }
    }
}
