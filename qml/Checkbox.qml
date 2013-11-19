import QtQuick 1.1

Item {
    id: checkbox
    width: 32; height: 32
    property bool enabled: true
    property bool focusset: false

    Image {
        id: image
        anchors.centerIn: parent
        source: "unchecked" + ((focusset||!enabled)?'Highlighted':"") + ".png"
        states: [
            State { name: "Disabled"; when: !checkbox.enabled },
            State { name: "Pressed" },
            State { name: "Highlighted" }
        ]
        Image {
            id: imagecheck
            opacity:0
            anchors.centerIn: parent
            source: "checked" + ((focusset||!enabled)?'Highlighted':"") + ".png"
        }
    }
    /*
    MouseArea {
        id: mouse_area
        enabled: parent.enabled
        anchors.fill: parent
        property string checkboxState
        property url test
        // TODO: change to focus
        hoverEnabled: true
        onEntered: { image.state = 'Highlighted'; }
        onExited: { image.state = '' }
        onPressed: { image.state = '' }
        onReleased: {  checkbox.clicked() }
    }
    */
    states: [
        State {
            name: "Checked"
            PropertyChanges {
                target: imagecheck
                opacity: 1.0
            }
        }
    ]
    transitions: [
        Transition {
            from: ""; to: "Checked"
            NumberAnimation { properties: "opacity"; duration: 150 }
        },
        Transition {
            from: "Checked"; to: ""
            NumberAnimation { properties: "opacity"; duration: 150 }
        }
    ]
    function clicked() {
        image.state = '';
        if (checkbox.state == '') {
            checkbox.state = 'Checked'
        } else {
        checkbox.state = ''
        }
    }
}
