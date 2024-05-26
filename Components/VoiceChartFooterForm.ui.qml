import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.14

ListView {
    id: root

    property alias minLabel: minLabel
    property alias maxLabel: maxLabel

    orientation: ListView.Horizontal
    height: 20
    model: [1,2,3,4]
    delegate: Rectangle {
        width: ListView.view.width / 4 + (index === 0 ? 2 : 0)
        height: root.height
        color: 'lightgray'
        Rectangle {
            anchors.fill: parent
            anchors.rightMargin: 1
            anchors.leftMargin: index === 0 ? 1 : 0
        }
    }

    Label {
        id: minLabel
        anchors.bottom: root.bottom
        anchors.left: root.left
        anchors.leftMargin: 5
        text: "min"
        font.pointSize:  10
        font.bold: true
    }

    Label {
        id: maxLabel
        anchors.bottom: root.bottom
        anchors.right: root.right
        anchors.rightMargin: 5
        text: "max"
        font.pointSize:  10
        font.bold: true
    }
}
