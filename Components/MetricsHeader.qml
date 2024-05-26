import QtQuick 2.4

import "../App"

MetricsHeaderForm {
    id: root

    Component.onCompleted: {
        showOctavesMetrics()
    }

    function isNumeric(str) {
      return !isNaN(str) && !isNaN(parseFloat(str))
    }

    function showOctavesMetrics() {
        console.log("PitchForm.showOctavesMetrics")

        let metricsData = Bus.getMetrics()

        root.model.clear()
        for(let val in metricsData) {
            let value = metricsData[val]
            console.log("PitchForm.showOctavesMetrics metricsData ", value)

            if (isNumeric(value)) {
                Bus.setCompareResult(value)
            } else {
                let data = {}
                data["itemText1"] = value.split("|")[0]
                data["itemText2"] = value.split("|")[1] || ""
                root.model.append(data)
            }
        }
    }

}
