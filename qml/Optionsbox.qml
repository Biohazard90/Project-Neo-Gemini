import QtQuick 1.1

Item {
    property variant options: ["option 1", "option 2 long"]
    property int selected: 0
    property string textColor: enabled ? "#DDD" : "#111"

    signal optionChanged(string option)

    function nextOption() {
        if (selected < options.length - 1)
            selected++;
        else
            selected = 0;

        optionChanged(options[selected])
    }

    function previousOption() {
        if (selected === 0)
            selected = options.length - 1;
        else
            selected--;

        optionChanged(options[selected])
    }

    id: optionsbox
    width: textField.width
    height: textField.height

    FontLoader {
        id: arista_light
        source: "arista_light.ttf"
    }

    Text {
        id: textField
        anchors.centerIn: parent
        text: options[selected]
        font.family: arista_light.name
        font.pixelSize: 22
        horizontalAlignment: Text.AlignRight
        color: textColor
    }
}
