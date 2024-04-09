#include "CompilerApp.h"

CCompilerApp::CCompilerApp()
{
}

CCompilerApp::~CCompilerApp()
{
}

void CCompilerApp::OnInit()
{
	IModuleLoader loader;
	loader.LoadModule("ResourceCompiler.dll");
	printf("Initalized!");
}

void CCompilerApp::OnUpdate()
{
}

void CCompilerApp::OnQuit()
{
}
