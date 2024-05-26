import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.14

import "FontAwesome"
import "Material"
import "../App"

ColumnLayout {
    id: root

    readonly property int recMargin: 2
    property int isHight: Bus.compareResult >= 0.7
    property int isMid: (Bus.compareResult >= 0.4 && Bus.compareResult < 0.7)
    property int isLow: Bus.compareResult < 0.4

    Label {
        Layout.fillWidth: true

        text: qsTr("Degree of Emotionality") + ": <b>" + (Bus.compareResult*100).toFixed(0) + "°</b>"
        font.pointSize: 14
        horizontalAlignment: "AlignHCenter"
        color: Theme.backgound.textColor
    }

    Label {
        Layout.fillWidth: true

        text: qsTr("Basic voice: ") + (Bus.activeTemplateName != "" ? Bus.activeTemplateName : "")
        font.pointSize: 12
        horizontalAlignment: "AlignHCenter"
        color: Theme.backgound.textColor
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Rectangle {
            color: Theme.primary.textColor
            Layout.fillHeight: true
            Layout.fillWidth: true
            width: 15

            ColumnLayout {
                id: labels
                anchors.top: parent.top
                width: 70
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 25
                anchors.left: parent.horizontalCenter
                spacing: 0
                Rectangle {
                    color: Theme.primary.color
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Rectangle {
                        color: Theme.primary.textColor
                        anchors.fill: parent
                        anchors.bottomMargin: 1
                        Label {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 1
                            text: "100"
                        }
                    }
                }
                Rectangle {
                    color: Theme.primary.color
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Rectangle {
                        color: Theme.primary.textColor
                        anchors.fill: parent
                        anchors.bottomMargin: 1
                        Label {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 1
                            text: "90"
                        }
                    }
                }
                Rectangle {
                    color: Theme.primary.color
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Rectangle {
                        color: Theme.primary.textColor
                        anchors.fill: parent
                        anchors.bottomMargin: 1
                        Label {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 1
                            text: "80"
                        }
                    }
                }
                Rectangle {
                    color: Theme.primary.color
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Rectangle {
                        color: Theme.primary.textColor
                        anchors.fill: parent
                        anchors.bottomMargin: 1
                        Label {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 1
                            text: "70"
                        }
                    }
                }
                Rectangle {
                    color: Theme.primary.color
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Rectangle {
                        color: Theme.primary.textColor
                        anchors.fill: parent
                        anchors.bottomMargin: 1
                        Label {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 1
                            text: "60"
                        }
                    }
                }
                Rectangle {
                    color: Theme.primary.color
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Rectangle {
                        color: Theme.primary.textColor
                        anchors.fill: parent
                        anchors.bottomMargin: 1
                        Label {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 1
                            text: "50"
                        }
                    }
                }
                Rectangle {
                    color: Theme.primary.color
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Rectangle {
                        color: Theme.primary.textColor
                        anchors.fill: parent
                        anchors.bottomMargin: 1
                        Label {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 1
                            text: "40"
                        }
                    }
                }
                Rectangle {
                    color: Theme.primary.color
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Rectangle {
                        color: Theme.primary.textColor
                        anchors.fill: parent
                        anchors.bottomMargin: 1
                        Label {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 1
                            text: "30"
                        }
                    }
                }
                Rectangle {
                    color: Theme.primary.color
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Rectangle {
                        color: Theme.primary.textColor
                        anchors.fill: parent
                        anchors.bottomMargin: 1
                        Label {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 1
                            text: "20"
                        }
                    }
                }
                Rectangle {
                    color: Theme.primary.color
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Rectangle {
                        color: Theme.primary.textColor
                        anchors.fill: parent
                        anchors.bottomMargin: 1
                        Label {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 1
                            text: "10"
                        }
                    }
                }
                Rectangle {
                    color: Theme.primary.color
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Rectangle {
                        color: Theme.primary.textColor
                        anchors.fill: parent
                        anchors.bottomMargin: 1
                        Label {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 1
                            text: "0"
                        }
                    }
                }
            }

            Rectangle {
                id: circle
                color: Theme.primary.color
                anchors.top: labels.bottom
                anchors.left: labels.left
                anchors.leftMargin: -width / 2
                width: 30
                height: 30
                radius: width / 2
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 1
                    radius: parent.radius
                    color: Theme.primary.textColor
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: 1
                        radius: parent.radius
                        color: Theme.primary.color
                    }
                }
            }

            Rectangle {
                id: tube
                color: Theme.primary.color
                anchors.top: labels.top
                anchors.topMargin: labels.height / 11 - 3
                anchors.bottom: labels.bottom
                anchors.bottomMargin: -width
                anchors.horizontalCenter: circle.horizontalCenter
                width: circle.width / 2
            }

            Rectangle {
                id: tubeValue
                color: Theme.primary.textColor
                anchors.margins: 1
                anchors.top: tube.top
                anchors.bottom: labels.bottom
                anchors.bottomMargin: -1
                anchors.left: tube.left
                anchors.right: tube.right
                Rectangle {
                    color: Theme.primary.color
                    anchors.margins: 1
                    anchors.bottom: tubeValue.bottom
                    anchors.left: tubeValue.left
                    anchors.right: tubeValue.right
                    height: Bus.compareResult * (tubeValue.height - 2)
                }
            }
            Rectangle {
                height: 10
                color: Theme.primary.color
                anchors.margins: 1
                anchors.bottom: tubeValue.bottom
                anchors.bottomMargin: -8
                anchors.left: tubeValue.left
                anchors.right: tubeValue.right
            }
        }

        ColumnLayout {
            spacing: 0
            Layout.topMargin: 20
            Layout.bottomMargin: 25

            Rectangle {
                id: hightSection
                width: 30
                Layout.fillHeight: true
                color: isHight ? Colors.red400 : Colors.red50
                opacity: isHight ? 1 : 0.5
                Label {
                    anchors.left: parent.left
                    anchors.top: parent.bottom
                    height: parent.width
                    width: parent.height
                    horizontalAlignment: "AlignHCenter"
                    verticalAlignment: "AlignVCenter"
                    transform: Rotation { origin.x: 0; origin.y: 0; angle: -90}
                    text: qsTr("High")
                }
            }
            Rectangle {
                id: midSection
                width: 30
                Layout.fillHeight: true
                color: isMid ? Colors.yellow400 : Colors.yellow50
                opacity: isMid ? 1 : 0.5
                Label {
                    anchors.left: parent.left
                    anchors.top: parent.bottom
                    height: parent.width
                    width: parent.height
                    horizontalAlignment: "AlignHCenter"
                    verticalAlignment: "AlignVCenter"
                    transform: Rotation { origin.x: 0; origin.y: 0; angle: -90}
                    text: qsTr("Medium")
                }
            }
            Rectangle {
                id: lowSection
                width: 30
                Layout.fillHeight: true
                color: isLow ? Colors.green400 : Colors.green50
                opacity: isLow ? 1 : 0.5
                Label {
                    anchors.left: parent.left
                    anchors.top: parent.bottom
                    height: parent.width
                    width: parent.height
                    horizontalAlignment: "AlignHCenter"
                    verticalAlignment: "AlignVCenter"
                    transform: Rotation { origin.x: 0; origin.y: 0; angle: -90}
                    text: qsTr("Low")
                }
            }
        }

        ColumnLayout {
            spacing: 0
            Layout.topMargin: 20
            Layout.bottomMargin: 25

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 10
                color: hightSection.color
                opacity: hightSection.opacity

                Label {
                    text: qsTr("<b>You can't control your emotions</b>")
                    anchors.fill: parent
                    wrapMode: Text.WordWrap
                    padding: 5
                    horizontalAlignment: "AlignHCenter"
                    verticalAlignment: "AlignVCenter"
                }
            }
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 10
                color: midSection.color
                opacity: midSection.opacity

                Label {
                    text: qsTr("<b>You are in control of your emotions</b>")
                    anchors.fill: parent
                    wrapMode: Text.WordWrap
                    padding: 5
                    horizontalAlignment: "AlignHCenter"
                    verticalAlignment: "AlignVCenter"
                }
            }
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 10
                color: lowSection.color
                opacity: lowSection.opacity

                Label {
                    text: qsTr("<b>You have no tension and stress</b>")
                    anchors.fill: parent
                    wrapMode: Text.WordWrap
                    padding: 5
                    horizontalAlignment: "AlignHCenter"
                    verticalAlignment: "AlignVCenter"
                }
            }
        }
    }
}
