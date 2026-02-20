import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

// qmllint disable unqualified
// qmllint disable missing-property

Rectangle {
    id: knn
    color: "#1e1e1e"

    property var algorithm: null
    property var mnistImageComponent: null

    property string statusText: "Starting KNN..."
    property string kValueText: "Waiting for nearest neighbors..."
    property var neighborsData: []

    Connections {
        target: algorithm
        enabled: algorithm !== null

        function onProgress(info) {
            skipButton.enabled = true;
            kValueText = "Nearest " + (info.k === 1 ? "" : info.k + " ") + "neighbor" + (info.k === 1 ? "" : "s") + ":";
            statusText = (info.isValidation ? "Validation" : "Test") + " | Acc: " + info.accuracy.toFixed(2) + "%" + " | Image " + info.currentIndex + "/" + info.totalCount;
            if (currentImage.item)
                currentImage.item.imageData = info.currentImage;
            neighborsData = info.neighbors;
        }

        function onTrainComplete(result) {
            var col = result.i;

            if (col === 0) {
                kRowLabel.visible = true;
                accuracyRowLabel.visible = true;
            }

            var kItem = kRepeater.itemAt(col);
            if (kItem)
                kItem.visible = true;

            var accItem = accuracyRepeater.itemAt(col);
            if (accItem) {
                accItem.visible = true;
                accItem.text = result.accuracy.toFixed(1) + "%";
            }
        }

        function onTestComplete(bestK, bestAccuracy, accuracy) {
            statusText = "Test Complete! Using k=" + bestK + " (best validation: " + bestAccuracy.toFixed(2) + "%)\n" + "Test accuracy: " + accuracy.toFixed(2) + "%";
            pauseButton.visible = false;
            skipButton.visible = false;
            restartButton.visible = true;
        }
    }

    // KNN Visualization
    ColumnLayout {
        id: visualizationLayout
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        // K Results Grid
        Rectangle {
            id: resultsContainer
            Layout.fillWidth: true
            Layout.preferredHeight: resultsContainerLayout.height
            Layout.maximumHeight: 90
            color: "transparent"

            Column {
                id: resultsContainerLayout
                anchors.left: parent.left
                anchors.top: parent.top
                spacing: 14

                // Row 0: k values
                Row {
                    spacing: 19

                    Text {
                        id: kRowLabel
                        text: "k"
                        color: "#00aaff"
                        font.pixelSize: 11
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        width: 45
                        visible: false
                    }

                    Repeater {
                        id: kRepeater
                        model: algorithm ? algorithm.kValues.length : 0
                        delegate: Text {
                            text: algorithm ? algorithm.kValues[index] : ""
                            color: "#aaa"
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            width: 30
                            visible: false
                        }
                    }
                }

                // Row 1: Accuracy values
                Row {
                    spacing: 19

                    Text {
                        id: accuracyRowLabel
                        text: "Accuracy"
                        color: "#00aaff"
                        font.pixelSize: 11
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        width: 45
                        visible: false
                    }

                    Repeater {
                        id: accuracyRepeater
                        model: algorithm ? algorithm.kValues.length : 0
                        delegate: Text {
                            text: ""
                            color: "#aaa"
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            width: 30
                            visible: false
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
                id: statusLabel
                text: knn.statusText
                color: "white"
                font.pixelSize: 14
                font.bold: true
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
            }

            Button {
                id: pauseButton
                text: algorithm ? (algorithm.paused ? "Resume" : "Pause") : ""
                implicitWidth: 90
                implicitHeight: 30

                onClicked: {
                    algorithm.onPause();
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
                id: skipButton
                text: "Skip"
                implicitWidth: 120
                implicitHeight: 30

                onClicked: algorithm.onSkip()

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
                id: restartButton
                text: "Restart"
                implicitWidth: 90
                implicitHeight: 30
                visible: false

                onClicked: algorithm.onRestart()

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
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Left: Neighbors grid
            ColumnLayout {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width * 0.6 - 15
                spacing: 10

                Text {
                    id: kValueLabel
                    text: knn.kValueText
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
                        model: knn.neighborsData.length

                        ColumnLayout {
                            width: 80
                            height: 100
                            spacing: 3

                            Text {
                                text: knn.neighborsData[index] ? knn.neighborsData[index].label : "-"
                                color: "white"
                                font.pixelSize: 11
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                                Layout.preferredHeight: 16
                            }

                            Loader {
                                id: neighborLoader
                                readonly property int neighborIndex: index
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 80
                                Layout.alignment: Qt.AlignHCenter
                                width: 80
                                height: 80
                                sourceComponent: mnistImageComponent
                                onLoaded: {
                                    item.borderColor = "#3d3d3d";
                                    item.imageData = knn.neighborsData[neighborLoader.neighborIndex]?.image ?? [];
                                }

                                Connections {
                                    target: knn
                                    function onNeighborsDataChanged() {
                                        if (neighborLoader.item)
                                            neighborLoader.item.imageData = knn.neighborsData[neighborLoader.neighborIndex]?.image ?? [];
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Right: Current image
            ColumnLayout {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width * 0.4 - 15
                spacing: 15

                Text {
                    text: "Current Image:"
                    color: "white"
                    font.pixelSize: 14
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                Loader {
                    id: currentImage
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 200
                    Layout.alignment: Qt.AlignHCenter
                    width: 200
                    height: 200
                    sourceComponent: mnistImageComponent
                    onLoaded: {
                        item.borderColor = "#00aaff";
                        item.borderWidth = 2;
                        item.imageData = knn.currentImageData;
                    }
                }
            }
        }
    }
}
