import QtQuick 1.1

Menubutton {

    property string rightText: ""

    mouseEnabled: false
    leftAlign: true
    acceptedButtons: Qt.LeftButton | Qt.RightButton

    FontLoader {
        id: arista_light
        source: "arista_light.ttf"
    }

    Text {
        font.family: arista_light.name
        font.pixelSize: 22
        color: "#DDD"
        horizontalAlignment: Text.AlignRight
        text: rightText
        x: parent.width - width - (parent.height - height) * 0.5
        y:  (parent.height - height) * 0.5
    }

}
