#include "D3D11Renderer.h"
#include <fstream>
#include "Configs.h"
#include "Level.h"
#include "DirectXTex\DirectXTex.h"

namespace MarkTech
{
	CD3D11Renderer* CD3D11Renderer::g_pd3dRenderer = new CD3D11Renderer();

	bool CD3D11Renderer::InitRenderer(const CWinWindow& window)
	{
		szSourcePath = MGameInfo::GetGameInfo()->szRawShaderPath;
		szCompiledPath = MGameInfo::GetGameInfo()->szShaderPath;

		DXGI_SWAP_CHAIN_DESC SCDesc;
		ZeroMemory(&SCDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		SCDesc.BufferDesc.RefreshRate.Numerator = 0;
		SCDesc.BufferDesc.RefreshRate.Denominator = 1;
		SCDesc.BufferDesc.Width = window.nWidth;
		SCDesc.BufferDesc.Height = window.nHeight;
		SCDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		SCDesc.SampleDesc.Count = 1;
		SCDesc.SampleDesc.Quality = 0;
		SCDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SCDesc.BufferCount = 1;
		SCDesc.OutputWindow = window.GetHWND();
		SCDesc.Windowed = MUserSettings::GetUserSettings()->bVSWindowed;

		D3D_FEATURE_LEVEL feature_level;
		UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined( DEBUG )
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		//Create device
		HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, NULL, 0, D3D11_SDK_VERSION, &SCDesc, &m_pSwapChain, &m_pd3dDevice, &feature_level, &m_pd3dDeviceContext);
		assert(S_OK == hr && m_pSwapChain && m_pd3dDevice && m_pd3dDeviceContext);

#ifdef DEBUG
		hr = m_pd3dDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_pDebug); //Create debug interface
		assert(SUCCEEDED(hr));
#endif // DEBUG

		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer);	//Get backbuffer from swap cahin
		assert(SUCCEEDED(hr));

		hr = m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer, NULL, &m_pMainRenderTargetView); //Create main render target using the back buffer
		assert(SUCCEEDED(hr));

		D3D11_TEXTURE2D_DESC depthStencilBuffDesc;
		ZeroMemory(&depthStencilBuffDesc, sizeof(D3D11_TEXTURE2D_DESC));
		depthStencilBuffDesc.Width = window.nWidth;
		depthStencilBuffDesc.Height = window.nHeight;
		depthStencilBuffDesc.SampleDesc.Count = 1;
		depthStencilBuffDesc.SampleDesc.Quality = 0;
		depthStencilBuffDesc.MipLevels = 1;
		depthStencilBuffDesc.ArraySize = 1;
		depthStencilBuffDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilBuffDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilBuffDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBuffDesc.CPUAccessFlags = 0;
		depthStencilBuffDesc.MiscFlags = 0;

		hr = m_pd3dDevice->CreateTexture2D(&depthStencilBuffDesc, NULL, &m_pDepthStencilBuffer);
		assert(SUCCEEDED(hr));

		hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView);
		assert(SUCCEEDED(hr));

		m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pDepthStencilView);

		m_pBackBuffer->Release(); //Release backbuffer

		D3D11_BUFFER_DESC cbuffDesc;
		ZeroMemory(&cbuffDesc, sizeof(D3D11_BUFFER_DESC));
		cbuffDesc.ByteWidth = sizeof(constBuffer);
		cbuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbuffDesc.CPUAccessFlags = 0;
		cbuffDesc.MiscFlags = 0;

		hr = m_pd3dDevice->CreateBuffer(&cbuffDesc, NULL, &m_pConstBuffer);
		assert(SUCCEEDED(hr));

		D3D11_BUFFER_DESC VSDesc;
		ZeroMemory(&VSDesc, sizeof(D3D11_BUFFER_DESC));
		VSDesc.ByteWidth = sizeof(MVertex) * MAX_VERTICES_PER_DRAWCALL;
		VSDesc.Usage = D3D11_USAGE_DYNAMIC;
		VSDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VSDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		VSDesc.MiscFlags = 0;

		hr = m_pd3dDevice->CreateBuffer(&VSDesc, NULL, &m_pMainVertexBuffer); //Allocate memory for vertex buffer
		assert(SUCCEEDED(hr));

		D3D11_BUFFER_DESC ISDesc;
		ZeroMemory(&ISDesc, sizeof(D3D11_BUFFER_DESC));
		ISDesc.ByteWidth = sizeof(uint32_t) * MAX_INDICES_PER_DRAWCALL;
		ISDesc.Usage = D3D11_USAGE_DYNAMIC;
		ISDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ISDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ISDesc.MiscFlags = 0;

		hr = m_pd3dDevice->CreateBuffer(&ISDesc, NULL, &m_pMainIndexBuffer); //Allocate mempry for index buffer
		assert(SUCCEEDED(hr));

		CreateShaders();

		return true;
	}

	void CD3D11Renderer::RenderFrame(const CWinWindow& window)
	{
		D3D11_MAPPED_SUBRESOURCE VertSubResource;
		ZeroMemory(&VertSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		D3D11_MAPPED_SUBRESOURCE IndSubResource;
		ZeroMemory(&IndSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		float color[4] = {0.0f,0.0f,0.25f,1.0f};

		m_pd3dDeviceContext->ClearRenderTargetView(m_pMainRenderTargetView, color);
		m_pd3dDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (FLOAT)window.nWidth;
		viewport.Height = (FLOAT)window.nHeight;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;
		m_pd3dDeviceContext->RSSetViewports(1, &viewport);

		if(MUserSettings::GetUserSettings()->bVSVSync >= 1)
			m_pSwapChain->Present(1, 0);
		else
			m_pSwapChain->Present(0, 0);


	}

	void CD3D11Renderer::UpdateRender(const CWinWindow& window)
	{
		rcamPosition = DirectX::XMVectorSet(camData.camPos.y, camData.camPos.z, camData.camPos.x, 0.0f);
		rcamTarget = DirectX::XMVectorSet(0.0f, 0.0f, 10.0f, 0.0f);
		rcamUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		camView = DirectX::XMMatrixLookAtLH(rcamPosition, rcamTarget, rcamUp);

		camProjection = DirectX::XMMatrixPerspectiveFovLH(camData.flFov * 3.14f, (float)window.nWidth/ window.nHeight, camData.flNearZ, camData.flFarZ);

		objectWorld = DirectX::XMMatrixIdentity();

		WVP = objectWorld * camView * camProjection;

		constBuffer.WVP = DirectX::XMMatrixTranspose(WVP);	

		m_pd3dDeviceContext->UpdateSubresource(m_pConstBuffer, 0, NULL, &constBuffer, 0, 0);
	}

	void CD3D11Renderer::CreateShaders()
	{
		CAssetHandle VertexShader = GetLevel()->LoadAsset("Vert.mfx", MShader);
		CAssetHandle PixelShader = GetLevel()->LoadAsset("Unlit.mfx", MShader);

		HRESULT hr = m_pd3dDevice->CreateVertexShader(
			reinterpret_cast<CShader*>(VertexShader.GetAssetDataPtr())->m_pShaderByteCode.GetPtr(),
			reinterpret_cast<CShader*>(VertexShader.GetAssetDataPtr())->m_nShaderByteCodeSize,
			NULL,
			&m_pVertexShader
		);
		assert(SUCCEEDED(hr));

		hr = m_pd3dDevice->CreatePixelShader(
			reinterpret_cast<CShader*>(PixelShader.GetAssetDataPtr())->m_pShaderByteCode.GetPtr(),
			reinterpret_cast<CShader*>(PixelShader.GetAssetDataPtr())->m_nShaderByteCodeSize,
			NULL,
			&m_pPixelShader
		);
		assert(SUCCEEDED(hr));

		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		hr = m_pd3dDevice->CreateInputLayout(
			inputElementDesc,
			ARRAYSIZE(inputElementDesc),
			reinterpret_cast<CShader*>(VertexShader.GetAssetDataPtr())->m_pShaderByteCode.GetPtr(),
			reinterpret_cast<CShader*>(VertexShader.GetAssetDataPtr())->m_nShaderByteCodeSize,
			&m_pInputLayout);
		assert(SUCCEEDED(hr));
	}

	ID3DBlob* CD3D11Renderer::GetShaderBytecodeFromFile(LPCWSTR Filename, LPCWSTR CompiledFilename, LPCSTR Compiler, LPCSTR Entrypoint)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined( DEBUG )
		flags |= D3DCOMPILE_DEBUG; // add more debug output
	#endif

		ID3DBlob* blob, * ErrorBlob;

		std::wstringstream FilePath;
		FilePath << szCompiledPath << CompiledFilename;

		std::wstringstream SourceFilePath;
		SourceFilePath << szSourcePath << Filename;

		//Try to get precompiled shader file
		HRESULT hr = D3DReadFileToBlob(FilePath.str().c_str(), &blob);
		if (FAILED(hr))
		{
			//Compile shader source file
			HRESULT hr = D3DCompileFromFile(
				SourceFilePath.str().c_str(),
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				Entrypoint,
				Compiler,
				flags,
				0,
				&blob,
				&ErrorBlob);
			if (FAILED(hr)) {
				assert(false);
			}
			else
			{
				hr = D3DWriteBlobToFile(blob, FilePath.str().c_str(), TRUE);
				if (FAILED(hr))

					assert(false);
				else
					return blob;
			}
		}
		return blob;
	}

	void CD3D11Renderer::UpdateCameraData(const MCameraData& data)
	{
		camData.camPos = data.camPos;
		camData.camTarget = data.camTarget;
		camData.camUp = data.camUp;
		camData.flFarZ = data.flFarZ;
		camData.flNearZ = data.flNearZ;
		camData.flFov = data.flFov;
	}

	void CD3D11Renderer::SubmitModel(CAssetHandle model)
	{
		/*CModel TempModel;
		TempModel.Init(model.GetAssetDataPtr());
		m_SubmittedModels.Push(TempModel);*/
	}

	void CD3D11Renderer::DestroyRenderer()
	{
		m_pSwapChain->Release();
		m_pd3dDevice->Release();
		m_pd3dDeviceContext->Release();
		m_pMainRenderTargetView->Release();

		if(m_pMainVertexBuffer)
			m_pMainVertexBuffer->Release();
		if(m_pMainIndexBuffer)
			m_pMainIndexBuffer->Release();

		m_pVertexShader->Release();
		m_pPixelShader->Release();
		m_pInputLayout->Release();
		m_pDepthStencilBuffer->Release();
		m_pDepthStencilView->Release();
		m_pConstBuffer->Release();

		//m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

#ifdef DEBUG 
		m_pDebug->Release();
#endif //DEBUG

		delete g_pd3dRenderer;
	}
}