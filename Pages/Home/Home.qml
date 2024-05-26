import QtQuick 2.14
import QtQuick.Controls 2.14

import "../../App"

HomeForm {
    id: root
    objectName: Enum.pageHome

    StackView.onActivated: {
        console.log("HomeForm.StackView.onActivated")
        Bus.reinitialize()
        Bus.hideAllBottomActions()
    }

    analyserButton.onClicked: {
        Bus.goAnalyzer()
    }

    monitorButton.onClicked: {
        Bus.goMonitor()
    }

    trainerButton.onClicked: {
        Bus.goTraining()
    }
}
