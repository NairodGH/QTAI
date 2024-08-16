# <p align="center">📊 QTAI 🤖</p>

<p align="center">
    <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/8/81/Qt_logo_neon_2022.svg/800px-Qt_logo_neon_2022.svg.png">
</p>

## 📋 Table of contents
<details>
<summary>Click to reveal</summary>

- [About](#-about)
- [Requirements](#-requirements)
- [Building](#-building)
- [Usage](#-usage)
- [Author](#-author)

</details>

## 🔍 About

Can also be found at https://github.com/NairodGH/QTAI.\
QTAI is a personal C++ windows project I've started back in 2023 for school.\
It is a [Qt](https://fr.wikipedia.org/wiki/Qt) application to visualize the prediction results of both [KNN](https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm) and [KMC](https://en.wikipedia.org/wiki/K-means_clustering) on the [MNIST](https://en.wikipedia.org/wiki/MNIST_database) database.\
It features asynchronous usage of both algorithms once their common data and labels files are drag and dropped by displaying dataset infos, the current number's guess with visualization (KNN, for each number and each k so takes a while) and the current k accuracy (KMC).\
Having already worked with the Win32 API, I wanted to try Qt as a more modern UI library while exploring a new subject such as from-scratch machine learning.\
Thanks to [Gerard TAYLOR](https://www.youtube.com/playlist?list=PL79n_WS-sPHKklEvOLiM1K94oJBsGnz71) for the tutorial.

## 💻 Requirements

You will need:
- [Visual Studio](https://visualstudio.microsoft.com) 2022
- [Qt](https://www.qt.io/download-qt-installer-oss)

## 🔧 Building

Configure Qt to be detected by Visual Studio, their [extension](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022) helps.
Launch the Visual Studio project from ```QTAI.vcxproj``` and start it using the top "Local Windows Debugger" button.

## 🎮 Usage

Drag and drop the data file ```train-images-idx3-ubyte``` first and then the labels file ```train-labels-idx1-ubyte``` to be able to start the algorithms with their respective buttons.

## 🤝 Author

[Nairod](https://github.com/NairodGH)
