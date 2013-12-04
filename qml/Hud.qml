import QtQuick 1.1
import Qt.labs.shaders 1.0

Item {
    id: hudRoot
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
        //onLiveCountChanged: animate_lives.start()
        onPlayerHealthChanged: playerHealthChangedCallback();
        onWarningText: playWarningAnim();
    }

    SequentialAnimation {
        id: animate_score
        running: false
        ParallelAnimation {
            NumberAnimation { target: blurPass0;
                properties: "blurWidth"; duration: 200; easing.type: Easing.InOutBack; from: 0.0065 ; to: 0.005 }
            NumberAnimation { target: blurPass1;
                properties: "blurHeight"; duration: 200; easing.type: Easing.InOutBack; from: 0.022 ; to: 0.015 }
        ColorAnimation { target: score; property: "color"; from: "#9BF"; to: "#FFF"; duration: 200 }
        }
    }

   // SequentialAnimation {
     //   id: animate_lives
     //   running: false
        //ColorAnimation { target: lives_text; property: "color"; from: "#FF102A"; to: "#FFF"; duration: 1000 }
  //  }

    FontLoader {
        id: monofonto
        source: "monofonto.ttf"
    }

    FontLoader {
        id: airstrike
        source: "airstrike.ttf"
    }

    Text {
        id: score_shadow
        visible : false
        text: scorePrefix + pad(scoreValue, 6)
        font.family: monofonto.name
        font.pixelSize: 34
        color: "#333"
        x: parent.width - width - 10 + shadowOffset
        y: shadowOffset
    }

    Item {
        id: scoreBlurContainer
        x: parent.width - width
        y: 0
        width: scoreBlur.width + 20
        height: scoreBlur.height + 20

        Text {
            id: scoreBlur
            text: scorePrefix + pad(scoreValue, 6)
            font.family: monofonto.name
            font.pixelSize: 34
            color: "#3AF"
            x: 10
            y: 10
        }
    }

    ShaderEffectItem {
        id: blurPass0
         property variant source: ShaderEffectSource { sourceItem: scoreBlurContainer; hideSource: true }
         anchors.fill: scoreBlurContainer
         property real blurWidth: 1.0 / width

         fragmentShader: "
         #version 120
         varying highp vec2 qt_TexCoord0;
         uniform highp float blurWidth;
         uniform sampler2D source;
         const float[7] weights7 = float[7]( 0.023977, 0.097843, 0.227491, 0.301377, 0.227491, 0.097843, 0.023977 );
                const float[13] weights13 = float[13](0.006299, 0.017298, 0.039533, 0.075189, 0.119007, 0.156756, 0.171834,
                0.156756, 0.119007, 0.075189, 0.039533, 0.017298, 0.006299);
        const float[21] weights21 = float[21]( 0.002650, 0.005372, 0.010109, 0.017660, 0.028639, 0.043115, 0.060254,
            0.078171, 0.094147, 0.105259, 0.109247, 0.105259, 0.094147, 0.078171, 0.060254, 0.043115, 0.028639, 0.017660,
            0.010109, 0.005372, 0.002650 );
         void main(void)
         {
                vec4 color = vec4(0, 0, 0, 0);
                vec2 uv = qt_TexCoord0 - vec2(blurWidth * 10.0f, 0.0f);
                for (int i = 0; i < 21; i++)
                {
                    color += texture2D(source, uv) * weights21[i];
                    uv.x += blurWidth;
                }
                gl_FragColor = color;
         }
         "
     }

    ShaderEffectItem {
        id: blurPass1
         property variant source: ShaderEffectSource { sourceItem: blurPass0; hideSource: true }
         anchors.fill: blurPass0
         property real blurHeight: 1.0 / height

         fragmentShader: "
         varying highp vec2 qt_TexCoord0;
         uniform sampler2D source;
         uniform highp float blurHeight;
         const float[7] weights7 = float[7]( 0.023977, 0.097843, 0.227491, 0.301377, 0.227491, 0.097843, 0.023977 );
                const float[13] weights13 = float[13](0.006299, 0.017298, 0.039533, 0.075189, 0.119007, 0.156756, 0.171834,
                0.156756, 0.119007, 0.075189, 0.039533, 0.017298, 0.006299);
        const float[21] weights21 = float[21]( 0.002650, 0.005372, 0.010109, 0.017660, 0.028639, 0.043115, 0.060254,
            0.078171, 0.094147, 0.105259, 0.109247, 0.105259, 0.094147, 0.078171, 0.060254, 0.043115, 0.028639, 0.017660,
            0.010109, 0.005372, 0.002650 );
         void main(void)
         {
                vec4 color = vec4(0, 0, 0, 0);
                vec2 uv = qt_TexCoord0 - vec2(0.0f, blurHeight * 10.0f);
                for (int i = 0; i < 21; i++)
                {
                    color += texture2D(source, uv) * weights21[i];
                    uv.y += blurHeight;
                }
                gl_FragColor = color * 2;
         }
         "
     }

    Text {
        id: score
        text: scorePrefix + pad(scoreValue, 6)
        font.family: monofonto.name
        font.pixelSize: 34
        color: "#BDF"
        x: parent.width - width - 10
        y: 10
        visible: true
    }

    Image {
        id: shieldImage
        source: "shield_hud.png"
    }

    Image {
        id: scanlinesImage
        source: "scanlines.png"
    }

    property int lastShield: 0

    function playerHealthChangedCallback() {
        var isIncreasing = (gameController.PlayerHealth > lastShield) ? true : false;

        if (isIncreasing)
            animate_shield_warning.stop();

        while (gameController.PlayerHealth != lastShield) {

            if (!isIncreasing)
                lastShield--;

            switch (lastShield) {
            case 3:
                if (isIncreasing)
                {
                    animate_shield_2_down.stop();
                    animate_shield_2_up.restart();
                }
                else
                {
                    animate_shield_2_up.stop();
                    animate_shield_2_down.restart();
                }
                break;
            case 2:
                if (isIncreasing)
                {
                    animate_shield_1_down.stop();
                    animate_shield_1_up.restart();
                }
                else
                {
                    animate_shield_1_up.stop();
                    animate_shield_1_down.restart();
                }
                break;
            case 1:
                if (isIncreasing)
                {
                    animate_shield_0_down.stop();
                    animate_shield_0_up.restart();
                }
                else
                {
                    animate_shield_0_up.stop();
                    animate_shield_0_down.restart();
                    animate_shield_warning.restart();
                }
                break;
            }

            if (isIncreasing)
                lastShield++;
        }
    }

    NumberAnimation { id: animate_shield_2_down; target: shieldShader2; property: "darken";
        easing.amplitude: 5; from: 0; to: 1; duration: 400; easing.type: Easing.InOutBounce; running: false  }

    NumberAnimation { id: animate_shield_2_up; running: false; target: shieldShader2;
        property: "darken"; from: 1; to: 0; duration: 400; easing.type: Easing.InQuad }

    NumberAnimation { id: animate_shield_1_down; running: false; target: shieldShader1; property: "darken";
        easing.amplitude: 5; from: 0; to: 1; duration: 400; easing.type: Easing.InOutBounce  }

    NumberAnimation { id: animate_shield_1_up; running: false; target: shieldShader1;
        property: "darken"; from: 1; to: 0; duration: 400; easing.type: Easing.InQuad }

    NumberAnimation { id: animate_shield_0_down; running: false; target: shieldShader0; property: "darken";
        easing.amplitude: 5; from: 0; to: 1; duration: 400; easing.type: Easing.InOutBounce  }

    NumberAnimation { id: animate_shield_0_up; running: false; target: shieldShader0;
        property: "darken"; from: 1; to: 0; duration: 400; easing.type: Easing.InQuad }

    SequentialAnimation {
        id: animate_shield_warning
        running: false
        PauseAnimation { duration: 550 }
        NumberAnimation { target: hudRoot; property: "shieldIconsRedFlash"; easing.amplitude: 4; duration: 300
            easing.type: Easing.InOutSine; from: 0.0; to: 1.0 }
        NumberAnimation { target: hudRoot; property: "shieldIconsRedFlash"; easing.amplitude: 4; duration: 300
            easing.type: Easing.InOutSine; from: 1.0; to: 0.0 }
        NumberAnimation { target: hudRoot; property: "shieldIconsRedFlash"; easing.amplitude: 4; duration: 300
            easing.type: Easing.InOutSine; from: 0.0; to: 1.0 }
        NumberAnimation { target: hudRoot; property: "shieldIconsRedFlash"; easing.amplitude: 4; duration: 300
            easing.type: Easing.InOutSine; from: 1.0; to: 0.0 }
        NumberAnimation { target: hudRoot; property: "shieldIconsRedFlash"; easing.amplitude: 4; duration: 300
            easing.type: Easing.InOutSine; from: 0.0; to: 1.0 }
        NumberAnimation { target: hudRoot; property: "shieldIconsRedFlash"; easing.amplitude: 4; duration: 300
            easing.type: Easing.InOutSine; from: 1.0; to: 0.0 }
    }

    property real scanlinesTime: 0
    property real shieldIconsRedFlash: 0

    Timer {
        id: scanlinesTimer
         interval: 33; running: true; repeat: true
         onTriggered: scanlinesTime += 0.033
     }

    property string shieldShaderCode: "
         varying highp vec2 qt_TexCoord0;
         uniform sampler2D source;
         uniform sampler2D scanlinesSource;
            uniform highp float darken;
            uniform highp float redflash;
            uniform highp float time;
        uniform highp float scanlineOffset;
         void main(void)
         {
            float scanlines = texture2D(scanlinesSource, qt_TexCoord0 * vec2(0.1, 0.15)
                + vec2(0, time * 6) + vec2(scanlineOffset, 0)).r;
            vec4 color = texture2D(source, qt_TexCoord0);

            scanlines *= sin(time * 50) * 0.1 + 0.9;

            color.a = color.g * mix(scanlines * 3.5, 1.0, min(1.0, darken + 0.8 * pow(1.0 - pow(color.b, 0.5), 4.0)));

            color.rgb = mix(vec3(0.5, 0.7, 1.0), vec3(1,1,1), color.r * color.r);
            color.rgb = mix(color.rgb, vec3(0.2, 0.27, 0.3), darken);
            color.rgb = mix(color.rgb, vec3(1.0, 0.25, 0.05), redflash);

            color.rgb *= color.a;
            gl_FragColor = color;
         }
         "

    ShaderEffectItem {
        id: shieldShader0
         property variant source: ShaderEffectSource { sourceItem: shieldImage; hideSource: true }
        property variant scanlinesSource: ShaderEffectSource { sourceItem: scanlinesImage; hideSource: true; wrapMode: ShaderEffectSource.Repeat }
        property real darken: 0.0
        property real redflash: shieldIconsRedFlash
        property real time: scanlinesTime
        property real scanlineOffset: 0
         x: score.x
         y: score.y + score.height - 5
         width: shieldImage.width
         height: shieldImage.height
         blending: true

         fragmentShader: shieldShaderCode
     }

     ShaderEffectItem {
        id: shieldShader1
         property variant source: ShaderEffectSource { sourceItem: shieldImage; hideSource: true }
        property variant scanlinesSource: ShaderEffectSource { sourceItem: scanlinesImage; hideSource: true; wrapMode: ShaderEffectSource.Repeat }
        property real darken: 0.0
        property real redflash: shieldIconsRedFlash
        property real time: scanlinesTime
        property real scanlineOffset: 0.2
         x: score.x + 20
         y: score.y + score.height - 5
         width: shieldImage.width
         height: shieldImage.height
         blending: true

         fragmentShader: shieldShaderCode
     }

     ShaderEffectItem {
        id: shieldShader2
         property variant source: ShaderEffectSource { sourceItem: shieldImage; hideSource: true }
        property variant scanlinesSource: ShaderEffectSource { sourceItem: scanlinesImage; hideSource: true; wrapMode: ShaderEffectSource.Repeat }
        property real darken: 0.0
        property real redflash: shieldIconsRedFlash
        property real time: scanlinesTime
        property real scanlineOffset: 0.4
         x: score.x + 40
         y: score.y + score.height - 5
         width: shieldImage.width
         height: shieldImage.height
         blending: true

         fragmentShader: shieldShaderCode
     }

    //IngameCutscene {

   // }

     function playWarningAnim() {
        for (var i = 0; i < warningRepeater.count; i++) {
            warningRepeater.itemAt(i).fadeIn((warningRepeater.count - i - 1) * 200 + 500);
        }
        warningRoot.visible = true;
        lineAnimation.restart();
     }

     Row {
         id: warningRoot
         anchors.centerIn: parent
         visible: false
         opacity: 0.5
         property int warningRectWidth: hudRoot.width // parent.height * 0.2 * 7
         property int warningRectMove: hudRoot.width

         Column {

             SequentialAnimation {
                    id: lineAnimation
                    ScriptAction {
                        script: { border_top.x = -warningRoot.warningRectMove; border_bottom.x = -warningRoot.warningRectMove; }
                    }
                    PauseAnimation { duration: 1200 }
                    ParallelAnimation {
                        NumberAnimation { target: border_top; property: "x"; duration: 600; easing.type: Easing.InOutQuad;
                            from: -warningRoot.warningRectMove; to: 0 }
                        SequentialAnimation {
                            PauseAnimation { duration: 450 }
                            NumberAnimation { target: border_bottom; property: "x"; duration: 600; easing.type: Easing.InOutQuad;
                                from: -warningRoot.warningRectMove; to: 0 }
                        }
                    }
                    PauseAnimation { duration: 1800 }
                    ParallelAnimation {
                        NumberAnimation { target: border_top; property: "x"; duration: 600; easing.type: Easing.InOutQuad;
                            to: warningRoot.warningRectMove; from: 0 }
                        SequentialAnimation {
                            PauseAnimation { duration: 450 }
                            NumberAnimation { target: border_bottom; property: "x"; duration: 600; easing.type: Easing.InOutQuad;
                                to: warningRoot.warningRectMove; from: 0 }
                        }
                    }
             }

             Rectangle {
                 id: border_top
                 color: "#F21"
                 width: warningRoot.warningRectWidth
                 height: warningRoot.parent.height * 0.03
                 x: 0
                 y: 0
             }

             Rectangle {
                 visible: false
                 height: warningRoot.parent.height * 0.06
             }

             Row {
                 anchors.horizontalCenter: parent.horizontalCenter

                 Repeater {
                     id: warningRepeater
                        model: ["W", "A", "R", "N", "I", "N", "G"]
                        Item {
                            id: objRoot

                            function fadeIn(delay) {
                                objDelay.interval = delay;
                                objDelay.restart();
                                objColorAnim.restart();
                                objContainer.opacity = 0.0;
                            }

                            width: objContainer.width
                            height: objContainer.height

                            Timer {
                                id: objDelay
                                interval: 0
                                repeat: false
                                onTriggered: {
                                    objAnim.restart();
                                    objAnim2.restart();
                                    objAnim3.restart();
                                }
                            }

                            Item {
                                id: objContainer
                                opacity: 0.0
                                height: warningRoot.parent.height * 0.2
                                width: height

                                Text {
                                    id: obj
                                    text: modelData
                                    font.family: airstrike.name
                                    font.pixelSize: parent.height
                                    color: "#F21"
                                    anchors.centerIn: parent
                                }
                            }
                            NumberAnimation { id: objAnim; target: objContainer; running: false; property: "opacity"; from: 0.0; to: 1.0; duration: 500; easing.type: Easing.InOutQuad }
                            NumberAnimation { id: objAnim2; target: objContainer; running: false; property: "x"; from: -warningRoot.width; to: 0.0; duration: 500; easing.type: Easing.InOutQuad }
                            NumberAnimation { id: objAnim3; target: obj; running: false; property: "font.pixelSize";
                                from: 0; to: objContainer.height; duration: 500; easing.type: Easing.InOutQuad }
                            SequentialAnimation {
                                id: objColorAnim;
                                running: false;
                                PauseAnimation { duration: 2500 }
                                ColorAnimation { target: obj; property: "color";  from: "#F21"; to: "#FD1"; duration: 300; easing.type: Easing.InOutQuad }
                                ColorAnimation { target: obj; property: "color";  to: "#F21"; from: "#FD1"; duration: 300; easing.type: Easing.InOutQuad }
                                ColorAnimation { target: obj; property: "color";  from: "#F21"; to: "#FD1"; duration: 300; easing.type: Easing.InOutQuad }
                                ColorAnimation { target: obj; property: "color";  to: "#F21"; from: "#FD1"; duration: 300; easing.type: Easing.InOutQuad }
                                ColorAnimation { target: obj; property: "color";  from: "#F21"; to: "#FD1"; duration: 300; easing.type: Easing.InOutQuad }
                                ColorAnimation { target: obj; property: "color";  to: "#F21"; from: "#FD1"; duration: 300; easing.type: Easing.InOutQuad }
                                PauseAnimation { duration: (6 - index) * 100 }
                                NumberAnimation { target: objContainer; running: false; property: "x"; easing.overshoot: 2.752;
                                    to: warningRoot.parent.width; from: 0.0; duration: 1200; easing.type: Easing.InBack }
                                ScriptAction {
                                    script: if (index == 0) warningRoot.visible = false;
                                }
                            }
                        }
                   }
             }

             Rectangle {
                 visible: false
                 height: warningRoot.parent.height * 0.06
             }

             Rectangle {
                 id: border_bottom
                 color: "#F21"
                 width: warningRoot.warningRectWidth
                 height: warningRoot.parent.height * 0.03
                 x: 0
                 y: 0
             }
         }
     }
}
