import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

ColumnLayout {
    id: root

    property string text: ""
    property string mean: "-"
    property string rms: "-"

    Label {
        text: root.text
        font.pointSize: 14
        Layout.alignment: Qt.AlignHCenter
    }
    RowLayout {
        Layout.alignment: Qt.AlignHCenter
        Label {
            text: qsTr("Mean value:")
            font.pointSize: 14
        }
        Label {
            text: root.mean
            font.pointSize: 16
            font.bold: true
            Layout.rightMargin: 30
        }
        Label {
            text: qsTr("Deviation:")
            font.pointSize: 14
            Layout.leftMargin: 30
        }
        Label {
            text: root.rms
            font.pointSize: 16
            font.bold: true
        }
    }
}
