import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Styles 1.4
import QtCharts 2.3
import QtQuick.Layouts 1.14

import "Material"

ChartView {
    id: octaves

    legend.visible: false

    margins.bottom: 0
    margins.top: 0
    margins.left: 0
    margins.right: 30

    property alias axisY: axisY
    property alias axisX: axisX
    property alias recordSeries: recordSeries
    property alias angryTemplateSeries: angryTemplateSeries
    property alias calmTemplateSeries: calmTemplateSeries

    ValueAxis {
        id: axisY
        max: 1
        min: 0
        labelsVisible: true
        tickCount: 10
    }

    ValueAxis {
        id: axisX
        min: 0
        max: 1000
        labelsVisible: false
    }

    LineSeries {
        id: recordSeries
        name: qsTr("Record")
        axisY: axisY
        axisX: axisX
        width: 3
        color: Colors.brown500
    }

    LineSeries {
        id: angryTemplateSeries
        name: qsTr("Angry")
        axisY: axisY
        axisX: axisX
        width: 2
        color: Colors.red500
    }

    LineSeries {
        id: calmTemplateSeries
        name: qsTr("Calm")
        axisY: axisY
        axisX: axisX
        width: 2
        color: Colors.blue500
    }
}
