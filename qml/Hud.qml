import QtQuick 1.1

Item {
    property int scoreValue: scoreController.score
    property int livesValue: gameController.LiveCount

    property real shadowOffset: 3
    property real shadowOffsetSmall: 1.5

    property string scorePrefix: "Score:"

    enabled: false

    function pad(number, length) {
        var str = '' + number;
        while (str.length < length) {
            str = '0' + str;
        }
        return str;
    }

    Connections {
        target: scoreController
        onScoreChanged: animate_score.start()
    }

    Connections {
        target: gameController
        onLiveCountChanged: animate_lives.start()
    }

    SequentialAnimation {
        id: animate_score
        running: false
        ColorAnimation { target: score; property: "color"; from: "#F41"; to: "#FFF"; duration: 200 }
    }

    SequentialAnimation {
        id: animate_lives
        running: false
        ColorAnimation { target: lives_text; property: "color"; from: "#FF102A"; to: "#FFF"; duration: 1000 }
    }

    FontLoader {
        id: monofonto
        source: "monofonto.ttf"
    }

    Text {
        id: score_shadow
        text: scorePrefix + pad(scoreValue, 6)
        font.family: monofonto.name
        font.pixelSize: 34
        color: "#333"
        x: parent.width - width - 10 + shadowOffset
        y: shadowOffset
    }

    Text {
        id: score
        text: scorePrefix + pad(scoreValue, 6)
        font.family: monofonto.name
        font.pixelSize: 34
        color: "#FFF"
        x: parent.width - width - 10
        y: 0
    }

    Item {
        anchors.horizontalCenter: parent.horizontalCenter

        width: lives_layout.width

        Grid {
            id: lives_layout
            columns: 2

            Image {
                id: lives_image
                source: "player_lives_hud.png"
            }
            Item {
                height: lives_image.height
                width: lives_text.width

                Text {
                    id: lives_text_shadow
                    text: "x" + pad(livesValue, 1)
                    font.family: monofonto.name
                    font.pixelSize: 26
                    color: "#333"
                    y: lives_image.height / 2 - lives_text.height / 2 + shadowOffsetSmall
                    x: shadowOffsetSmall
                }
                Text {
                    id: lives_text
                    text: "x" + pad(livesValue, 1)
                    font.family: monofonto.name
                    font.pixelSize: 26
                    color: "#FFF"
                    y: lives_image.height / 2 - lives_text.height / 2
                }
            }
        }
    }
}
