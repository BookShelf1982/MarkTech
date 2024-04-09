#pragma once
#include <Application.h>

struct test_t
{
	uint32_t a;
	uint16_t b;
	uint64_t c;
};

class CSandboxApp : public IMarkTechApplication
{
public:
	CSandboxApp();
	~CSandboxApp();

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnQuit() override;

private:
	IMarkTechEngine* m_pEngine;
	CTSharedPointer<test_t> masterText;
};