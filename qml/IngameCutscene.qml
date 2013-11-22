import QtQuick 1.1


Item{
    id: cutSceneItem

    //y: parent.height - parent.height / 4
    width: parent.width
    height: parent.height * 0.25
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 10

    property string txtTitle
    property string txtMessage

    property int animSpeed
    animSpeed: 10;
    txtTitle: "Frog"
    txtMessage: "DO A BARREL ROLL! SPAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACE! I like turtles. Daniel is sleepy.  "





    /*  Rectangle and Image are Siblings, because of the opacity.
        Children are also affected from the opacity (Obvious)*/


    Rectangle {
        id: recAvatarLeft
        color: "grey"
        opacity: 0.5
        anchors.right: message.left

        width: cutSceneItem.width * 0.15
        height: cutSceneItem.height
    }
    Image {
        id: imageAvatarLeft

        width: recAvatarLeft.width - 20
        height: recAvatarLeft.height - 20
        smooth: true

        anchors.centerIn: recAvatarLeft;
        //fillMode: Image.PreserveAspectFit
        source: "avatar1.jpg"
    }



    Rectangle {
        id: recAvatarRight
        color: "grey"
        anchors.left: message.right

        opacity: 0.5

        width: cutSceneItem.width * 0.15
        height: cutSceneItem.height

    }

    Image {
        id: imageAvatarRight

        width: recAvatarLeft.width - 20
        height: recAvatarLeft.height - 20
        smooth: true

        anchors.centerIn: recAvatarRight;


        //fillMode: Image.PreserveAspectFit
        source: "avatar2.jpg"
    }



    Rectangle {

        id: message

        anchors.centerIn: parent;

        width: cutSceneItem.width - recAvatarLeft.width - recAvatarRight.width - 20
        height: cutSceneItem.height - 20

        color: "grey";
        opacity: 0.5


    }

    SequentialAnimation {
        id: messageAnim
        PropertyAnimation {
            target: messageText; properties: "x";
            from: messageText.width*-1; to: message.width; duration: cutSceneItem.animSpeed; easing.type: Easing.OutInExpo}
        PropertyAction { target: messageText; properties: "opacity"; value: 0 }
    }
    //TODO FontSize MinMax Size
    Item {

        width: message.width - 20
        height: message.height - 20

        anchors.centerIn: message

        Column  {
            Text {
                id: messageTitle

                text: cutSceneItem.txtTitle
                color: "#467fb4"
                font.family: "Calibri"
                font.pixelSize:  22
                font.bold: true
            }
            Text {
                id: messageText

                width: message.width
                text: cutSceneItem.txtMessage
                wrapMode: Text.WordWrap
                color: "white"
                font.family: "Calibri"
                font.pixelSize:  15

            }

        }
    }
}
