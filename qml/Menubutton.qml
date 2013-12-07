import QtQuick 1.1

Item {
    id: buttonRoot
    width: 75; height: 75

    property string buttonText
    property int animDuration: 100
    property bool leftAlign: false
    signal click(bool rightlick)
    signal hover(bool mouseover)
    property int acceptedButtons: Qt.LeftButton
    property bool mouseEnabled: true
    property bool setUnderline: false
    property string borderDefaultColor: "#000"
    property int borderDefaultWidth: 1

    onMouseEnabledChanged: { if (mouseEnabled == false) button.state = "RELEASED"; }

    Rectangle
    {
     id: button
     state: enabled ? "RELEASED" : "DISABLED"
     width: parent.width
     height: parent.height
     radius: 7
     border.width: borderDefaultWidth
     smooth: true

     MouseArea {
         id: mouse_area_button
         anchors.fill: parent
         hoverEnabled: true
         enabled: mouseEnabled
         acceptedButtons: buttonRoot.acceptedButtons
         onPressed: button.state = "PRESSED"
         onReleased: {
             if (containsMouse)
             {
                button.state = "HOVER"
                 click(mouse.button === Qt.RightButton)
                 menuController.onButtonClick()
             }
             else
                button.state = "RELEASED"
         }
         onEntered: {
             button.state = "HOVER"
             menuController.onButtonHover(mouse_area_button, true)
         }
         onExited: {
             button.state = "RELEASED"
             menuController.onButtonHover(mouse_area_button, false)
         }
         onHoveredChanged: hover(containsMouse)
     }

     states: [
         State {
             name: "PRESSED"
             PropertyChanges {
                 target: button; color: "#CDF"; opacity: 0.8; border.color: "#DEF"; border.width: 2
             }
             PropertyChanges {
                 target: textField; color: "#111"
             }
         },
         State {
             name: "RELEASED"
             PropertyChanges {
                 target: button; color: "#111"; opacity: 0.5; border.color: borderDefaultColor
             }
             PropertyChanges {
                 target: textField; color: "#DDD"
             }
         },
         State {
             name: "HOVER"
             PropertyChanges {
                 target: button; color: "#7BD"; opacity: 0.8; border.color: "#125"
             }
             PropertyChanges {
                 target: textField; color: "#111"
             }
         },
         State {
             name: "DISABLED"
             PropertyChanges {
                 target: button; color: "#DDD"; opacity: 0.5; border.color: "#000"
             }
             PropertyChanges {
                 target: textField; color: "#444"
             }
         }
     ]

     transitions: [
         Transition {
             from: "*"
             to: "*"
             ColorAnimation { target: button; duration: animDuration}
             ColorAnimation { target: textField; duration: animDuration}
         }
     ]
    }

    FontLoader {
        id: arista_light
        source: "arista_light.ttf"
    }

    Text {
        id: textField
        anchors.centerIn: parent
        text: buttonText
        font.underline: setUnderline
        font.family: arista_light.name
        font.pixelSize: 22
        horizontalAlignment: leftAlign ? Text.AlignLeft : Text.AlignHCenter
        width: leftAlign ? (parent.width - parent.height * 0.6) : parent.width
    }

}
