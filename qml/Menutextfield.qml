import QtQuick 1.1

Menubutton {

    leftAlign: true;
    property string playerName: ""
    onClick: textinput.focus = true

    function getText() {
        return textinput.displayText;
    }

    Rectangle {
        height: textinput.height+2
        width: textinput.width+4
        radius: 3
        color: "black"
        opacity: 0.3
        x: textinput.x-2
        y: textinput.y-1
    }

    FontLoader {
        id: arista_light
        source: "arista_light.ttf"
    }

    TextInput {
        id: textinput
        font.family: arista_light.name
        font.pixelSize: 22
        font.bold: true
        color: "#DDD"
        width: parent.width * 0.65
        maximumLength: 20
        autoScroll: true
        horizontalAlignment: TextInput.AlignLeft
        y: ((parent.height / 2) - (height / 2))
        x: parent.width * 0.35 - (parent.height-height) * 0.5
        selectedTextColor: "grey"
        readOnly: false
        validator: RegExpValidator{regExp: /[\w \-]+/}
    }

    onHover: {
        textinput.color = (mouseover||!enabled) ? "#111" : "#DDD"
    }
}
