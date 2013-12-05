import QtQuick 1.1


Item{
    id: cutSceneItem

    opacity: 0

    width: parent.width
    height: parent.height * 0.25
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 10



    property string stringTitle
    property string stringMessage
    property string stringPortraitLeft
    property string stringPortraitRight
    property bool boolPortraitLeftVisible
    property bool boolPortraitRightVisible
    property int intDurationOfCutscene

    //I cant use both variables for some reason
    property int intDurationOfFadingIn
    property int intDurationOfFadingOut

    property int intFontSize


    Component.onCompleted: {
     fadeAnim.restart()
    }

    // stringTitle: "Frog"
    // stringMessage: "DO A BARREL ROLL! SPAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACE! I like turtles. Daniel is sleepy.  "


    SequentialAnimation {
        id: fadeAnim

        NumberAnimation {
            id: penis;
            target: cutSceneItem;
            property: "opacity";
            from: 0; to: 1;
            duration: 100;
        }

        PauseAnimation {
            duration:  intDurationOfCutscene;
        }

        NumberAnimation {
            id:pirat;
            target: cutSceneItem;
            property: "opacity";
            from: 1; to: 0;
            duration: intDurationOfFadingOut;
        }
    }

    Rectangle {
        id: recAvatarLeft
        color: "grey"
        opacity: 0.5
        anchors.right: message.left

        width: imageAvatarLeft.width + 20
        height: imageAvatarLeft.height + 20
    }
    Image {
        id: imageAvatarLeft

        width: cutSceneItem.height * 0.8

        smooth: true

        anchors.centerIn: recAvatarLeft;
        fillMode: Image.PreserveAspectFit
        source: stringPortraitLeft
    }



    Rectangle {
        id: recAvatarRight
        visible: cutSceneItem.boolPortraitRightVisible
        color: "grey"
        anchors.left: message.right

        opacity: 0.5

        width: imageAvatarRight.width + 20
        height: imageAvatarRight.height + 20

    }

    Image {
        id: imageAvatarRight

        width: cutSceneItem.height * 0.8
        smooth: true

        anchors.centerIn: recAvatarRight;


        fillMode: Image.PreserveAspectFit
        source: stringPortraitRight
    }



    Rectangle {

        id: message

        anchors.centerIn: parent;

        width: cutSceneItem.width - recAvatarLeft.width - recAvatarRight.width - 20
        height: cutSceneItem.height - 20

        color: "grey";
        opacity: 0.5


    }

    //TODO FontSize MinMax Size
    Item {

        width: message.width - 20
        height: message.height - 20

        anchors.centerIn: message

        Column  {
            Text {
                id: messageTitle

                text: cutSceneItem.stringTitle
                color: "#467fb4"
                font.family: "Calibri"
                font.pixelSize:  intFontSize + 3
                font.bold: true
            }

            Text {
                id: messageText

                width: message.width
                text: cutSceneItem.stringMessage
                wrapMode: Text.WordWrap
                color: "white"
                font.family: "Calibri"
                font.pixelSize:  intFontSize

            }



        }
    }
}
