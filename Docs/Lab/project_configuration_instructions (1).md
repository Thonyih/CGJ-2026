# **CGJ 2025 Configurations for Visual Studio 2022 \- Community Edition**

In this document, we will describe the project configurations and dependencies that are required for using the CGJ 2025 source code, and to run your own projects.

Additionally, we will also provide some notes regarding common issues.

<!-- When viewing this markdown source, remember that $ is used in math markdown, and as such, should be escaped, so \$ just means $ -->

&nbsp;

## **Creating a new project**

### **Creating a new Empty Project from scratch**

In order to create a new Empty Project from scratch, you can simply use the wizard that appears when you open up *Visual Studio*, or you can do it from *Visual Studio*'s menu. This includes no code.

- Select *File \-\> New \-\> Project* from the menu bar

- Select *Empty Project* (this type of project is already a *Console application project*)

- Insert the *Project Name*

- Select the *Project Location*

- Insert the *Solution Name*

- Click *Create*

### **Creating a new Console App Project from scratch**

In of order to create a new Console App Project from scratch, you can simply use the wizard that appears when you open up *Visual Studio*, or you can do it from *Visual Studio*'s menu. This will include code for a "Hello World\!" program.

- Select *File \-\> New \-\> Project* from the menu bar

- Select *Console App* (this type of project is already a *Console application project*)

- Insert the *Project Name*

- Select the *Project Location*

- Insert the *Solution Name*

- Click *Create*

### **Creating a new project from existing code**

In order to create a new project from existing code, select the option through *Visual Studio*'s menu.

- Select *File \-\> New \-\> Project From Existing Code* from the menu bar

- Select *Type of Project \-\> Visual C++*

- Select *Project file location*

- Insert the *Project Name*

- Select *Use Visual Studio \-\> Project Type \-\> Console application project*

- Click *Finish*

## **Quick Configuration**

<!-- When viewing this markdown source, remember that $ is used in math markdown, and as such, should be escaped, so \$ just means $ -->

&nbsp;

In this section, we explain how to quickly configure a Visual Studio project to use all the required libraries.

### **Dependencies installation**

OpenGL should already be installed in the system, and, as such, you do not need to install it or copy it into a project.

GLEW should be installed by extracting the library's zipped file contents into *\$(SolutionDir)libraries\\glew* of the solutions root folder.

GLFW should be installed by extracting the library's zipped file contents into *\$(SolutionDir)libraries\\glfw*.

GLM should be installed by extracting the library's zipped file contents into *\$(SolutionDir)libraries\\glm*.

#### **Configuring additional include directories for C/C++**

Open a project's properties:

- Right-click the project's name

- Select *Properties* (should be the last item in the right-click menu)

- Select *Configuration Properties \-\> C/C++ \- General \- Additional Include Directories*

- Add *\$(SolutionDir)libraries\\glew\\include*

- Add *\$(SolutionDir)libraries\\glfw\\include*

- Add *\$(SolutionDir)libraries\\glm*

#### **Configuring additional include library directories for the linker**

Open a project's properties:

- Right-click the project's name

- Select *Properties* (should be the last item in the right-click menu)

- Select *Configuration Properties \-\> Linker \- General \- Additional Library Directories*

- Add *\$(SolutionDir)libraries\\glew\\lib\\Release\\x64*

- Add *\$(SolutionDir)libraries\\glfw\\lib-vc2022*

#### **Configuring additional dependencies for the linker**

Open a project's properties:

- Right-click the project's name

- Select *Properties* (should be the last item in the right-click menu)

- Select *Configuration Properties \-\> Linker \-\> Input \- Additional Dependencies*

- Add *opengl32.lib*

- Add *glew32.lib*

- Add *glfw3dll.lib*

## **Individual Configurations**

<!-- When viewing this markdown source, remember that $ is used in math markdown, and as such, should be escaped, so \$ just means $ -->

&nbsp;

Here, we approach how to configure each of the libraries individually, which can help debug issues with a specific library.

### **OpenGL**

In this section, we go over the specific configurations required for using OpenGL in *Visual Studio*.

### **OpenGL \- Installation**

OpenGL should already be installed in the system, and, as such, you do not need to install it or copy it into a project.

#### **OpenGl \- Configuring additional dependencies**

Open a project's properties:

- Right-click the project's name

- Select *Properties* (should be the last item in the right-click menu)

- Select *Configuration Properties \-\> Linker \-\> Input \- Additional Dependencies*

- Add *opengl32.lib*

### **GLEW**

In this section, we go over the specific configurations required for using GLEW in *Visual Studio*.

### **GLEW \- Installation**

GLEW should be installed by extracting the library's zipped file contents into *\$(SolutionDir)libraries\\glew* of the solutions root folder.

#### **GLEW \- Configuring additional include directories for C/C++**

Open a project's properties:

- Right-click the project's name

- Select *Properties* (should be the last item in the right-click menu)

- Select *Configuration Properties \-\> C/C++ \- General \- Additional Include Directories*

- Add *\$(SolutionDir)libraries\\glew\\include*

#### **GLEW \- Configuring additional include library directories for the linker**

Open a project's properties:

- Right-click the project's name

- Select *Properties* (should be the last item in the right-click menu)

- Select *Configuration Properties \-\> Linker \- General \- Additional Library Directories*

- Add *\$(SolutionDir)libraries\\glew\\lib\\Release\\x64*

#### **GLEW \- Configuring additional dependencies for the linker**

Open a project's properties:

- Right-click the project's name

- Select *Properties* (should be the last item in the right-click menu)

