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
    property string titleX: ""
    property string titleY: ""
    property var values: []
    property var value: []
    property double valueXMin: 0
    property double valueXMax: 100
    property double valueYMin: 0
    property double valueYMax: 100

    property alias canvas: canvas

    Layout.fillWidth: true
    Layout.fillHeight: true
    color: Colors.grey50

    onValuesChanged: canvas.requestPaint()
    onValueChanged: canvas.requestPaint()
    onValueXMinChanged: canvas.requestPaint()
    onValueXMaxChanged: canvas.requestPaint()
    onValueYMinChanged: canvas.requestPaint()
    onValueYMaxChanged: canvas.requestPaint()

    Canvas {
        property int lineWidth: 2
        property int barWidth: 30

        property int firstArea: 15
        property int secondArea: 75
        property int thirdArea: 90

        property string minColor: Colors.lightBlue200
        property string maxColor: Colors.lightGreen400

        property double minval: 3
        property double maxval: 87

        id: canvas
        anchors.fill: parent
        antialiasing: true

        onPaint: {
            const DASH_STYLE = [3, 3];
            const SOLID_STYLE = [3000, 0];

            var ctx = canvas.getContext('2d');

            ctx.save();
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            ctx.globalAlpha = canvas.alpha;

            const canvasHeight = canvas.height;
            const canvasWidth = canvas.width;

            const canvasMargins = 30;

            const fontFamily = 'Gill, Helvetica, sans-serif';
            const fontSize = 18;
            const textProps = [Colors.black, Colors.black, `${fontSize}px ${fontFamily}`, 'center', 'alphabetic'];
            const textBoldProps = [Colors.black, Colors.black, `bold ${fontSize+1}px ${fontFamily}`, 'center', 'alphabetic'];

            function drawRect(fillStyle, x1, y1, w, h) {
                ctx.fillStyle = fillStyle;
                ctx.beginPath();
                ctx.setLineDash(SOLID_STYLE);
                ctx.rect(x1, y1, w, h);
                ctx.closePath();
                ctx.fill();
            }

            function drawText(fillStyle, strokeStyle, font, textAlign, textBaseline, title, x, y, rotation = 0) {
                ctx.fillStyle = fillStyle;
                ctx.strokeStyle = strokeStyle;
                ctx.textBaseline = textBaseline;
                ctx.font = font;
                ctx.textAlign = textAlign;
                ctx.save();
                if (rotation) {
                    ctx.translate(x,y);
                    ctx.rotate(rotation);
                    x = 0;
                    y = 0;
                }
                ctx.fillText(title, x, y);
                ctx.restore();
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

            const bottomMargin = 2 * canvasMargins;
            const leftMargin = 2 * canvasMargins;
            const rightMargin = canvasMargins;
            const areaHeight = canvasHeight - bottomMargin;
            const areaWidth = canvasWidth - leftMargin - rightMargin;

            function valToX(val) {
                return (val-root.valueXMin)/(root.valueXMax-root.valueXMin)*areaWidth+leftMargin;
            }

            function valToY(val) {
                return areaHeight - (val-root.valueYMin)/(root.valueYMax-root.valueYMin)*areaHeight
            }

            const x0 = valToX(value[0]);
            const y0 = valToY(value[1]);
            const r0 = 10;
            const r1 = canvasWidth;
            const gradientStyle = ctx.createRadialGradient(x0, y0, r0, x0, y0, r1);
            gradientStyle.addColorStop(0, maxColor);
            gradientStyle.addColorStop(0.4, minColor);
            gradientStyle.addColorStop(1, minColor);
            drawRect(gradientStyle, leftMargin, 0, areaWidth, areaHeight);

            const x1 = leftMargin;
            const x2 = leftMargin + areaWidth/4;
            const x3 = leftMargin + areaWidth/2;
            const x4 = leftMargin + areaWidth/4*3;
            const x5 = canvasWidth - rightMargin;

            drawLine(Colors.white, 1, SOLID_STYLE, x2, 0, x2, areaHeight);
            drawLine(Colors.white, 1, SOLID_STYLE, x3, 0, x3, areaHeight);
            drawLine(Colors.white, 1, SOLID_STYLE, x4, 0, x4, areaHeight);

            const y1 = canvasHeight - bottomMargin;
            const y2 = areaHeight/4*3;
            const y3 = areaHeight/2;
            const y4 = areaHeight/4;
            const y5 = fontSize;

            drawLine(Colors.white, 1, SOLID_STYLE, leftMargin, y4, leftMargin + areaWidth, y4);
            drawLine(Colors.white, 1, SOLID_STYLE, leftMargin, y3, leftMargin + areaWidth, y3);
            drawLine(Colors.white, 1, SOLID_STYLE, leftMargin, y2, leftMargin + areaWidth, y2);

            drawCircle(Colors.green900, 4, x0, y0, r0/2);
            drawCircle(Colors.white, 4, x0, y0, r0);

            const x1v = root.valueXMin;
            const x2v = root.valueXMin + (root.valueXMax-root.valueXMin)/4;
            const x3v = root.valueXMin + (root.valueXMax-root.valueXMin)/2;
            const x4v = root.valueXMin + (root.valueXMax-root.valueXMin)/4*3;
            const x5v = root.valueXMax;

            const xLablesY = canvasHeight - bottomMargin + canvasMargins - fontSize/3;
            drawText(...textProps, root.titleX, canvasWidth/2, xLablesY + canvasMargins);
            drawText(...textBoldProps, qsTr("( X )"), canvasWidth - 2 * rightMargin, xLablesY + canvasMargins);
            drawText(...textProps, x1v.toFixed(0), x1, xLablesY);
            drawText(...textProps, x2v.toFixed(0), x2, xLablesY);
            drawText(...textProps, x3v.toFixed(0), x3, xLablesY);
            drawText(...textProps, x4v.toFixed(0), x4, xLablesY);
            drawText(...textProps, x5v.toFixed(0), x5, xLablesY);

            const y1v = root.valueYMin;
            const y2v = root.valueYMin + (root.valueYMax-root.valueYMin)/4;
            const y3v = root.valueYMin + (root.valueYMax-root.valueYMin)/2;
            const y4v = root.valueYMin + (root.valueYMax-root.valueYMin)/4*3;
            const y5v = root.valueYMax;

            drawText(...textProps, root.titleY, fontSize, canvasHeight/2, -Math.PI/2);
            drawText(...textBoldProps, qsTr("( Y )"), fontSize, 2*fontSize, -Math.PI/2);
            drawText(...textProps, y1v, canvasMargins+fontSize/2, y1);
            drawText(...textProps, y2v, canvasMargins+fontSize/2, y2);
            drawText(...textProps, y3v, canvasMargins+fontSize/2, y3);
            drawText(...textProps, y4v, canvasMargins+fontSize/2, y4);
            drawText(...textProps, y5v, canvasMargins+fontSize/2, y5);

            for(let i=0;i<root.values.length;i++) {
                let x = valToX(root.values[i][0]);
                let y = valToY(root.values[i][1]);
                console.log("x", x);
                console.log("y", y);
                drawCircle(Colors.white, 4, x, y, r0/2);
                if (i==(root.values.length-1)) {
                    drawCircle(Colors.black, 3, x, y, r0/4);
                }
            }

            ctx.restore();
        }
    }
}
