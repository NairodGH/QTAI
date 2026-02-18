# <p align="center">📊 QTAI 🤖</p>

<p align="center">
    <img src="knn.gif">
</p>

## 🔍 About

QTAI is a personal C++ windows project I've started back in 2023 for school.\
It is a [Qt](https://fr.wikipedia.org/wiki/Qt) application to visualize how [ML](https://en.wikipedia.org/wiki/Machine_learning) algorithms such as [KNN](https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm) and [KMC](https://en.wikipedia.org/wiki/K-means_clustering) on the [MNIST](https://en.wikipedia.org/wiki/MNIST_database) database (with [ETL](https://en.wikipedia.org/wiki/Extract,_transform,_load)).\
Having already worked with the [Win32 API](https://github.com/NairodGH/Chaos), I wanted to try Qt, with both widgets and QML, as a more modern/popular UI library while exploring from-scratch machine learning.\
The old (ugly) widgets only version can be found [here](https://github.com/NairodGH/QTAI/tree/ce7609ccc1f11c0c2fdb20603e694ac39224e7d1).

## 💻 Requirements

You will need:
- [CMake 4.0+](https://cmake.org/download/)
- [Qt 6.latest](https://www.qt.io/download-qt-installer-oss)

## 🔧 Building

I recommend opening the project in [Visual Studio Code](https://code.visualstudio.com/) with the [CMake tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extension installed.\
It should detect and build the CMakeLists.txt automatically, then you can just use the bottom left ▶️ button to launch it.

## 🎮 Usage

Use the top left switch button to change between QtWidgets and QML UI, it shouldn't visually change much because its goal was mainly for me to discover both code-wise.\
Use the top tabs to switch between ML algorithms, each then has its own set of QoL controls.

## 🤝 Author

[Nairod](https://github.com/NairodGH)