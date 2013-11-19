import QtQuick 1.1

Menubutton {

    property bool checked: false
    signal checkChanged( bool enabled )
    signal optionChanged(string option)
    property variant options: ["option 1", "option 2"]
    property int selected: 0

    leftAlign: true
    acceptedButtons: Qt.LeftButton | Qt.RightButton

    Optionsbox {
        options: parent.options
        selected: parent.selected
        state: checked ? 'Checked':''
        id: optionsbox
        x: parent.width - width - (parent.height - height) * 0.5
        y:  (parent.height - height) * 0.5
        onOptionChanged: {
            parent.optionChanged(option)
        }
    }

    onClick: {
        if (rightlick)
            optionsbox.previousOption()
        else
            optionsbox.nextOption()
    }

    onHover: {
        optionsbox.textColor = (mouseover||!enabled) ? "#111" : "#DDD"
    }

}
