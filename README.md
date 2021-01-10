# NULL Engine

---

Latest Release: [Click Here](<https://github.com/BarcinoLechiguino/NULL_Engine/releases/tag/v0.75>)

---

Null Engine is a basic 3D Graphics Engine for game development. It has been developed by a single [Bachelor's degree in Video Game Design and Development](<https://www.citm.upc.edu/ing/estudis/graus-videojocs/>) student at [UPC CITM](<https://www.citm.upc.edu/>) [Tech Talent Center](<https://www.talent.upc.edu/cat/school/ttc/>) in Barcelona.

It has been coded from scratch in C and C++ along with the following libraries:

- [Standard Template Library](<https://en.wikipedia.org/wiki/Standard_Template_Library>)
- [Simple DirectMedia Layer](<https://libsdl.org/>)
- [OpenGL v4.6](<https://www.opengl.org/>)
- [Glew v2.1.0](<http://glew.sourceforge.net/>)
- [ImGui v1.79 (Docking Branch)](<https://github.com/ocornut/imgui/tree/docking>)
- [ImGuizmo v1.61](<https://github.com/CedricGuillemet/ImGuizmo>)
- [MatGeoLib v1.5](<https://github.com/juj/MathGeoLib>)
- [PhysFS v3.0.2](<https://icculus.org/physfs/>)
- [Parson v1.1.0](<https://github.com/kgabis/parson>)
- [Assimp v3.1.1](<http://www.assimp.org/index.php/downloads>)
- [DevIL v1.8.0](<http://openil.sourceforge.net/download.php>)
- [mmgr](<http://www.paulnettle.com/>)

****

## The Developer: Ángel González 

* Github: BarcinoLechiguino (https://github.com/BarcinoLechiguino)
* Repository: https://github.com/BarcinoLechiguino/NULL_Engine 

**Implemented features** 

**As of 01/11/2020:**

* Library Integration (All o the stated in the introduction).
* ImGui UI Display & Structure
* Configuration & Console
* GameObject and Components
* Hierarchy & Inspector
* 3D Model Loading (W/ Support for .fbx files)
* Image Loading (W/ Support for .png and .dds files)
* Drag & Drop Models and textures from a directory.
  

**As of 13/12/2020:**

* GameObject Transform
* Mesh Material Color Import
* Custom File Formats for Mesh, Material and Texture resources. (.nemesh,  .nemat, .netex)
* Scene and GameObject Serialization.
* Editor Support for Loading Files.
* All scene rendering operations are now batched and processed together.
* Scene Framebuffer and Editor Scene displaying the Scene framebuffer texture.
* Camera Component (Refactored from Camera Module)
* Camera Frustum (FOV, Aspect Ratio, Near and Far Plane distance...)
* Frustum Culling
* Mouse Picking through Raycasting.
* Editor Gizmos that allow to manipulation of the selected GameObject.

**As of 10/01/2021:**

* Resource Manager (meta files, reference counting...).
* Animation Importing (Import, Save & Load).
* Clip Managing (Creation and Execution).
* Animation Interpolation.
* Animation Blending.

****

### Engine Features

* **Main Menu Bar:** Placed at the top of the window. It has various menu items that allow to exit the engine, open/close editor panels and show some primitive examples.

  ---

* **Toolbar:** Placed at the top of the window, just below the Main Menu Bar. It contains various items such as the Play&Stop buttons. This feature will be further developed when the Play&Stop feature is Implemented.

  ****

* **Configuration Panel:** Placed at the right side of the window. It has multiple collapsing headers that display different values and parameters of each existing module.

  ---

* **Scene Panel:** Placed at the center of the window. It will display the current state of the Engine's 3D environment. In this Panel GameObjects can be selected with the mouse and manipulated through the use of Gizmos.

  ---

* **Game Panel:**  Placed at the center of the window (tab at the right of the Scene Panel).  Will display the Engine's 3D environment while in "Game State". This feature will be further developed when the Play&Stop feature is implemented.

  ****

* **Hierarchy Panel:** Placed at the left side of the window. It shows all the game objects that are currently placed in the current scene. Upon clicking any of them the Inspector panel will show all the components the selected game object has.

  ****

* **Inspector Panel:** Placed at the right side of the window (along with Configuration). It shows the components of a selected game object. The components are Transform, Mesh and Material. 

  Disclaimer: To see all the possible components that a game object can have create an empty game object. Otherwise if a game object was created with an imported model it will only have the components it needs.

  ****

* **Console Panel: ** Placed at the bottom of the window. It outputs information about the state of the engine: Loading&Saving, Importing models and textures...

  ---

* **Project Panel:** Placed at the bottom of the window. It shows all the assets inside the "Assets/" folder. It allows to load into scene any file displayed in the panel just by dragging and dropping it onto the Scene Panel.

* ---

  **Load File Panel:** Placed at the center of the window. It is hidden by default, and can be opened with *Ctrl + O*. It will display all the Assets that are inside the "Assets/" folder and allows to select a file and Importing it into the Engine.

  ****

* **Drag&Drop:** In this version of the engine models and textures can be dropped from a directory into the engine. For models this creates a new game object per individual mesh in the model. Moreover, the first mesh to be processed will be set as the parent of the rest. Textures will also be loaded onto the model.

  For textures, if a texture is dropped and a game object is currently being selected/inspected, that texture will be added to it. To try out this feature just go to the Textures folder in Assets and drag and drop the test image (it's a classic).

  ****

  

### Innovations

Currently there are no innovations to speak of.

****

## About Use

### Installation

No previous installation is required to use the engine, just download the latest [RELEASE](<https://github.com/BarcinoLechiguino/NULL_Engine/releases>) from the repository and execute the "NULL_Engine.exe" executable, which you will find in the "NULLEngine" folder.



### Camera Controls

#### **MOUSE**

* **Right Click:** Selects the Game Object under the mouse.
* **Left Alt + Right Click + Drag:** Rotates the camera around a target.
* **Wheel Click + Drag:** Pans the camera in the plane of the camera frame.
* **Mouse Wheel:** Zooms the camera in its Z axis.
* **Left Alt + Left Click**: Rotate around a reference point.

**KEYBOARD**

* **W Key:** Moves the camera forwards on it's X axis.
* **A Key:** Strafes the camera to the left on it's Z axis.
* **S Key:** Moves the camera backwards on it's X axis.
* **D Key:** Strafes the camera to the right on it's Z axis.
* **Q Key:** Moves the camera vertically up on its Y axis.
* **E Key:** Moves the camera vertically down on its Y axis.
* **SHIFT Key:** Duplicates the camera's movement speed.
* **O Key: ** Moves the camera back to the world origin.
* **KP PLUS:** Increses the FOV of the current camera by 5 degrees.
* **KP MINUS:** Decreases the FOV of the current camera by 5 degrees.



### Shortcuts

#### **Panels**

* **1 Key:** Enable/Disable the Configuration Panel.
* **2 Key:** Enable/Disable the Hierarchy Panel.
* **3 Key:** Enable/Disable the Inspector Panel.
* **4 Key:** Enable/Disable the Console Panel.
* **8 Key:** Enable/Disable the About Panel.
* **9 Key:** Enable/Disable the ImGui Demo Panels.
* **ESCAPE Key:** Enable/Disable the Close App Popup.



#### **Save&Load** 

* **Ctrl + S:** Serializes the current scene.
* **Ctrl + O:** Opens the "Load File" window, from which any file in "Assets/" can be loaded.
* **F5 Key:** Save current editor state. (WIP, Not Working Yet)
* **F6 Key:** Enable/Disable the show normals mode. (WIP, Not Working Yet)



### Debug Keys

* **F1 Key:** Enable/Disable the world grid. 
* **F2 Key:** Enable/Disable the world axis.
* **F3 Key:** Enable/Disable the show wireframe mode.
* **F4 Key:** Enable/Disable the show normals mode.
* **F7 Key:** Enable/Disable the show textures mode.
* **F8 Key:** Enable/Disable the show colours mode.

****

## License & Copyright

MIT License

Copyright (c) 2021 [Angel Gonzalez T.]

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.