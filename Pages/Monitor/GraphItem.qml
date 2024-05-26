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
    property var values: []
    property double valueMin: 0
    property double valueMax: 0
    property var valuesMaxCount: 15
    property int digits: 0

    property alias canvas: canvas

    Layout.fillWidth: true
    Layout.fillHeight: true
    color: Colors.grey50

    onValuesChanged: canvas.requestPaint()
    onValueMinChanged: canvas.requestPaint()
    onValueMaxChanged: canvas.requestPaint()

    Canvas {
        property int lineWidth: 2
        property int barWidth: 30

        property int firstArea: 30
        property int secondArea: 60
        property int thirdArea: 90

        property string minColor: Colors.lightBlueA700
        property string minColorBackground: Colors.lightBlue200
        property string midColor: Colors.lightGreenA700
        property string midColorBackground: Colors.lightGreen400
        property string maxColor: Colors.lightBlueA700
        property string maxColorBackground: Colors.lightBlue200

        property double minval: 3
        property double maxval: 87

        id: canvas
        anchors.fill: parent
        antialiasing: true

        onPaint: {
            const DASH_STYLE = [3, 3];
            const SOLID_STYLE = [1000, 0];

            var ctx = canvas.getContext('2d');

            let fontFamily = 'Gill, Helvetica, sans-serif';

            ctx.save();
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            ctx.globalAlpha = canvas.alpha;

            const canvasHeight = canvas.height;
            const canvasWidth = canvas.width;

            const verticalCenter = canvasHeight / 2;
            const horizontalCenter = canvasWidth / 2;

            const firstAreaHeight = canvasHeight / canvas.thirdArea * canvas.firstArea;
            const firstAreaY = canvasHeight - firstAreaHeight;

            const secondAreaHeight = canvasHeight / canvas.thirdArea * (canvas.secondArea - canvas.firstArea);
            const secondAreaY = firstAreaY - secondAreaHeight;

            const thirdAreaHeight = canvasHeight / canvas.thirdArea * (canvas.thirdArea - canvas.secondArea);
            const thirdAreaY = secondAreaY - thirdAreaHeight;

            function drawRect(fillStyle, x1, y1, w, h) {
                ctx.fillStyle = fillStyle;
                ctx.beginPath();
                ctx.setLineDash(SOLID_STYLE);
                ctx.rect(x1, y1, w, h);
                ctx.closePath();
                ctx.fill();
            }

            function drawText(fillStyle, strokeStyle, font, textAlign, textBaseline, title, x, y) {
                ctx.fillStyle = fillStyle;
                ctx.strokeStyle = strokeStyle;
                ctx.textBaseline = textBaseline;
                ctx.font = font;
                ctx.textAlign = textAlign;
                ctx.fillText(title, x, y)
            }

            function drawLine(strokeStyle, width, lineDash, x1, y1, x2, y2) {
                ctx.strokeStyle = strokeStyle;
                ctx.beginPath();
                ctx.setLineDash(lineDash);
                ctx.lineWidth = width;
                ctx.moveTo(x1, y1)
                ctx.lineTo(x2, y2)
                ctx.moveTo(x1, y1)
                ctx.closePath();
                ctx.stroke();
            }

            function drawCircle(strokeStyle, width, x, y, r) {
                ctx.strokeStyle = strokeStyle;
                ctx.beginPath();
                ctx.setLineDash(SOLID_STYLE);
                ctx.lineWidth = width;
                ctx.arc(x, y, r, 0, Math.PI*2);
                ctx.moveTo(x, y)
                ctx.closePath();
                ctx.stroke();
            }

            const gradientStyle = ctx.createLinearGradient(0, canvas.height, 0, 0);
            gradientStyle.addColorStop(0, canvas.minColorBackground);
            gradientStyle.addColorStop(0.4, canvas.midColorBackground);
            gradientStyle.addColorStop(0.6, canvas.midColorBackground);
            gradientStyle.addColorStop(1, canvas.maxColorBackground);

            drawRect(gradientStyle, 0, 0, canvasWidth, canvasHeight)

            drawLine(Colors.blueGray300, 1, SOLID_STYLE, 0, verticalCenter, canvasWidth, verticalCenter)

            drawLine(Colors.blueGray400, 2, DASH_STYLE, 0, canvasHeight/4, canvasWidth, canvasHeight/4)
            drawLine(Colors.blueGray400, 2, DASH_STYLE, 0, canvasHeight/4*3, canvasWidth, canvasHeight/4*3)

            let valueMinMax = valueMax - valueMin;
            drawText(Colors.black, Colors.black, `${14}px ${fontFamily}`, 'left', 'alphabetic', valueMax, 5, 14)
            drawText(Colors.black, Colors.black, `${14}px ${fontFamily}`, 'left', 'alphabetic', (valueMin + valueMinMax/4*3).toFixed(0), 5, canvasHeight/4+5)
            drawText(Colors.black, Colors.black, `${14}px ${fontFamily}`, 'left', 'alphabetic', (valueMin + valueMinMax/2).toFixed(0), 5, canvasHeight/2+5)
            drawText(Colors.black, Colors.black, `${14}px ${fontFamily}`, 'left', 'alphabetic', (valueMin + valueMinMax/4).toFixed(0), 5, canvasHeight/4*3+5)
            drawText(Colors.black, Colors.black, `${14}px ${fontFamily}`, 'left', 'alphabetic', valueMin, 5, canvasHeight-5)

            const dValueX = canvasWidth / (root.valuesMaxCount+1);

            const radius = 5;

            function valToCoordinates(element) {
                const height = (canvasHeight-radius*2);
                return height - (element-root.valueMin)/(root.valueMax-root.valueMin)*height;
            }

            for(let i=1;i<root.values.length;i++) {
                let value1 = valToCoordinates(root.values[i-1])
                let value2 = valToCoordinates(root.values[i])
                drawLine(Colors.blueGray400, 3, SOLID_STYLE, dValueX*i, value1, dValueX*(i+1), value2)
            }

            for(let i=0;i<root.values.length;i++) {
                let value = valToCoordinates(root.values[i])
                drawCircle(Colors.black, 1, dValueX*(i+1), value, radius);
                ctx.fillStyle = Colors.white;
                ctx.fill();
            }

            ctx.restore();
        }
    }
}
