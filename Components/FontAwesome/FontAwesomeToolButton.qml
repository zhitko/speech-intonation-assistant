import QtQuick 2.14
import QtQuick.Controls 2.14

Item {
    id: root

    property alias button: button
    property alias label: label
    property alias icon: awesome.icon
    property alias font: awesome.font
    property alias color: awesome.color
    property alias type: awesome.type
    property alias hintText: hint.text
    property alias hintColor: hint.color
    property bool hintVisibleAlways: false

    property int baseSize: 22

    width: 32
    height: 32

    FontAwesomeIcon {
        id: awesome
        font.pointSize: button.hovered ? baseSize + 4 : baseSize
        anchors.centerIn: parent
    }

    ToolButton {
        id: button
        anchors.fill: parent
    }

    Label {
        id: hint

        anchors.top: button.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: button.horizontalCenter

        visible: hintVisibleAlways ? true : button.hovered
        font.pointSize: button.hovered ? 10 : 8
    }

    Label {
        id: label

        anchors.left: button.right
        anchors.leftMargin: 5
        anchors.verticalCenter: button.verticalCenter

        visible: !!label.text
        font.pointSize: root.font.pointSize - 1
        color: root.color
    }
}
