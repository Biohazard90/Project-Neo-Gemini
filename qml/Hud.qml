import QtQuick 1.1
import Qt.labs.shaders 1.0

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
        onPlayerHealthChanged: playerHealthChangedCallback()
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
         property real blurWidth: 0.005

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
         property real blurHeight: 0.015

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
                }
                break;
            }

            if (isIncreasing)
                lastShield++;
        }
    }

    SequentialAnimation {
        id: animate_shield_2_down
        running: false
        NumberAnimation { target: shieldShader2; property: "darken"; easing.amplitude: 5; from: 0; to: 1; duration: 400; easing.type: Easing.InOutBounce  }
    }

    SequentialAnimation {
        id: animate_shield_2_up
        running: false
        NumberAnimation { target: shieldShader2; property: "darken"; from: 1; to: 0; duration: 400; easing.type: Easing.InQuad }
    }

    SequentialAnimation {
        id: animate_shield_1_down
        running: false
        NumberAnimation { target: shieldShader1; property: "darken"; easing.amplitude: 5; from: 0; to: 1; duration: 400; easing.type: Easing.InOutBounce  }
    }

    SequentialAnimation {
        id: animate_shield_1_up
        running: false
        NumberAnimation { target: shieldShader1; property: "darken"; from: 1; to: 0; duration: 400; easing.type: Easing.InQuad }
    }

    SequentialAnimation {
        id: animate_shield_0_down
        running: false
        NumberAnimation { target: shieldShader0; property: "darken"; easing.amplitude: 5; from: 0; to: 1; duration: 400; easing.type: Easing.InOutBounce  }
    }

    SequentialAnimation {
        id: animate_shield_0_up
        running: false
        NumberAnimation { target: shieldShader0; property: "darken"; from: 1; to: 0; duration: 400; easing.type: Easing.InQuad }
    }

    property real scanlinesTime: 0
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
            uniform highp float time;
        uniform highp float scanlineOffset;
         void main(void)
         {
            vec4 scanlines = texture2D(scanlinesSource, qt_TexCoord0 * vec2(0.1, 0.15)
                + vec2(0, time * 4) + vec2(scanlineOffset, 0));
            vec4 color = texture2D(source, qt_TexCoord0);

            color.a = color.g * mix(scanlines.r * 2.5, 1.0, min(1.0, darken + pow(1.0 - color.b, 4.0)));

            color.rgb = mix(vec3(0.5, 0.7, 1.0), vec3(1,1,1), color.r * color.r);
            color.rgb = mix(color.rgb, vec3(0.2, 0.27, 0.3), darken);

            color.rgb *= color.a;
            gl_FragColor = color;
         }
         "

    ShaderEffectItem {
        id: shieldShader0
         property variant source: ShaderEffectSource { sourceItem: shieldImage; hideSource: true }
        property variant scanlinesSource: ShaderEffectSource { sourceItem: scanlinesImage; hideSource: true; wrapMode: ShaderEffectSource.Repeat }
        property real darken: 0.0
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
        property real time: scanlinesTime
        property real scanlineOffset: 0.4
         x: score.x + 40
         y: score.y + score.height - 5
         width: shieldImage.width
         height: shieldImage.height
         blending: true

         fragmentShader: shieldShaderCode
     }

   /* Item {
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
    */

    //IngameCutscene {

   // }
}