- Select *Configuration Properties \-\> Linker \-\> Input \- Additional Dependencies*

- Add *glew32.lib*

### **GLFW**

In this section, we go over the specific configurations required for using GLFW in *Visual Studio*.

### **GLFW \- Installation**

GLFW should be installed by extracting the library's zipped file contents into *\$(SolutionDir)libraries\\glfw*.

#### **GLFW \- Configuring additional include directories for C/C++**

Open a project's properties:

- Right-click the project's name

- Select *Properties* (should be the last item in the right-click menu)

- Select *Configuration Properties \-\> C/C++ \- General \- Additional Include Directories*

- Add *\$(SolutionDir)libraries\\glfw\\include*

#### **GLFW \- Configuring additional include library directories for the linker**

Open a project's properties:

- Right-click the project's name

- Select *Properties* (should be the last item in the right-click menu)

- Select *Configuration Properties \-\> Linker \- General \- Additional Library Directories*

- Add *\$(SolutionDir)libraries\\glfw\\lib-vc2022*

#### **GLFW \- Configuring additional dependencies for the linker**

Open a project's properties:

- Right-click the project's name

- Select *Properties* (should be the last item in the right-click menu)

- Navigate to *Configuration Properties \-\> Linker \-\> Input \- Additional Dependencies*

- Add *glfw3dll.lib*

### **GLM**

In this section, we go over the specific configurations required for using GLM in *Visual Studio*.

#### **GLM \- Installation**

GLM should be installed by extracting the library's zipped file contents into *\$(SolutionDir)libraries\\glm*.

#### **GLM \- Configuring additional include directories for C/C++**

Open a project's properties:

- Right-click the project's name

- Select *Properties* (should be the last item in the right-click menu)

- Navigate to *Configuration Properties \-\> C/C++ \-\> General \- Additional Include Directories*

- Add *\$(SolutionDir)libraries\\glm*

## **Notes**

<!-- When viewing this markdown source, remember that $ is used in math markdown, and as such, should be escaped, so \$ just means $ -->

&nbsp;

### **Library Paths**

We are using the path **\$(SolutionDir)libraries\\** to signify that each of the used libraries is placed under a folder named **dependencies** of the solution's root directory. The macro **\$(SolutionDir)** is just a *Visual Studio* macro that points to the solution's directory, no matter where it is located \- basically just solves the issues that can arise from having fixed paths and migrating projects from one machine to another.

### **Copying the required libraries to the output directories**

<!-- When viewing this markdown source, remember that $ is used in math markdown, and as such, should be escaped, so \$ just means $ -->

&nbsp;

The current workflow requires manually copying some libraries (dll) them into the output directory.

These libraries are located at:

- glew32 \-\> dependencies\\glew\\bin\\Release\\x64\\glew32.dll

- glfw32 \-\> dependencies\\glfw\\lib-vc2022\\glfw3.dll

An alternative to manually copying the libraries is setting up a *Post-Build Event*, this can be done in the project's properties:

- Right-click the project's name

- Select *Properties* (should be the last item in the right-click menu)

- Navigate to *Configuration Properties \-\> Build Events \-\> Post-Build Event \- Command Line*

- add *xcopy /y \$(SolutionDir)libraries\\glew\\bin\\Release\\x64\\glew32.dll \$(OutDir)*

- add *xcopy /y \$(SolutionDir)libraries\\glfw\\lib-vc2022\\glfw3.dll \$(OutDir)*

### **Console Application Reminder**

Remember to setup the project as a console application so that there are no weird errors regarding different signatures for the entry functions (the main function).

Otherwise, we will get an error similar to *Error LNK2019 unresolved external symbol \_WinMain@16 referenced in function "int \_\_cdecl invoke\_main(void)" (?invoke\_main@@YAHXZ)*.

To fix this, we need to change the project type to a console project in a project's properties:

- Right-click the project's name

- Select *Properties* (should be the last item in the right-click menu)

- Navigate to *Configuration Properties \-\> Linker \-\> System \- Subsystem*

- Change it to *Console (/SUBSYSTEM:CONSOLE)*

### **Applications with multiple executables (entry points)**

For solutions with multiple entry points, remember to create different projects in the solution (one for each entry point), and to make sure all the dependencies and linking is sorted for the multiple projects in the solutions.

<!-- Needs a better explanation when starting from existing code -->

&nbsp;

One solution to this, if starting from existing code, is to simply to start by creating multiple projects in subfolders of the solution's directory (with the necessary source code files). Then create a new project from existing code in the root, add the previously created projects to this new solution, then delete the dummy project that was created by default in this new solution. Then you can delete the other solutions that were created for the subfolders (they are no longer needed).

When creating a project from scratch, simply add new projects to the solution as subdirectories.

### **Visual Studio version used**

All mentions of *Visual Studio* refer to *Visual Studio 2022 \- Community Edition*.

### **Linux source code to Windows Issues**

#### **Error C2079 'oss' uses undefined class 'std::basic\_ostringstream\<char,std::char\_traits\<char\>,std::allocator\<char\>\>'**

- Simply add *\#include \<sstream\>* to the includes of the code

<!-- TODO/Possible Improvements: -->

&nbsp;

<!--    
         - Setting up *Macros* that have each of the libraries paths dlls, and libs (these can be used to have simpler strings for the configurations that deal with the libraries - C/C++ additonal include, Linker additional library, ) - GLM, GLFW, GLEW

        - Create an empty default project with all the premade configurations so that I can do the exercises more easily (without needing to configure everything for every single project)
-->