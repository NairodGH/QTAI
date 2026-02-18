import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: knn
    color: "#1e1e1e"

    property var knn

    Connections {
        target: knn
        function onSkip() { knn.onSkip() }
        function onProgress(info) { /* UI */ }
        function onTrainComplete(result) { /* UI */ }
    }

    // Loading/Initial state
    MouseArea {
        id: knnClickArea
        anchors.fill: parent
        visible: !root.showKNNVisualization
        cursorShape: Qt.ArrowCursor // or Qt.PointingHandCursor based on data loaded or not
        enabled: true // based on data loaded or not
        onClicked: {}

        ColumnLayout {
            anchors.centerIn: parent
            width: Math.min(parent.width - 80, 600)
            spacing: 20

            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                text: ""// based on data loaded or not
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

    // KNN Visualization
    ColumnLayout {
        id: knnVisualization
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 0
        anchors.bottomMargin: 20
        spacing: 10
        visible: root.showKNNVisualization

        // K Results Grid
        Rectangle {
            id: knnResultsContainer
            Layout.fillWidth: true
            Layout.maximumHeight: 90
            color: "transparent"
            visible: false

            Column {
                anchors.left: parent.left
                anchors.top: parent.top
                spacing: 0

                // Row 0: k values
                Row {
                    spacing: 12

                    Text {
                        text: "k"
                        color: "#00aaff"
                        font.pixelSize: 11
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        width: 45
                    }

                    Repeater {
                        model: root.kResults.length
                        delegate: Text {
                            text: root.kResults[index] ? root.kResults[index].k : ""
                            color: "#aaa"
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            width: 30
                        }
                    }
                }

                // Row 1: Accuracy values
                Row {
                    spacing: 12

                    Text {
                        text: "Accuracy"
                        color: "#00aaff"
                        font.pixelSize: 11
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        width: 45
                    }

                    Repeater {
                        model: root.kResults.length
                        delegate: Text {
                            text: root.kResults[index] ? (root.kResults[index].accuracy.toFixed(1) + "%") : ""
                            color: "#aaa"
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            width: 30
                        }
                    }
                }
            }
        }

        // Top bar with status and buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Text {
                id: knnStatusLabel
                text: "Starting KNN..."
                color: "white"
                font.pixelSize: 14
                font.bold: true
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
            }

            Button {
                id: knnPauseButton
                text: root.knnPaused ? "Resume" : "Pause"
                implicitWidth: 90
                implicitHeight: 30
                visible: false

                onClicked: {
                }

                background: Rectangle {
                    color: parent.enabled ? (parent.pressed ? "#505050" : (parent.hovered ? "#505050" : "#404040")) : "#2d2d2d"
                    border.color: "#5d5d5d"
                    border.width: 1
                    radius: 3
                }

                contentItem: Text {
                    text: parent.text
                    color: parent.enabled ? "white" : "#666"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                id: knnSkipButton
                text: "Skip k"
                implicitWidth: 120
                implicitHeight: 30
                visible: false

                onClicked: {
                }

                background: Rectangle {
                    color: parent.enabled ? (parent.pressed ? "#505050" : (parent.hovered ? "#505050" : "#404040")) : "#2d2d2d"
                    border.color: "#5d5d5d"
                    border.width: 1
                    radius: 3
                }

                contentItem: Text {
                    text: parent.text
                    color: parent.enabled ? "white" : "#666"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                id: knnRestartButton
                text: "Restart"
                implicitWidth: 90
                implicitHeight: 30
                visible: false

                onClicked: {
                }

                background: Rectangle {
                    color: parent.pressed ? "#505050" : (parent.hovered ? "#505050" : "#404040")
                    border.color: "#5d5d5d"
                    border.width: 1
                    radius: 3
                }

                contentItem: Text {
                    text: parent.text
                    color: "white"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // Main content
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 30

            // Left: Neighbors grid
            ColumnLayout {
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.6
                spacing: 10

                Text {
                    id: knnKValueLabel
                    text: "Nearest 3 Neighbors:"
                    color: "#00aaff"
                    font.pixelSize: 14
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                Grid {
                    id: neighborsGrid
                    columns: 5
                    spacing: 10
                    Layout.alignment: Qt.AlignHCenter

                    Repeater {
                        id: neighborsRepeater
                        model: 0
                        property var neighborsData: []

                        delegate: ColumnLayout {
                            width: 80
                            height: 100
                            spacing: 3
                            visible: index < neighborsRepeater.model

                            Text {
                                text: (neighborsRepeater.neighborsData && index < neighborsRepeater.neighborsData.length) ? neighborsRepeater.neighborsData[index].label : "-"
                                color: "white"
                                font.pixelSize: 11
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                                Layout.preferredHeight: 16
                            }

                            MNISTImage {
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 80
                                Layout.alignment: Qt.AlignHCenter
                                width: 80
                                height: 80
                                imageData: (neighborsRepeater.neighborsData && index < neighborsRepeater.neighborsData.length) ? neighborsRepeater.neighborsData[index].image : []
                                borderColor: "#3d3d3d"
                            }
                        }
                    }
                }
            }

            // Right: Current image
            ColumnLayout {
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.4
                spacing: 15
                Layout.alignment: Qt.AlignHCenter

                Text {
                    text: "Current Image:"
                    color: "white"
                    font.pixelSize: 14
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                MNISTImage {
                    id: knnCurrentImage
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 200
                    Layout.alignment: Qt.AlignHCenter
                    width: 200
                    height: 200
                    borderColor: "#00aaff"
                    borderWidth: 2
                }
            }
        }
    }
}