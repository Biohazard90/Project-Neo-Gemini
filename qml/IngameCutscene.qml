import QtQuick 1.1


Item{
    id: cutSceneItem

    opacity: 0
    //y: parent.height - parent.height / 4
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



    //TODO Why can I not pass the duration value. WHY?
    //TODO It shows only one cutscene :\
Component.onCompleted: {
    //fadeAnimDuration.duration =  stringMessage.length * 2000
    fadeAnimDuration.duration =  10000
    ; fadeAnim.start()}

    property int animSpeed
    animSpeed: 10;
    // stringTitle: "Frog"
    // stringMessage: "DO A BARREL ROLL! SPAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACE! I like turtles. Daniel is sleepy.  "



//    Timer {
//        id: cutsceneTimer
//        interval: stringMessage.length * 500; running: true;
//        onTriggered: cutSceneFadeOut.restart();

//    }

    SequentialAnimation {
        id: fadeAnim

        NumberAnimation {target: cutSceneItem; property: "opacity"; from: 0; to: 1; duration: 1000 }
        PauseAnimation {id:fadeAnimDuration;}
        NumberAnimation {target: cutSceneItem; property: "opacity"; from: 1; to: 0; duration: 1000 }

    }

//    NumberAnimation on opacity{
//        id: cutSceneFadeIn
//        from: 0
//        to: 1
//        duration: 2000
//        easing.type: Easing.InOutQuad
//    }

//    ParallelAnimation {
//        id: cutSceneFadeOut
//        running: false
//        NumberAnimation{
//            target: cutSceneItem
//            property: "opacity"
//            to: 0.5
//            duration: 400
//            easing.type: Easing.InOutQuad

//        }
        //        NumberAnimation{
        //            target: cutSceneItem
        //            property: "y"
        //            to: 1
        //            duration: 1000
        //            //   easing.type: Easing.InOutQuad
        //        }
    //}

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
                font.pixelSize:  22
                font.bold: true
            }

            Text {
                id: messageText

                width: message.width
                text: cutSceneItem.stringMessage
                wrapMode: Text.WordWrap
                color: "white"
                font.family: "Calibri"
                font.pixelSize:  15

            }



        }
    }
}
