import QtQuick 1.1
import CustomComponents 1.0

Item {
    id: root
    property bool isDebugging: false
    property bool isIngame: false
    property bool isNavigationEnabled: true
    property int buttonW: 200
    property int buttonH: 40
    property int verticalSpacing: 15
    property string nextLevel: ""
    property string desiredResolution: ""
    property bool gameInTransition: false

    Connections {
        target: menuController
        onPrepareIngameMenu: {
            showMenu();
            menu_main.opacity = 0
            menu_main.scale = 0
            ingame_fade.visible = true
            root.isIngame = true
            root.state = "INGAME"
        }
        onPrepareGameoverMenu: {
            showMenu();
            menu_main.opacity = 0
            menu_main.scale = 0
            highscoreName.playerName = ""
            root.isIngame = false
            gameover_return_to_main.start()
        }
        onPrepareMapTransition: {
            nextLevel = lvl
            gameInTransition = true
            showMenu();
            menu_main.opacity = 0
            menu_main.scale = 0
            return_to_maptransition.start()
        }
    }
    Connections {
        target: rankingController
        onUpdateHighscore: {
            scorePlayer0.buttonText = "1. "+rankingController.getPlayer(0)
            scorePlayer0.rightText = rankingController.getScore(0)
            scorePlayer0.visible = rankingController.getScore(0) !== -1
            scorePlayer1.buttonText = "2. "+rankingController.getPlayer(1)
            scorePlayer1.rightText = rankingController.getScore(1)
            scorePlayer1.visible = rankingController.getScore(1) !== -1
            scorePlayer2.buttonText = "3. "+rankingController.getPlayer(2)
            scorePlayer2.rightText = rankingController.getScore(2)
            scorePlayer2.visible = rankingController.getScore(2) !== -1
            scorePlayer3.buttonText = "4. "+rankingController.getPlayer(3)
            scorePlayer3.rightText = rankingController.getScore(3)
            scorePlayer3.visible = rankingController.getScore(3) !== -1
            scorePlayer4.buttonText = "5. "+rankingController.getPlayer(4)
            scorePlayer4.rightText = rankingController.getScore(4)
            scorePlayer4.visible = rankingController.getScore(4) !== -1
        }
    }

    function startLevel(levelName) {
        root.state = "TRANSITIONLEVELINTRO";
        nextLevel = levelName
        menuController.LevelName = levelName;

        if (isDebugging)
        {
            // for testing
            hideMenu();
            menuController.onStartGame(nextLevel, true);
            menuController.onMenuFadedOut();
        }
        else
        {
            // uncomment for anim
            resetLevelTransition();
            text_level_name_fadein.start();
        }
    }

    function resetLevelTransition() {
        transition_game.opacity = 0
        cine_blend_top.x = -cine_blend_top.parent.width
        cine_blend_bottom.x = cine_blend_bottom.parent.width
        text_level_name.opacity = 0
        text_level_name.scale = 0.7
        transition_game.visible = true
    }

    function hideMenu() {
        menu_root.visible = false
    }

    function showMenu() {
        menu_root.visible = true
    }

    state: "MAIN"

    focus: true
    Keys.onDigit1Pressed: if (state === "MAIN") startLevel("dev_boss")
    //Keys.onDigit2Pressed: if (state === "MAIN") startLevel("dev_fighter_0")
    //Keys.onDigit3Pressed: if (state === "MAIN") startLevel("dev_fighter_1")
    //Keys.onDigit4Pressed: if (state === "MAIN") startLevel("dev_fighter_2")
    //Keys.onDigit5Pressed: if (state === "MAIN") startLevel("dev_fighter_3")
    Keys.onDigit6Pressed: if (state === "MAIN") startLevel("tutorial")
    Keys.onDigit7Pressed: if (state === "MAIN") startLevel("level_1")

    GameView {
        id: game
        anchors.fill: parent
    }

    Hud {
        visible: menuController.GameVisible
        anchors.fill: parent
        Keys.enabled: false
    }

    Item {
        id: menu_root
        width: parent.width
        height: parent.height
        Keys.enabled: false

        IngameBackground {
            id: ingame_fade
        }

        Item {

            x: (root.width * 0.5 - menuController.MouseX) * 0.04
            y: (root.height * 0.5 - menuController.MouseY) * 0.04
            width: parent.width
            height: parent.height

            Grid {
                id: menu_main
                opacity: 0
                scale: 0

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                columns: 1
                spacing: verticalSpacing

                Image {
                    id: title_image
                    source: "title.png"
                }
                Menubutton {
                    width: buttonW
                    height: buttonH
                    buttonText: "Play"
                    mouseEnabled: root.isNavigationEnabled
                    onClick: {
                        root.state = "CHAPTERS"
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Menubutton {
                    width: buttonW
                    height: buttonH
                    buttonText: "Highscores"
                    mouseEnabled: root.isNavigationEnabled
                    onClick: {
                        root.state = "SCORE"
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Menubutton {
                    width: buttonW
                    height: buttonH
                    buttonText: "Options"
                    mouseEnabled: root.isNavigationEnabled
                    onClick: {
                        desiredResolution = ""
                        root.state = "OPTIONS"
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Menubutton {
                    width: buttonW
                    height: buttonH
                    buttonText: "Exit"
                    mouseEnabled: root.isNavigationEnabled
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClick:  {
                        if (isDebugging)
                            Qt.quit()
                        else
                            exit_animation.start()
                    }
                }

                SequentialAnimation {
                    id: exit_animation
                    running: false
                    onCompleted: Qt.quit()

                    ScriptAction {
                        script: {
                            root.isNavigationEnabled = false
                            resetLevelTransition()
                            transition_game.visible = true
                            transition_game.opacity = 0
                        }
                    }
                    ParallelAnimation {
                        NumberAnimation { target: menu_main;
                            properties: "opacity, scale"; duration: 1000; easing.type: Easing.InOutBack; to: 0 }
                        NumberAnimation { target: transition_game; property: "opacity";
                            duration: 1000; easing.type: Easing.InQuad; to: 1 }
                    }
                }
            }

            Grid {
                id: menu_chapter
                opacity: 0
                scale: 0

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                columns: 1
                spacing: verticalSpacing

                Menubutton {
                    width: buttonW
                    height: buttonH
                    buttonText: "Select level"
                    mouseEnabled: false
                    anchors.horizontalCenter: parent.horizontalCenter
                    borderDefaultColor: "#FFF"
                    borderDefaultWidth: 2
                }
                Menubutton {
                    width: buttonW * 1.5
                    height: buttonH
                    buttonText: "Tutorial"
                    mouseEnabled: root.isNavigationEnabled
                    onClick: {
                        startLevel("tutorial")
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Menubutton {
                    width: buttonW * 1.5
                    height: buttonH
                    buttonText: "Level 1"
                    mouseEnabled: root.isNavigationEnabled
                    onClick: {
                        startLevel("level_1")
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Menubutton {
                    width: buttonW
                    height: buttonH
                    buttonText: "Back"
                    mouseEnabled: root.isNavigationEnabled
                    onClick: {
                        if (desiredResolution.length > 0)
                            menuController.onSetResolution(desiredResolution)
                        root.state = isIngame ? "INGAME" : "MAIN"
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            Grid {
                id: menu_options
                opacity: 0
                scale: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                columns: 1
                spacing: verticalSpacing
                property variant resOptions: ["640x480", "800x600", "1024x768", "1280x960", "1600x1200",
                    "852x480", "1280x720", "1600x900", "1920x1080"]

                Menuoptionsbox {
                    options: parent.resOptions
                    selected: parent.resOptions.indexOf( root.width + "x" + root.height )
                    enabled: !isIngame && !menuController.FullscreenEnabled
                    width: buttonW + 50
                    height: buttonH
                    anchors.horizontalCenter: parent.horizontalCenter
                    buttonText: "Resolution:"
                    onOptionChanged: {
                        desiredResolution = option
                    }
                }
                Menucheckbox {
                    width: buttonW + 50
                    height: buttonH
                    anchors.horizontalCenter: parent.horizontalCenter
                    buttonText: "Fullscreen:"
                    enabled: !isIngame
                    checked: menuController.FullscreenEnabled
                    onCheckChanged: {
                        menuController.FullscreenEnabled = enabled;
                    }
                }
                Menucheckbox {
                    width: buttonW + 50
                    height: buttonH
                    anchors.horizontalCenter: parent.horizontalCenter
                    buttonText: "High quality:"
                    checked: menuController.AAEnabled
                    onCheckChanged: {
                        menuController.AAEnabled = enabled;
                    }
                }
                Menucheckbox {
                    width: buttonW + 50
                    height: buttonH
                    anchors.horizontalCenter: parent.horizontalCenter
                    buttonText: "Enable music:"
                    checked: menuController.MusicEnabled
                    onCheckChanged: {
                        menuController.MusicEnabled = enabled;
                    }
                }
                Menucheckbox {
                    width: buttonW + 50
                    height: buttonH
                    anchors.horizontalCenter: parent.horizontalCenter
                    buttonText: "Show FPS:"
                    checked: menuController.FPSEnabled
                    onCheckChanged: {
                        menuController.FPSEnabled = enabled;
                    }
                }
                Menubutton {
                    width: buttonW
                    height: buttonH
                    buttonText: "Back"
                    mouseEnabled: root.isNavigationEnabled
                    onClick: {
                        if (desiredResolution.length > 0)
                            menuController.onSetResolution(desiredResolution)
                        root.state = isIngame ? "INGAME" : "MAIN"
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            Grid {
                id: menu_ingame
                opacity: 0
                scale: 0

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                columns: 1
                spacing: verticalSpacing

                Menubutton {
                    width: buttonW
                    height: buttonH
                    buttonText: "Return"
                    mouseEnabled: root.isNavigationEnabled
                    onClick: {
                        ingame_clearmenu.start()
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Menubutton {
                    width: buttonW
                    height: buttonH
                    buttonText: "Options"
                    mouseEnabled: root.isNavigationEnabled
                    onClick: {
                        desiredResolution = ""
                        root.state = "OPTIONSINGAME"
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Menubutton {
                    width: buttonW
                    height: buttonH
                    buttonText: "Back to menu"
                    anchors.horizontalCenter: parent.horizontalCenter
                    mouseEnabled: root.isNavigationEnabled
                    onClick:  {
                        ingame_return_to_main.start()
                    }
                }

                SequentialAnimation {
                    id: ingame_clearmenu
                    running: false

                    onCompleted: {
                        menuController.onMenuFadedOut()
                    }

                    ScriptAction {
                        script:{ root.state = "CLEAR"; root.isNavigationEnabled = false }
                    }
                    PauseAnimation { duration: 400; }
                }

                SequentialAnimation {
                    id: ingame_return_to_main
                    running: false

                    onCompleted: {
                        root.isIngame = false
                        transition_menu.visible = false
                    }

                    ScriptAction {
                        script: {
                            root.state = "CLEAR"
                            transition_menu.visible = true
                        }
                    }
                    NumberAnimation { target: transition_menu; property: "opacity";
                        duration: 400; easing.type: Easing.InOutQuad; to: 1 }
                    ScriptAction {
                        script: { menuController.onAbortGame();
                            menuController.onShowBackground();
                            ingame_fade.visible = false;
                            root.state = "MAIN"; }
                    }
                    NumberAnimation { target: transition_menu; property: "opacity";
                        duration: 400; easing.type: Easing.InOutQuad; to: 0 }
                }
            }

            Grid {
                id: menu_gameover
                opacity: 0
                scale: 0

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                columns: 1
                spacing: verticalSpacing

                Row {
                    Menubutton {
                        width: buttonW*2 + verticalSpacing
                        height: buttonH
                        buttonText: "Your Score: " + scoreController.score
                        mouseEnabled: false
                    }
                }

                Row {
                    Menutextfield {
                        id: highscoreName
                        width: buttonW*2 + verticalSpacing
                        height: buttonH
                        buttonText: "Your name:"
                    }
                }

                Row {
                    spacing: verticalSpacing

                    Menubutton {
                        width: buttonW
                        height: buttonH
                        buttonText: "Submit"
                        mouseEnabled: root.isNavigationEnabled
                        onClick: {

                            scoreController.name = highscoreName.getText()
                            rankingController.onSubmit()

                            root.state = "SCORE"
                            root.focus = true
                        }
                    }
                    Menubutton {
                        width: buttonW
                        height: buttonH
                        buttonText: "Back to menu"
                        mouseEnabled: root.isNavigationEnabled
                        onClick:  {
                            root.state = "MAIN"
                        }
                    }
                }
            }

            Grid {
                id: menu_score
                opacity: 0
                scale: 0

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                columns: 1
                spacing: verticalSpacing


                Menubutton {
                    width: buttonW
                    height: buttonH
                    buttonText: "Highscore"
                    mouseEnabled: false
                    anchors.horizontalCenter: parent.horizontalCenter
                    borderDefaultColor: "#FFF"
                    borderDefaultWidth: 2
                }
                Grid {
                    columns: 1
                    spacing: verticalSpacing / 2
                    Menutext {
                        id: scorePlayer0
                        width: buttonW*2
                        height: buttonH
                        mouseEnabled: false
                    }
                    Menutext {
                        id: scorePlayer1
                        width: buttonW*2
                        height: buttonH
                        mouseEnabled: false
                    }
                    Menutext {
                        id: scorePlayer2
                        width: buttonW*2
                        height: buttonH
                        mouseEnabled: false
                    }
                    Menutext {
                        id: scorePlayer3
                        width: buttonW*2
                        height: buttonH
                        mouseEnabled: false
                    }
                    Menutext {
                        id: scorePlayer4
                        width: buttonW*2
                        height: buttonH
                        mouseEnabled: false
                    }
                }

                    Menubutton {
                        width: buttonW
                        height: buttonH
                        buttonText: "Back to menu"
                        mouseEnabled: root.isNavigationEnabled
                        onClick:  {
                            root.state = "MAIN"
                        }
                        anchors.horizontalCenter: parent.horizontalCenter
                    }


            }
        }


        SequentialAnimation {
            id: gameover_return_to_main
            running: false

            onCompleted: {
                transition_menu.visible = false
            }

            ScriptAction {
                script: {
                    root.state = "CLEAR"
                    transition_menu.visible = true
                }
            }
            NumberAnimation { target: transition_menu; property: "opacity";
                duration: 400; easing.type: Easing.InOutQuad; to: 1 }
            ScriptAction {
                script: { menuController.onShowBackground();
                    ingame_fade.visible = false;
                    root.state = "GAMEOVER"; }
            }
            NumberAnimation { target: transition_menu; property: "opacity";
                duration: 400; easing.type: Easing.InOutQuad; to: 0 }
        }

        SequentialAnimation {
            id: return_to_maptransition
            running: false

            onCompleted: {
                transition_menu.visible = false
            }

            ScriptAction {
                script: {
                    root.state = "CLEAR"
                    transition_menu.visible = true
                }
            }
            ParallelAnimation {
                NumberAnimation { target: transition_menu; property: "opacity";
                    duration: 800; easing.type: Easing.InOutQuad; to: 1 }
                ScriptAction {
                    script: { startLevel(nextLevel) }
                }
            }
            PauseAnimation { duration: 200 }
            NumberAnimation { target: transition_menu; property: "opacity";
                duration: 400; easing.type: Easing.InOutQuad; to: 0 }
        }
    }

    Rectangle {
        id: transition_menu
        width: parent.width
        height: parent.height
        color: "#000"
        visible: false
        opacity: 0
    }

    Rectangle {
        id: transition_game
        width: parent.width
        height: parent.height
        color: "#000"
        visible: false
        opacity: 0

        Rectangle {
            id: cine_blend_top
            color: "#AAA"
            y: 0
            width: parent.width
            height: parent.height * 0.15
        }

        Rectangle {
            id: cine_blend_bottom
            color: "#AAA"
            y: parent.height - parent.height * 0.15
            width: parent.width
            height: parent.height * 0.15
        }

        Text {
            id: text_level_name
            text: "Starting on: " + menuController.LevelName
            font.pixelSize: 64
            color: "#FFF"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        SequentialAnimation {
            id: text_level_name_fadein
            running: false

            onCompleted: {
                hideMenu()
            }

            ScriptAction {
                script: menuController.onLevelIntro()
            }

            PauseAnimation { duration: 1200; }
            NumberAnimation { target: cine_blend_top; property: "x";
                duration: 150; easing.type: Easing.InOutQuad; to: 0 }
            PauseAnimation { duration: 30; }
            NumberAnimation { target: cine_blend_bottom; property: "x";
                duration: 150; easing.type: Easing.InOutQuad; to: 0 }
            PauseAnimation { duration: 30; }
            ParallelAnimation{
                NumberAnimation { target: text_level_name; property: "opacity";
                    duration: 500; easing.type: Easing.InOutQuad; to: 1 }
                NumberAnimation { target: text_level_name; property: "scale";
                    duration: 500; easing.type: Easing.InOutQuad; to: 1 }
            }
            PauseAnimation { duration: 1750; }
            ParallelAnimation{
                NumberAnimation { target: text_level_name; property: "opacity";
                    duration: 600; easing.type: Easing.InOutQuad; to: 0 }
                NumberAnimation { target: text_level_name; property: "scale";
                    duration: 700; easing.type: Easing.InOutQuad; to: 0.7 }
                SequentialAnimation {

                    PauseAnimation { duration: 300; }
                    NumberAnimation { target: cine_blend_top; property: "x";
                        duration: 150; easing.type: Easing.InOutQuad; to: transition_game.width }
                    PauseAnimation { duration: 30; }
                    NumberAnimation { target: cine_blend_bottom; property: "x";
                        duration: 150; easing.type: Easing.InOutQuad; to: -transition_game.width }
                    PauseAnimation { duration: 30; }
                }
            }
            ScriptAction {
                script: {
                    menuController.onStartGame(nextLevel, gameInTransition == false);
                    gameInTransition = false;
                }
            }
            NumberAnimation { target: transition_game; property: "opacity";
                duration: 600; easing.type: Easing.InOutQuad; to: 0 }
            ScriptAction {
                script: menuController.onMenuFadedOut()
            }
        }
    }



    states: [
        State {
            name: "MAIN"
            PropertyChanges {
                target: menu_main; scale: 1; opacity: 1
            }
        },
        State {
            name: "INGAME"
            PropertyChanges {
                target: menu_ingame; scale: 1; opacity: 1
            }
            PropertyChanges {
                target: ingame_fade; opacity: 0.4
            }
        },
        State {
            name: "GAMEOVER"
            PropertyChanges {
                target: menu_gameover; scale: 1; opacity: 1
            }
        },
        State {
            name: "CHAPTERS"
            PropertyChanges {
                target: menu_chapter; scale: 1; opacity: 1
            }
        },
        State {
            name: "SCORE"
            PropertyChanges {
                target: menu_score; scale: 1; opacity: 1
            }
        },
        State {
            name: "OPTIONS"
            PropertyChanges {
                target: menu_options; scale: 1; opacity: 1
            }
        },
        State {
            name: "OPTIONSINGAME"
            PropertyChanges {
                target: menu_options; scale: 1; opacity: 1
            }
            PropertyChanges {
                target: ingame_fade; opacity: 0.4
            }
        },
        State {
            name: "TRANSITIONLEVELINTRO"
            PropertyChanges {
                target: transition_game; opacity: 1
            }
        },
        State {
            name: "CLEAR"
            PropertyChanges {
                target: ingame_fade; opacity: 0
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "*"

            SequentialAnimation {
                ScriptAction {
                    script: root.isNavigationEnabled = false
                }
                NumberAnimation { targets: [menu_main, menu_options, menu_ingame, ingame_fade, menu_gameover, menu_score, menu_chapter];
                    properties: "opacity,scale"; duration: 400; easing.type: Easing.InOutBack }
                ScriptAction {
                    script: root.isNavigationEnabled = true
                }
            }
        },
        Transition {
            from: "MAIN,CHAPTERS,CLEAR"
            to: "TRANSITIONLEVELINTRO"
            SequentialAnimation {
                ScriptAction {
                    script: root.isNavigationEnabled = false
                }
                NumberAnimation { targets: [transition_game, menu_main, menu_chapter];
                    properties: "opacity,scale"; duration: 1000; easing.type: Easing.InCubic }
                ScriptAction {
                    script: root.isNavigationEnabled = true
                }
            }
        }
    ]
}
