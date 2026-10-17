# 🎨 QtScene Studio

### 🖥️ A Lightweight 2D Scene & Object Editor Built with C++ and Qt

QtScene Studio is a desktop application developed using **C++17 and Qt Widgets**. It provides an interactive canvas for creating, selecting, manipulating, and animating 2D geometric objects.


## ✨ Features

- 🎨 **Interactive Canvas** — View and manipulate objects on a grid-based canvas.
- 🔷 **Geometric Shapes** — Create and render rectangles and ellipses.
- ➕ **Object Management** — Add and delete objects easily.
- 🖱️ **Mouse Interaction** — Select and drag objects directly on the canvas.
- ⚙️ **Property Editing** — Modify object names, positions, rotation, scale, and visibility.
- 📐 **Grid Snapping** — Align object positions to a 20-unit grid.
- 🔄 **Rotation Animation** — Animate object rotation with start and stop controls.
- 📊 **Progress Indicator** — Display animation progress using a custom widget.
- ⌨️ **Keyboard Shortcuts** — Delete selected objects using the Delete or Backspace key.
- 🔗 **Synchronized Data Model** — Keep the object table and canvas connected through Qt's Model/View architecture.

## 🛠️ Tech Stack

| Technology | Purpose |
|---|---|
| 💻 C++17 | Core application logic |
| 🖼️ Qt Widgets | Desktop user interface |
| 🎨 Qt Designer | UI layout and design |
| 📋 Qt Model/View | Object data management |
| 🖌️ QPainter | Canvas rendering |
| 🔄 QTransform | Object transformations and hit testing |
| ⏱️ QTimer | Rotation animation |
| ⚙️ qmake | Project configuration and build system |

## 📂 Project Structure

```text
QtScene-Studio/
├── main.cpp
├── mainwindow.cpp
├── mainwindow.h
├── mainwindow.ui
├── objectdata.h
├── objectmodel.cpp
├── objectmodel.h
├── scenecanvas.cpp
├── scenecanvas.h
├── levelwidget.cpp
├── levelwidget.h
├── Module_3.pro
├── README.md
├── .gitignore
```


## 🚀 Getting Started

### 📋 Prerequisites

Before running the project, make sure you have:

- ✅ Qt 6 with the Qt Widgets module
- ✅ Qt Creator
- ✅ A compatible C++17 compiler
- ✅ The required build tools for your operating system



## 🎮 How to Use

1. ➕ Add a geometric object using the application's object controls.
2. 🖱️ Select an object from the table or directly from the canvas.
3. ⚙️ Edit its properties using the available controls.
4. 📐 Enable grid snapping to align object positions.
5. 🔄 Use the animation controls to rotate the selected object.
6. 🗑️ Remove unwanted objects using the delete control or keyboard shortcuts.



## 🧠 Concepts Demonstrated

- Object-oriented programming in C++
- Qt signals and slots
- Custom widget development
- Model/View architecture
- 2D graphics rendering
- Mouse and keyboard event handling
- Geometric transformations
- Timer-based animation
- UI and data synchronization

## 🔮 Future Improvements

- 💾 Save and load scenes
- 🎨 Custom colors for individual objects
- 🔷 Support for additional geometric shapes
- ↩️ Undo and redo operations
- 🖼️ Export scenes as images
- 📏 Improved transformation controls
- 🧪 Automated tests for object manipulation and model updates

## 🤝 Contributing

Suggestions, bug reports, and improvements are welcome!

1. 🍴 Fork the repository.
2. 🌿 Create a feature branch.
3. 💻 Make your changes.
4. 📤 Submit a pull request.


## 👨‍💻 Author

**Alan George**