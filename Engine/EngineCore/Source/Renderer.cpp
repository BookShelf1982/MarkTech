#include "Renderer.h"
ERendererAPI IRenderer::m_API = ERendererAPI::None;

CSceneBuffer::CSceneBuffer()
	:m_pSceneElements(nullptr), m_nNumPushedSceneElements(0), m_nNumSceneElements(0)
{
}

CSceneBuffer::~CSceneBuffer()
{
}

void CSceneBuffer::Initialize(size_t initalSize)
{
	m_pSceneElements = new MSceneElement[initalSize];
	m_nNumSceneElements = initalSize;
}

void CSceneBuffer::Release()
{
	delete[] m_pSceneElements;
	m_nNumSceneElements = 0;
	m_nNumPushedSceneElements = 0;
}

void CSceneBuffer::PushSceneElement(MSceneElement element)
{
	m_pSceneElements[m_nNumPushedSceneElements] = element;
	m_nNumPushedSceneElements++;
}

void CSceneBuffer::Clear()
{
	m_pSceneElements = nullptr;
	m_nNumPushedSceneElements = 0;
	m_nNumSceneElements = 0;
}
