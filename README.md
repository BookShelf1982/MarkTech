# MarkTech

A Game Framework

# How to build

1. Run the GenerateProjectFiles.bat to generate the .sln and .vcxproj files.  
2. Open MarkTech.sln and build the ResourceCompiler and ResourceCompilerCmd projects first then build the Game and Engine Projects next.  
3. There will be a Build folder generated in the root directory where the build is containing the game exe and engine dll.  
4. Next create a GameInfo.ini file in the build folder where the game exe is.  
5. Make sure to fill out the apporpriate data for your game in the section [MarkTech.GameInfo]  
Here is an example:
```
[MarkTech.GameInfo]
Game.GameName=MarkTechGame
Game.RawShaderPath=Bin/Shaders/Source/
Game.ShaderPath=Bin/Shaders/
Game.ContentPath=Content/
Game.ConfigPath=Configs/
Game.Model=teapot.mmdl
Game.Image=Textures/wood.mtex
```

# How to run

### Create a model

In order to run the game exe you must create a model and texture file using the ResourceCompiler Command-Line app. 
You can find the exe in the compiled build folder in ```C:\Path\To\Root\Dir\Build\Bin\ResourceCompilerCmd.exe``` 
1. Open a Command-Line Prompt and drag the exe onto the command prompt. Then type ```-mmdl``` 
that will tell the resource compiler that you want to make a model.  
2. Next drag a ```.obj``` file onto the command prompt to tell the resource compiler what model you want to import.  
3. Next specify an output file **Note that the output file must have the .mmdl extension!**  

Now you can press enter and wait for it to compile. Here is an example of what we just did:
```C:\Path\To\Root\Dir\Build\Bin\ResourceCompilerCmd.exe -mmdl C:\Path\To\OBJ\File C:\Path\To\Output\File```

### Create a texture

Now we will create a texture. 
1. Open a Command-Line Prompt and drag Resource Compiler onto the command prompt. Then type ```-mtex```  
2. Next drag a image file such as ```.png .jpg .bmp``` onto the command prompt **Note that the image's dimensions must be divisible by 4.**  
3. Next specify the output file **Note that the output file must have the .mtex file extension!**  
4. Next type ```-mips``` and specify how many mipmaps you want.  
5. Next type ```-compression``` and specify what type of compression do you want the only available options are ```DXT1 DXT5 NONE```  

Now you can press enter and wait for it to compile. Here is an example of what we just did:
```C:\Path\To\Root\Dir\Build\Bin\ResourceCompilerCmd.exe -mtex C:\Path\To\Image\File C:\Path\To\Output\File -mips 4 -compression DXT1```

### Compile shaders using Resource Compiler

You can compile shaders using Resource Compiler
1. Open a Command-Line Prompt and drag Resource Compiler onto the command prompt. Then type ```-mfx```  
2. Next drag a ```.hlsl``` file onto the command prompt  
3. Next specify the output file **Note that the output file must have the .mfx file extension!**  
4. Next type ```-compiler``` and specify the compiler version and type you want. The only avalable options are ```ps_5_0 vs_5_0```  
5. Next type ```-entrypoint``` and specify the shader entrypoint  

Now you can press enter and wait for it to compile. Here is an example of what we just did:
```C:\Path\To\Root\Dir\Build\Bin\ResourceCompilerCmd.exe -mfx C:\Path\To\HLSL\File C:\Path\To\Output\File -compiler vs_5_0 -entrypoint main```

Now in your ```GameInfo.ini``` file you can specify where the model you are using in ```Game.Model``` and you can specify where the texture you are using in ```Game.Image```. 
The main search path for assets is where ever you specified you content path.
