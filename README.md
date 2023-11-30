# MarkTech
A Game Framework

# How to build
Run the GenerateProjectFiles.bat to generate the .sln and .vcxproj files
Open MarkTech.sln and build the Engine project first then build the Game Project next.
There will be a Build folder generated in the root directory where the build is containing the game exe and engine dll.
Next create a GameInfo.ini file in the build folder where the game exe is. 
Make sure to fill out the apporpriate data for your game in the section [MarkTech.GameInfo]
Here is an example:
```
[MarkTech.GameInfo]
Game.GameName=MarkTechGame
Game.RawShaderPath=Bin/Shaders/Source/
Game.ShaderPath=Bin/Shaders/
Game.ContentPath=Content/
Game.ConfigPath=Configs/
```
