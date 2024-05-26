import QtQuick 2.14
import QtQuick.Controls 2.14
import QtCharts 2.14

import "../App"
import "Material"

VoiceChartGraphForm {
    id: root

    Component.onCompleted: {
        showOctaves()
    }

    function showOctaves() {
        console.log("VoiceChartForm.showOctaves: ")

        showOctavesData(root.recordSeries, Bus.getOctavesData(false))
        showOctavesData(root.angryTemplateSeries, Bus.getOctavesData(true, true))
        showOctavesData(root.calmTemplateSeries, Bus.getOctavesData(true, false))
    }

    function showOctavesData(series, data) {
        console.log("VoiceChartForm.showOctaves", data)

        if (data.length === 0) return 0;
        for (let i=0; i< data.length - 1; i++) {
            console.log("value: ", data[i])
            series.append(i, data[i])
        }

        axisX.max = data.length - 1
    }
}
