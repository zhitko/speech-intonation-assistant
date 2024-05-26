import QtQuick 2.14
import QtQuick.Controls 2.14
import QtCharts 2.14

import "../App"
import "Material"

VoiceChartFooterForm {
    id: root

    Component.onCompleted: {
        loadMinMax()
    }

    function loadMinMax() {
        let octavesCategories = Bus.getOctavesCategories()

        root.minLabel.text = octavesCategories[0]
        root.maxLabel.text = octavesCategories[octavesCategories.length - 1]
    }

}
