import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: root
    width: 1200
    height: 800
    color: "#252525"

    property var qtai: QTAI

    Connections {
        target: qtai
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Top bar
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "#2d2d2d"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                anchors.topMargin: 5
                anchors.bottomMargin: 5

                Button {
                    text: "Switch to Widgets"
                    implicitWidth: 120
                    implicitHeight: 30
                    onClicked: qtai.onSwitchUI()

                    background: Rectangle {
                        color: parent.pressed ? "#4d4d4d" : (parent.hovered ? "#3d3d3d" : "#1e1e1e")
                        border.color: "#3d3d3d"
                        border.width: 1
                        radius: 3
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "#ffffff"
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                TabBar {
                    id: tabBar
                    currentIndex: qtai ? qtai.currentIndex : 0
                    onCurrentIndexChanged: if (qtai) qtai.currentIndex = currentIndex
                    Layout.fillWidth: true
                    spacing: 0

                    Repeater {
                        model: ["KNN", "KMC"]
                        TabButton {
                            text: modelData // ^ names from model 
                            height: 30
                            background: Rectangle {
                                color: parent.checked ? "#404040" : (parent.hovered ? "#3d3d3d" : "#1e1e1e")
                                border.color: parent.checked ? "#5d5d5d" : "#3d3d3d"
                                border.width: 1
                                radius: 3
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }
            }
        }

        StackLayout {
            id: stackLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            Loader {
                id: knn
                source: "knn.qml"  // Path relative to qtai.qml
                asynchronous: true  // Optional, for smoother loading
                onLoaded: {
                    knn.item.knn = qtai.algorithms[0]
                }
            }

            // KMC Page (index 1)
            Rectangle {
                color: "#1e1e1e"
                ColumnLayout {
                    anchors.centerIn: parent
                    width: Math.min(parent.width - 80, 600)
                    spacing: 20

                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        text: "" // based data loaded or not
                        color: "#ffffff"
                        font.pixelSize: 14
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }

                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        height: 8
                        color: "#2d2d2d"
                        radius: 4
                        visible: true // based on data loaded or not

                        Rectangle {
                            width: 0 // based on width and loading progress
                            height: parent.height
                            color: "#00aaff"
                            radius: 4
                            Behavior on width {
                                NumberAnimation {
                                    duration: 300
                                    easing.type: Easing.InOutQuad
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // MNIST Image component
    component MNISTImage: Rectangle {
        id: imageRect
        property var imageData: []
        property color borderColor: "#3d3d3d"
        property int borderWidth: 1

        color: "transparent"
        border.color: borderColor
        border.width: borderWidth

        Canvas {
            id: canvas
            anchors.fill: parent
            anchors.margins: parent.border.width
            antialiasing: false

            onPaint: {
                if (!imageRect.imageData || imageRect.imageData.length !== 784)
                    return;
                var ctx = getContext("2d");
                ctx.imageSmoothingEnabled = false; // keep smoothing disabled for pixel look
                ctx.clearRect(0, 0, width, height);
                var cellWidth = width / 28;
                var cellHeight = height / 28;

                for (var i = 0; i < 784; i++) {
                    var val = imageRect.imageData[i];
                    var x = (i % 28) * cellWidth;
                    var y = Math.floor(i / 28) * cellHeight;

                    ctx.fillStyle = Qt.rgba(val / 255, val / 255, val / 255, 1.0);
                    ctx.fillRect(x, y, cellWidth + 0.1, cellHeight + 0.1);
                }
            }
        }

        onImageDataChanged: canvas.requestPaint()
    }
}
