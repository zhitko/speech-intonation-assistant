import QtQuick 2.14

import "../../App"

PlayButtonForm {
    id: root

    property bool isTemplate: false

    Timer {
        id: timer
    }

    button.onClicked: {
        let interval = Bus.getLength(root.isTemplate)
        if (interval === 0) return
        Bus.play(root.playing, root.isTemplate)

        root.playing = !root.playing

        if (playing) {
            timer.interval = interval * 1000
            timer.repeat = false
            timer.triggered.connect(function(){
                root.playing = false
            })
            timer.start()
        } else {
            timer.stop()
        }
    }
}
