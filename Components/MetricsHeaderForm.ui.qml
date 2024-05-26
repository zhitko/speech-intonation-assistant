import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.14

import "FontAwesome"
import "Material"

ListView {
    id: list

    readonly property TextMetrics tm: TextMetrics { text: "#";  font.pointSize: 12}
    readonly property int itemPadding: 5
    readonly property int itemHeight: tm.height + itemPadding
    readonly property int recMargin: 1
    property int visibleItems: model.count

    clip: true
    height: visibleItems * itemHeight
    spacing: itemPadding

    ListModel {
       id: componentModel
    }

    Component {
        id: componentDelegate

        RowLayout {
            width: list.width
            height: tm.height

            Rectangle {
                id: bar
                color: Theme.primaryLight.color
                width: 20
                Layout.fillHeight: true
            }

            Text {
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: itemText1
                textFormat: Text.RichText
                font.pointSize: tm.font.pointSize
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle {
                id: progress
                visible: !!itemText2
                color: Theme.primary.textColor
                Layout.fillHeight: true
                width: 100

                Rectangle {
                    color: Theme.primary.color
                    anchors.top: parent.top
                    anchors.topMargin: recMargin
                    anchors.right: parent.right
                    anchors.rightMargin: recMargin
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: recMargin
                    anchors.left: parent.left
                    anchors.leftMargin: recMargin

                    Rectangle {
                        color: Theme.primary.textColor
                        anchors.right: parent.right
                        anchors.rightMargin: recMargin
                        anchors.top: parent.top
                        anchors.topMargin: recMargin
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: recMargin
                        width: (1 - itemText2) * (progress.width - 4 * recMargin)
                    }
                }
            }

            Rectangle {
                color: bar.color
                width: bar.width
                Layout.fillHeight: true
            }
        }
    }

    model: componentModel
    delegate: componentDelegate
}
