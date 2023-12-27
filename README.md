# MarkTech

A Game Framework

# How to build

1. Clone this repository or download a zip file  
2. Run the ```GenerateProjectFiles.bat``` to generate the .sln and .vcxproj files.  
3. Open MarkTech.sln and build the Game project first and then the EngineCore Project next.  
4. After building there should be a ```Build``` folder in the root directory.  
5. Inside the ```Build``` folder is a ```Bin``` folder which contains a ```Modules``` folder which contains the ```EngineCore.dll``` file. 
6. Inside the ```Modules``` folder you must create a ```Modules.ini``` file. This will tell the game what modules to load.  
Here is an example of what the ```Modules.ini``` file should look like.  
```
;Modules.ini file
[Modules]
LoadedModuleList[0]=EngineCore
```
After that you should be able to see a window pop up when opening ```Game.exe``` in the ```Build``` folder.  