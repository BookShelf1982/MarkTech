#include "D3D11Renderer.h"
#include "Configs.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image\stb_image.h"


namespace MarkTech
{
	CD3D11Renderer* CD3D11Renderer::g_pd3dRenderer = new CD3D11Renderer();

	bool CD3D11Renderer::InitRenderer(HWND hwnd)
	{
		szSourcePath = MGameInfo::GetGameInfo()->szRawShaderPath;
		szCompiledPath = MGameInfo::GetGameInfo()->szShaderPath;

		DXGI_SWAP_CHAIN_DESC SCDesc;
		ZeroMemory(&SCDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		SCDesc.BufferDesc.RefreshRate.Numerator = 0;
		SCDesc.BufferDesc.RefreshRate.Denominator = 1;
		SCDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		SCDesc.SampleDesc.Count = 1;
		SCDesc.SampleDesc.Quality = 0;
		SCDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SCDesc.BufferCount = 1;
		SCDesc.OutputWindow = hwnd;
		SCDesc.Windowed = MUserSettings::GetUserSettings()->bVSWindowed;


		D3D_FEATURE_LEVEL feature_level;
		UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined( DEBUG )
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		//Create device
		HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, NULL, 0, D3D11_SDK_VERSION, &SCDesc, &m_pSwapChain, &m_pd3dDevice, &feature_level, &m_pd3dDeviceContext);
		assert(S_OK == hr && m_pSwapChain && m_pd3dDevice && m_pd3dDeviceContext);

#ifdef EBUG
		hr = m_pd3dDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_pDebug); //Create debug interface
		assert(SUCCEEDED(hr));
#endif / DEBUG

		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer);	//Get backbuffer from swap cahin
		assert(SUCCEEDED(hr));

		hr = m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer, NULL, &m_pMainRenderTargetView); //Create main render target using the back buffer
		assert(SUCCEEDED(hr));

		m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, NULL);

		m_pBackBuffer->Release(); //Destroy backbuffer

		MVertex v[4] = {
			{-1.0f, -1.0f, 0.0f, 0.0f, 1.0f},
			{-1.0f,  1.0f, 0.0f, 0.0f, 0.0f},
			{1.0f,  1.0f, 0.0f, 1.0f, 0.0f},
			{1.0f, -1.0f, 0.0f, 1.0f, 1.0f}
		};

		D3D11_BUFFER_DESC VSDesc;
		ZeroMemory(&VSDesc, sizeof(D3D11_BUFFER_DESC));
		VSDesc.ByteWidth = sizeof(MVertex) * 4;
		VSDesc.Usage = D3D11_USAGE_DEFAULT;
		//VSDesc.Usage = D3D11_USAGE_DYNAMIC;
		VSDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VSDesc.CPUAccessFlags = 0;
		//VSDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		VSDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA VSSrc;
		ZeroMemory(&VSSrc, sizeof(D3D11_SUBRESOURCE_DATA));
		VSSrc.pSysMem = v;

		hr = m_pd3dDevice->CreateBuffer(&VSDesc, &VSSrc, &m_pMainVertexBuffer); //Allocate memory for vertex buffer
		assert(SUCCEEDED(hr));

		DWORD indices[6] = {
			0,1,2,
			0,2,3
		};

		D3D11_BUFFER_DESC ISDesc;
		ZeroMemory(&ISDesc, sizeof(D3D11_BUFFER_DESC));
		ISDesc.ByteWidth = sizeof(DWORD) * 6;
		ISDesc.Usage = D3D11_USAGE_DEFAULT;
		ISDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ISDesc.CPUAccessFlags = 0;
		ISDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA ISSrc;
		ZeroMemory(&ISSrc, sizeof(D3D11_SUBRESOURCE_DATA));
		ISSrc.pSysMem = indices;

		hr = m_pd3dDevice->CreateBuffer(&ISDesc, &ISSrc, &m_pMainIndexBuffer); //Allocate mempry for index buffer
		assert(SUCCEEDED(hr));

		int nWidth, nHeight, nChannels;
		void* img = stbi_load(MGameInfo::GetGameInfo()->szImage, &nWidth, &nHeight, &nChannels, 4);

		D3D11_TEXTURE2D_DESC TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		TextureDesc.Width = nWidth;
		TextureDesc.Height = nHeight;
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		D3D11_SUBRESOURCE_DATA imgSrc;
		ZeroMemory(&imgSrc, sizeof(D3D11_SUBRESOURCE_DATA));
		imgSrc.pSysMem = img;
		imgSrc.SysMemPitch = nWidth*4;

		hr = m_pd3dDevice->CreateTexture2D(&TextureDesc, &imgSrc, &m_pTexture);
		assert(SUCCEEDED(hr));

		hr = m_pd3dDevice->CreateShaderResourceView(m_pTexture, NULL, &m_pTextureView);
		assert(SUCCEEDED(hr));

		D3D11_SAMPLER_DESC ImageSamplerDesc = {};

		ImageSamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		ImageSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		ImageSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		ImageSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		ImageSamplerDesc.MipLODBias = 0.0f;
		ImageSamplerDesc.MaxAnisotropy = 1;
		ImageSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		ImageSamplerDesc.MinLOD = -FLT_MAX;
		ImageSamplerDesc.MaxLOD = FLT_MAX;

		hr = m_pd3dDevice->CreateSamplerState(&ImageSamplerDesc, &m_pTextureSampler);
		assert(SUCCEEDED(hr));

		CreateShaders();

		return true;
	}

	void CD3D11Renderer::RenderFrame(HWND hwnd)
	{
		float color[4] = { 0.0f,0.0f,0.25f,1.0f };

		m_pd3dDeviceContext->ClearRenderTargetView(m_pMainRenderTargetView, color);
		
		RECT WinRect;
		GetClientRect(hwnd, &WinRect);
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (FLOAT)(WinRect.right - WinRect.left);
		viewport.Height = (FLOAT)(WinRect.bottom - WinRect.top);
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;
		m_pd3dDeviceContext->RSSetViewports(1, &viewport);

		m_pd3dDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
		m_pd3dDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

		m_pd3dDeviceContext->PSSetShaderResources(0, 1, &m_pTextureView);
		m_pd3dDeviceContext->PSSetSamplers(0, 1, &m_pTextureSampler);

		m_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pd3dDeviceContext->IASetInputLayout(m_pInputLayout);
		m_pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pMainVertexBuffer, &stride, &offset);
		m_pd3dDeviceContext->IASetIndexBuffer(m_pMainIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		m_pd3dDeviceContext->DrawIndexed(6, 0, 0);

		m_pSwapChain->Present(1, 0);
	}

	void CD3D11Renderer::CreateShaders()
	{
		ID3DBlob* pVsBlob = NULL, * pPsBlob = NULL;
		pVsBlob = GetShaderBytecodeFromFile(L"DefaultVertex.hlsl", L"Vertex.cso", "vs_5_0", "main");
		pPsBlob = GetShaderBytecodeFromFile(L"Albedo.hlsl", L"Pixel.cso", "ps_5_0", "main");

		HRESULT hr = m_pd3dDevice->CreateVertexShader(
			pVsBlob->GetBufferPointer(),
			pVsBlob->GetBufferSize(),
			NULL,
			&m_pVertexShader);
		assert(SUCCEEDED(hr));


		hr = m_pd3dDevice->CreatePixelShader(
			pPsBlob->GetBufferPointer(),
			pPsBlob->GetBufferSize(),
			NULL,
			&m_pPixelShader);
		assert(SUCCEEDED(hr));


		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		hr = m_pd3dDevice->CreateInputLayout(
			inputElementDesc,
			ARRAYSIZE(inputElementDesc),
			pVsBlob->GetBufferPointer(),
			pVsBlob->GetBufferSize(),
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
				OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
				if (ErrorBlob) { ErrorBlob->Release(); }
				if (blob) { blob->Release(); }
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

	void CD3D11Renderer::DestroyRenderer()
	{
		m_pSwapChain->Release();
		m_pd3dDevice->Release();
		m_pd3dDeviceContext->Release();
		m_pDebug->Release();
		m_pMainRenderTargetView->Release();
		m_pMainVertexBuffer->Release();
		m_pMainIndexBuffer->Release();
		m_pVertexShader->Release();
		m_pPixelShader->Release();
		m_pTexture->Release();
		m_pTextureSampler->Release();
		m_pTextureView->Release();
		m_pInputLayout->Release();

		delete g_pd3dRenderer;
	}
}