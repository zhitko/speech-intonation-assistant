import QtQuick 2.14
import QtQuick.Controls 2.14
import "../FontAwesome"
import "../Material"

ToolButton {
    id: control

    property int baseSize: isRecording ? 14 : 20

    width: 50
    height: 50
    font.family: FontAwesome.solid
    font.pointSize: control.hovered ? baseSize + 4 : baseSize
    text: isRecording ? "Rec": FontAwesome.icons.faMicrophone

    contentItem: Text {
        text: control.text
        font: control.font
        color: Colors.white
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideMiddle
     }

    background: Rectangle {
        implicitWidth: isRecording ? 35 : 50
        implicitHeight: isRecording ? 35 : 50
        radius: 5
        color: isRecording ? Colors.red500 : Theme.primary.color
    }
}
