import QtQuick 2.4

import "../App"

ResultBarForm {
    id: root

    Component.onCompleted: {
        Bus.setResultItem("3", qsTr("Degree of Emotionality"))
        Bus.setResultItem("4", Bus.activeTemplateName + ": " + (Bus.compareResult*100).toFixed(0) + "°")
    }
}
