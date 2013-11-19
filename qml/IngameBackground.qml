import QtQuick 1.1

Rectangle {
    width: parent.width
    height: parent.height
    color: "#457"
    visible: false
    opacity: 0

    Rectangle {
        width: parent.width
        height: parent.height * 0.5
        opacity: 0.5
        gradient: Gradient {
            GradientStop {
                position: 0.5;
                color: "#345";
            }
            GradientStop {
                position: 0.00;
                color: "#000";
            }
        }
    }
    Rectangle {
        y: parent.height * 0.5
        width: parent.width
        height: parent.height * 0.5
        opacity: 0.5
        gradient: Gradient {
            GradientStop {
                position: 0.5;
                color: "#345";
            }
            GradientStop {
                position: 1.00;
                color: "#000";
            }
        }
    }
    Rectangle {
        x: (parent.width * 0.5 - parent.height) * 0.5
        y: (parent.height - parent.width * 0.5) * 0.5
        width: parent.height
        height: parent.width * 0.5
        rotation: 90
        opacity: 0.5
        gradient: Gradient {
            GradientStop {
                position: 0.5;
                color: "#345";
            }
            GradientStop {
                position: 1.00;
                color: "#000";
            }
        }
    }
    Rectangle {
        x: (parent.width * 0.5 - parent.height) * 0.5 + parent.width * 0.5
        y: (parent.height - parent.width * 0.5) * 0.5
        width: parent.height
        height: parent.width * 0.5
        rotation: -90
        opacity: 0.5
        gradient: Gradient {
            GradientStop {
                position: 0.5;
                color: "#345";
            }
            GradientStop {
                position: 1.00;
                color: "#000";
            }
        }
    }
}
