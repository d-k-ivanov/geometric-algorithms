# Geometric Algorithms (2023-2024)

Base project for the subject of Geometric Algorithms at the University of Granada (Course 2024).
The application's origins belong to [Alfonso López Ruiz](https://github.com/AlfonsoLRz). I strongly recommend reading his Master's Thesis, [Simulation of 3D Scans](https://crea.ujaen.es/bitstream/10953.1/19941/1/AlfonsoLopez.TFM.pdf).

## Compilation: Linux

- [Install Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)
- [Install VCPKG](https://vcpkg.io/en/getting-started.html)
- Add VCPKG folder to PATH:
  - Option 1: Temporary local environment

    ```bash
    # Assuming that VCPKG cloned and bootstrapped in ~/vcpkg
    export PATH="~/vcpkg;${PATH}"
    ```

  - Option 2: Local environment and Bash profile

    ```bash
    # Assuming that VCPKG cloned and bootstrapped in c:\src\vcpkg
    export PATH="~/vcpkg;${PATH}"
    echo 'export PATH="~/vcpkg;${PATH}"' >> ~/.bashrc
    ```

  - **Important Note:** the VCPKG requests installation of additional packages
- Navigate to the folder with the YOUR_FANCY_PROJECT_NAME App
- Run `build.sh`

## Compilation: Windows

- [Install Visual Studio](https://visualstudio.microsoft.com/downloads/)
- [Install Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)
- [Install VCPKG](https://vcpkg.io/en/getting-started.html)
- Add VCPKG folder to PATH:
  - Option 1: Command Line tools

    ```bat
    REM Assuming that VCPKG cloned and bootstrapped in c:\src\vcpkg
    REM setx for the global environment, set for the local
    setx PATH c:\src\vcpkg;%PATH%
    set  PATH c:\src\vcpkg;%PATH%
    ```

  - Option 2: PowerShell

    ```ps1
    # Assuming that VCPKG cloned and bootstrapped in c:\src\vcpkg
    [Environment]::SetEnvironmentVariable("PATH", "c:\src\vcpkg;${PATH}", "Machine")
    Set-Item -Path Env:PATH -Value "c:\src\vcpkg;${PATH}"
    ```

  - Option 3: Manually in *System Properties* --> *Environment Variables*
- Navigate to the folder with the YOUR_FANCY_PROJECT_NAME App
- Run `build.bat`
- Open `build\YOUR_FANCY_PROJECT_NAME.sln` in Visual Studio to work with the source code

## Screenshots

### Convex Hull and Triangulation

|================================|================================|================================|
| ![](Docs/p4/images/p4-4.png)   | ![](Docs/p4/images/p4-2.gif)   | ![](Docs/p4/images/p4-3-a.png) |
| ![](Docs/p4/images/p4-2-b.png) | ![](Docs/p4/images/p4-2-a.png) | ![](Docs/p4/images/p4-1-b.png) |

### Voxelixation

|=================================================|========================================================|
| ![](Docs/p3/images/p3-0-models.png)                                                                     ||
| ![](Docs/p3/images/p3-1-ajax-brute-force.png)   | ![](Docs/p3/images/p3-1-cheburashka-brute-force.png)   |
| ![](Docs/p3/images/p3-2-ajax-line-sweep.png)    | ![](Docs/p3/images/p3-2-cheburashka-line-sweep.png)    |
| ![](Docs/p3/images/p3-3-ajax-aabb-sampling.png) | ![](Docs/p3/images/p3-3-cheburashka-aabb-sampling.png) |

### 3D Geometry

|===============================|===============================|===============================|
| ![](Docs/p2/images/p2a-1.png) | ![](Docs/p2/images/p2a-2.png) | ![](Docs/p2/images/p2a-3.png) |
| ![](Docs/p2/images/p2a-4.png) | ![](Docs/p2/images/p2a-5.png) | ![](Docs/p2/images/p2a-6.png) |
| ![](Docs/p2/images/p2b-1.png) | ![](Docs/p2/images/p2b-2.png) | ![](Docs/p2/images/p2b-3.png) |
| ![](Docs/p2/images/p2c-1.png) | ![](Docs/p2/images/p2c-2.png) | ![](Docs/p2/images/p2c-3.png) |

### 2D Geometry

![](Docs/p1-a/images/all_in_one.png)

|============================================|==========================================|========================================|
| ![](Docs/p1-a/images/segment_ray_line.png) | ![](Docs/p1-a/images/convex_polygon.png) | ![](Docs/p1-a/images/bezier_curve.png) |
| ![](Docs/p1-b/images/result_1.png)         | ![](Docs/p1-b/images/result_2.png)       | ![](Docs/p1-b/images/result_3.png)     |

## Usage

### Camera

Interaction with keyboard and mouse.

| Movement          | Interaction                      |
|-------------------|----------------------------------|
| Forward           | Right Mouse Click + <kbd>W</kbd> |
| Backwards         | Right Mouse Click + <kbd>S</kbd> |
| Left              | Right Mouse Click + <kbd>A</kbd> |
| Backwards         | Right Mouse Click + <kbd>D</kbd> |
| Zoom              | Mouse Wheel                      |
| Horizontal orbit  | <kbd>X</kbd>                     |
| Vertical orbit    | <kbd>Y</kbd>                     |
| Camera turn       | Left Mouse Click                 |
| Reset camera      | <kbd>B</kbd>                     |

### Gizmos

Interaction with models to carry out translation, rotation, and scaling operations.

To do this, it is necessary to open the `Settings` > `Models` menu and select a model.

| Operation        | Interaction  |
|------------------|--------------|
| Model translation| <kbd>T</kbd> |
| Model rotation   | <kbd>R</kbd> |
| Model scaling    | <kbd>S</kbd> |

<p align="center">
    <img src="Images/guizmo.png" width=800 /></br>
    <em>Model transformation using the interface. In this case a rotation is shown.</em>
</p>

### Visualization

Visualization of different topologies, these having been generated during the loading of the models and being found in the vector of models of the scene (`SceneContent`, although it must be managed from the `Renderer`). The topology can be controlled at a global level, so we can activate and deactivate its rendering in the `Settings > Rendering` menu, or at a local level (for each model) through the `Settings > Models` menu.

| Operation                         | Interaction  |
|-----------------------------------|--------------|
| Activate/Deactivate point cloud   | <kbd>0</kbd> |
| Enable/Disable wire mesh          | <kbd>1</kbd> |
| Activate/Deactivate triangle mesh | <kbd>2</kbd> |

<p align="center">
    <img src="Images/topology.png" width=500 /></br>
    <em>Point cloud, wire mesh and triangle mesh displayed on the same model.</em>
</p>

### Taking a screenshot

Screenshot with antialiasing (for `:D` documentation). We can capture it using the keyboard or interface (`Settings > Screenshot` menu). With this last option it is also possible to modify the size of the image or the destination file.

| Operation  | Interaction  |
|------------|--------------|
| Screenshot | <kbd>K</kbd> |

## Other functionalities

Many other functionalities are offered from the interface:

1. `Settings` > `Rendering`:
    - Modification of visible topologies, as in point 3 of the previous list.
    - Background colour modification.
2. `Settings` > `Camera`:
    - Modification of camera properties and projection type.
3. `Settings` > `Lights`:
    - Modification of a single point light (colours and position in space). Remember that the objective of this subject is not *rendering*, so this point light is enough for us, which allows us to see any mesh of triangles located at any point in space.

<p align="center">
    <img src="Images/light_configuration.png" width=600 /></br>
    <em>Scene spot light configuration menu.</em>
</p>

4. `Settings` > `Models`:
    - Modification of model transformation.
    - Material modification (points, lines, and triangles).
    - Modification of size and width of points and lines, respectively.
    - Loading triangle meshes (`.obj`, `.gltf` and `.fbx`).

### Integration of new renderable models

The new model should be implemented as a subclass of `Model3D`, which will give us all the necessary functionality to load and draw the geometry and topology on the GPU. Therefore, we ignore this task and our only task is to define geometry and topology.

It should be noted that the attributes of a vertex (`VAO::Vertex`) are (in order): position (`vec3`), normal (`vec3`) and texture coordinates (`vec2`). Thus, we can add new vertices to our model using the following syntax:

```cpp
componente->_vertices.insert(component->vertices.end(), { vertices })
```

where vertices can be defined as follows:

```cpp
{
    VAO::Vertex { vec3(x, y, z), vec3(nx, ny, nz) },
    VAO::Vertex { vec3(x, y, z) },
    VAO::Vertex { vec3(x, y, z), vec3(nx, ny, nz), vec2(u, v) }
}
```

The order is important, but we can omit those attributes that we do not know.

Regarding the topology, we will have three vectors available (point cloud, wire mesh, and triangle mesh) in the variable `component->_indices`. Again, we can insert primitives as shown below:

- Triangles:

    ```cpp
        componente->_indices[VAO::IBO_TRIANGLES].insert(
            componente->_indices[VAO::IBO_TRIANGLES].end(),
            {
                0, 1, 2, RESTART_PRIMITIVE_INDEX,
                1, 2, 3, RESTART_PRIMITIVE_INDEX,
                ...
            })
    ```

- Lines:

    ```cpp
        componente->_indices[VAO::IBO_TRIANGLES].insert(
            componente->_indices[VAO::IBO_TRIANGLES].end(),
            {
                0, 1, RESTART_PRIMITIVE_INDEX,
                1, 2, RESTART_PRIMITIVE_INDEX,
                ...
            })
    ```

- Points:

    ```cpp
        componente->_indices[VAO::IBO_TRIANGLES].insert(
            componente->_indices[VAO::IBO_TRIANGLES].end(),
            {
                0, 1, 2, 3, 4
                ...
            })
    ```

    **Note**: given a number of vertices `n`, we can generate a vector like ${0, 1, 2, ..., n-1}$ using `std::iota(begin, end, 0)` after `vector.resize(n)`.

Additionally, the `Settings > Models` menu will display a list of objects available in the scene. Due to C++ limitations on inheritance, it is not possible to obtain the name of the class to which an object that inherits from `Model3D` belongs in its constructor. However, once built it is possible to access said name. For this reason, if we want the objects to have a meaningful name we can use the `overrideModelName` function.

The `SET` methods of the `Model3D` class have been implemented in such a way that calls can be chained on the same line after constructing the object, including operations such as `overrideModelName`, `setPointColor`, `setLineColor` or `setTopologyVisibility`.

## Scene management

The management of the scene elements will be carried out in `Graphics/Renderer`. To do this, we have two basic methods: `createModels` and `createCamera`, where both will generate models and cameras that will be stored in an instance of `SceneContent`. Therefore, simply use the `create*` functions in the `Renderer`.

Taking into account that the camera can be positioned depending on the models, we will first create the latter. In `buildFooScene()` we have an example of generating the scene:

```cpp
vec2 minBoundaries = vec2(-1.5, -.5), maxBoundaries = vec2(-minBoundaries);

// Triangle mesh
auto model = (new DrawMesh())->loadModelOBJ("Assets/Models/Ajax.obj");
model->moveGeometryToOrigin(model->getModelMatrix(), 10.0f);
_content->addNewModel(model);


// Spheric randomized point cloud
int numPoints = 800, numPointClouds = 6;

for (int pcIdx = 0; pcIdx < numPointClouds; ++pcIdx)
{
    PointCloud* pointCloud = new PointCloud;

    for (int idx = 0; idx < numPoints; ++idx)
    {
        ...
        pointCloud->addPoint(Point(rand.x, rand.y));
    }

    _content->addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(RandomUtilities::getUniformRandomColor())->overrideModelName());
    delete pointCloud;
}

// Random segments
int numSegments = 8;

for (int segmentIdx = 0; segmentIdx < numSegments; ++segmentIdx)
{
    ...
    SegmentLine* segment = new SegmentLine(a, b);

    _content->addNewModel((new DrawSegment(*segment))->setLineColor(RandomUtilities::getUniformRandomColor())->overrideModelName());
    delete segment;
}

// Random triangles
int numTriangles = 30;
float alpha = ...;

for (int triangleIdx = 0; triangleIdx < numTriangles; ++triangleIdx)
{
    ...
    Triangle* triangle = new Triangle(a, b, c);

    _content->addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))
        ->overrideModelName());
    delete triangle;
}
```

To consider:

- `addNewModel` will receive a pointer to an object that inherits from `Model3D`.
- `_content` will be the scene (we should not modify anything in this class).
- The `setters` of a 3D model have been implemented as `Model3D* set*()` to be able to chain calls in the same instantiation (considering that said instance will not be necessary in our `Renderer`, and therefore, it will be eliminated continuation).
  - What can we modify using `setters`?:
    - Colour: `setPointColor`, `setLineColor`, `setTriangleColor`. Keep in mind that the latter receives a `glm::vec4` to be able to modify the alpha of the model.
    - Visibility of primitives: `setTopologyVisibility`. You will receive a primitive type of `VAO::IBO_slots` and boolean.
    - `moveGeometryToOrigin`: calculates the transformation matrix that takes a model, located at an unknown point, to the origin of the coordinate system. Additionally, you can control the scale so that it can be displayed in our viewport.
    - `overrideModelName`: by default, a model will receive a generic name in its constructor, such as `Model3D 8, Comp. 0`. However, we can customize this name automatically so that it is identifiable in the list of models (accessible through the `Settings > Models` menu). Note that the name of a subclass cannot be obtained in the constructor. Therefore, this possibility is offered as a subsequent call.

<table style="margin:auto; width:80%">
<tr>
    <td>
        <img align="center" src="Images/generic_names.png"/>
    </td>
    <td>
        <img src="Images/customized_names.png"/>
    </td>
</tr>
</table>
<em>Comparison of model listings, using generic names and custom names for each model (automatically assigned).</em>
