import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

// qmllint disable unqualified
// qmllint disable missing-property

Rectangle {
    id: root
    width: 1200
    height: 800
    color: "#1e1e1e"

    property var qtai: QTAI
    property Component mnistImageComponent: Component {
        MNISTImage {}
    }

    Connections {
        target: qtai
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // top bar
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
                    onCurrentIndexChanged: if (qtai)
                        qtai.currentIndex = currentIndex
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

        // content
        StackLayout {
            id: stackLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            Repeater {
                model: [
                    {
                        name: "KNN",
                        index: 0
                    },
                    {
                        name: "KMC",
                        index: 1
                    }
                ]

                Item {
                    property int index: modelData.index
                    property string name: modelData.name
                    MouseArea {
                        id: clickArea
                        anchors.fill: parent
                        visible: qtai ? !qtai.algorithms[index] : true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: qtai.onContentClicked()

                        ColumnLayout {
                            anchors.centerIn: parent
                            width: Math.min(parent.width - 80, 600)
                            spacing: 20

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                Layout.fillWidth: true
                                text: qtai ? qtai.loadingStatusText.arg(name) : ""
                                color: "#ffffff"
                                font.pixelSize: 14
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap
                            }
                        }
                    }

                    Loader {
                        id: contentLoader
                        anchors.fill: parent
                        visible: qtai ? qtai.algorithms[index] : true
                        source: name.toLowerCase() + ".qml"
                        active: qtai ? qtai.algorithms[index] !== null : true
                        asynchronous: true
                        onLoaded: {
                            if (item) {
                                item.algorithm = qtai.algorithms[index];
                                item.mnistImageComponent = root.mnistImageComponent;
                            }
                        }
                    }
                }
            }
        }
    }

    // mnist image visualizer used by algorithms
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
                ctx.imageSmoothingEnabled = false;
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
