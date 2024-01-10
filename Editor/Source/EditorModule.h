#pragma once

class IEditorModule
{
public:
	virtual ~IEditorModule() = 0;
	virtual bool InitModule() = 0;
	virtual void ExecuteModule() = 0;
};