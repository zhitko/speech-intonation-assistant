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
    property alias durationValue: durationValue

    id: root
    title: qsTr("Intonation Parameters Testing")

    ColumnLayout {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

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

        MetricItem {
            id: metricSE
            title: qsTr("Pitch Variability [Hz]")
            valueTitle: qsTr("")
            titleMin: qsTr("Low")
            titleMid: qsTr("Average")
            titleMax: qsTr("High")
        }

        MetricItem {
            id: metricSR
            title: qsTr("Speech Rate [wpm]")
            titleMin: qsTr("Slow")
            titleMid: qsTr("Average")
            titleMax: qsTr("Fast")
        }
    }
}
