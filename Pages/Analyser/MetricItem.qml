import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../../App"
import "../../Components/"
import "../../Components/Buttons"
import "../../Components/FontAwesome"
import "../../Components/Material"

Rectangle {
    id: root
    property string title: ""
    property string titleMin: ""
    property string titleMid: ""
    property string titleMax: ""
    property string valueTitle: ""
    property double value: 0
    property double valueMin: 0
    property double valueMax: 0
    property int digits: 0

    property alias canvas: canvas

    Layout.fillWidth: true
    Layout.fillHeight: true
    color: Colors.grey50

    onValueChanged: canvas.requestPaint()
    onValueMinChanged: canvas.requestPaint()
    onValueMaxChanged: canvas.requestPaint()

    Canvas {
        property int lineWidth: 2
        property int barWidth: 30

        property int firstArea: 15
        property int secondArea: 75
        property int thirdArea: 90

        property string minColor: Colors.lightBlueA700
        property string midColor: Colors.lightGreenA700
        property string maxColor: Colors.lightBlueA700

        property double minval: 3
        property double maxval: 87

        id: canvas
        anchors.fill: parent
        antialiasing: true

        onPaint: {
            var ctx = canvas.getContext('2d');
            var minSize = (root.width*0.45 < root.height ? root.width*0.45 : root.height);
            minSize *= 0.7;
            var startX = canvas.width / 2;
            var startY = canvas.height - minSize/6;

            var val = canvas.thirdArea*(root.value-root.valueMin)/(root.valueMax-root.valueMin)

            console.log("canvas.value", root.value);
            console.log("val", val);
            console.log("canvas.minval", canvas.minval);
            console.log("canvas.maxval", canvas.maxval);
            if (val < canvas.minval) {
                val = canvas.minval
            }
            if (val > canvas.maxval) {
                val = canvas.maxval
            }

            var firstAreaToAngle = Math.PI+Math.PI*canvas.firstArea/canvas.thirdArea;
            var secondAreaToAngle = Math.PI+Math.PI*canvas.secondArea/canvas.thirdArea;
            var valueAreaToAngle = Math.PI+Math.PI*val/canvas.thirdArea;

            const arcAngleGap = 0.12;
            const arcAngleMidGap = 0.13;

            var color = canvas.minColor;
            if (val > firstArea) color = canvas.midColor;
            if (val > secondArea) color = canvas.maxColor;

            ctx.save();
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            ctx.globalAlpha = canvas.alpha;

            function drawArc(strokeStyle, lineCap, width, begin, end) {
                ctx.lineCap = lineCap;
                ctx.strokeStyle = strokeStyle;
                ctx.beginPath();
                ctx.lineWidth = width;
                ctx.arc(startX, startY, minSize, begin + arcAngleGap, end - arcAngleGap, false);
                ctx.moveTo(startX, startY)
                ctx.closePath();
                ctx.stroke();
            }

            function drawLine(strokeStyle, width, x1, y1, x2, y2) {
                ctx.strokeStyle = strokeStyle;
                ctx.beginPath();
                ctx.lineWidth = width;
                ctx.moveTo(x1, y1)
                ctx.lineTo(x2, y2)
                ctx.moveTo(x1, y1)
                ctx.closePath();
                ctx.stroke();
            }

            function drawText(fillStyle, strokeStyle, font, textAlign, textBaseline, title, x, y) {
                ctx.fillStyle = fillStyle;
                ctx.strokeStyle = strokeStyle;
                ctx.textBaseline = textBaseline;
                ctx.font = font;
                ctx.textAlign = textAlign;
                ctx.fillText(title, x, y)
            }

            const gradient = ctx.createLinearGradient(startX-minSize,0, startX+minSize,0);
            gradient.addColorStop(0, canvas.minColor);
            gradient.addColorStop(0.3, canvas.midColor);
            gradient.addColorStop(0.7, canvas.midColor);
            gradient.addColorStop(1, canvas.maxColor);

            // Main bar
            drawArc(Colors.blueGray100, 'round', canvas.barWidth*1.4, Math.PI/2, Math.PI/2)
            drawArc(Colors.black, 'round', canvas.barWidth, Math.PI, Math.PI*2)
            drawArc(gradient, 'round', canvas.barWidth - canvas.lineWidth, Math.PI, Math.PI*2)
            // Main bar separator
            drawArc(Colors.grey700, 'butt', canvas.barWidth - canvas.lineWidth, firstAreaToAngle-arcAngleMidGap, firstAreaToAngle+arcAngleMidGap)
            drawArc(Colors.grey700, 'butt', canvas.barWidth - canvas.lineWidth, secondAreaToAngle-arcAngleMidGap, secondAreaToAngle+arcAngleMidGap)
            // Value bar
            drawArc(Colors.grey800, 'round', canvas.barWidth - 5, valueAreaToAngle-0.121, valueAreaToAngle+0.121)
            drawArc(Colors.white, 'round', canvas.barWidth - 15, valueAreaToAngle-0.121, valueAreaToAngle+0.121)
            // Main bar labels
            let fontFamily = 'Gill, Helvetica, sans-serif';
            drawText(Colors.black, Colors.black, `italic ${minSize/7}px ${fontFamily}`, 'right', 'alphabetic', titleMin, startX - minSize*1.1, startY - minSize / 2)
            drawText(Colors.black, Colors.black, `italic ${minSize/7}px ${fontFamily}`, 'center', 'bottom', titleMid, startX, startY - minSize - canvas.barWidth/3*2)
            drawText(Colors.black, Colors.black, `italic ${minSize/7}px ${fontFamily}`, 'left', 'alphabetic', titleMax, startX + minSize*1.1, startY - minSize / 2)
            // Main title lable
            drawText(Colors.black, Colors.black, `${minSize/7}px ${fontFamily}`, 'center', 'bottom', title, startX, startY - minSize/8)
            // Min/Max labels
            drawText(Colors.black, Colors.black, `${minSize/7}px ${fontFamily}`, 'center', 'alphabetic', valueMin.toFixed(digits), startX - minSize, startY + minSize/7)
            drawText(Colors.black, Colors.black, `${minSize/7}px ${fontFamily}`, 'center', 'alphabetic', valueMax.toFixed(digits), startX + minSize, startY + minSize/7)
            // Main value label
            ctx.fillStyle = color;
            ctx.font = `bold ${minSize/3}px ${fontFamily}`;
            ctx.textAlign = 'center';
            ctx.textBaseline = 'middle';
            ctx.lineWidth = 3;
            ctx.fillText(`${root.value.toFixed(digits)} ${valueTitle}`, startX, startY - minSize/2)

            ctx.restore();
        }
    }
}
