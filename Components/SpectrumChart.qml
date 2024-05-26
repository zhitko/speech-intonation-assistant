import QtQuick 2.14
import QtQuick.Controls 2.14
import QtCharts 2.14

import "../App"
import "Material"

SpectrumChartForm {
    id: root

    Component.onCompleted: {
        showSpectrum()
    }

    function showSpectrum() {
        console.log("SpectrumChart.showSpectrum")

        console.log("SpectrumChart.showSpectrum record")
        let recordSpectrum = Bus.getSpectrumData(false)
        showSeries(root.recordSeries, recordSpectrum)

        console.log("SpectrumChart.showSpectrum angry template")
        let templateAngrySpectrum = Bus.getSpectrumData(true, true)
        showSeries(root.angryTemplateSeries, templateAngrySpectrum)

        console.log("SpectrumChart.showSpectrum calm template")
        let templateCalmSpectrum = Bus.getSpectrumData(true, false)
        showSeries(root.calmTemplateSeries, templateCalmSpectrum)
    }

    function showSeries(series, data) {
        console.log("SpectrumChartForm.showSeries", data)

        if (data.length === 0) return 0;
        for (let i=0; i< data.length - 1; i++) {
            console.log("value: ", data[i])
            series.append(data[i].x, data[i].y)
        }

        axisX.max = data[data.length - 1].x
    }
}
