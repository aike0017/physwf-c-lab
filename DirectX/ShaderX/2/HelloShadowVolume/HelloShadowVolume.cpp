#include "HelloShadowVolume.h"

HelloShadowVolume::HelloShadowVolume()
{

}

HelloShadowVolume::~HelloShadowVolume()
{

}


HRESULT HelloShadowVolume::Frame()
{
	FrameMove(0.0f);
	Render();
	return S_OK;
}

HRESULT HelloShadowVolume::OneTimeSceneInit()
{
	IDirect3D8* d3d = Direct3DCreate8(D3D_SDK_VERSION);
	if (!d3d)
	{
		MessageBox(0, L"Direct3DCreate8 error", 0, 0);
		return E_FAIL;
	}
	m_spD3D.reset(d3d, [](IDirect3D8* d3d){ d3d->Release(); });

	return S_OK;
}

HRESULT HelloShadowVolume::InitDeviceObjects()
{
	m_dpps.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_dpps.BackBufferCount = 1;
	m_dpps.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_dpps.BackBufferWidth = m_iWidth;
	m_dpps.BackBufferHeight = m_iHeight;
	m_dpps.EnableAutoDepthStencil = TRUE;
	m_dpps.Flags = 0;
	m_dpps.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	m_dpps.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_dpps.hDeviceWindow = m_hWnd;
	m_dpps.Windowed = true;
	m_dpps.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_dpps.SwapEffect = D3DSWAPEFFECT_DISCARD;

	return RestoreDeviceObjects();
}

HRESULT HelloShadowVolume::RestoreDeviceObjects()
{
	HRESULT hr;
	IDirect3DDevice8* device;
	hr = m_spD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_dpps,
		&device);
	if (FAILED(hr))
	{
		MessageBox(0, L"CreateDevice failed", 0, 0);
		return E_FAIL;
	}
	m_spDevice.reset(device, [](IDirect3DDevice8* device){ device->Release(); });

	m_spDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_spDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_spDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	D3DVIEWPORT8 viewport = { 0, 0, m_iWidth, m_iHeight };
	m_spDevice->SetViewport(&viewport);

	D3DXVECTOR3 eye(0.0f, 0.0f, 30.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_mtView, &eye, &target, &up);

	D3DXMatrixPerspectiveFovLH(&m_mtProj, 0.2*D3DX_PI, (float)m_iWidth / (float)m_iHeight, 1.0f, 100.f);

	m_cPlaneTint = { 0.7f, 0.6f, 0.4f, 1.0f };


	ID3DXMesh* plane;
	//D3DXCreatePolygon(m_spDevice.get(), 2.0f, 4, &plane, NULL);
	CreatePlane(m_spDevice.get(), 15.0f, 10, &plane);
	//D3DXCreateSphere(m_spDevice.get(), 1.0f,20,20, &plane, NULL);

	IDirect3DVertexBuffer8* vb;
	IDirect3DIndexBuffer8* ib;
	plane->GetVertexBuffer(&vb);
	plane->GetIndexBuffer(&ib);
	m_spPlaneVB.reset(vb, [](IDirect3DVertexBuffer8* vb){vb->Release(); });
	m_spPlaneIB.reset(ib, [](IDirect3DIndexBuffer8* ib){ib->Release(); });
	m_dwPlaneNumVertices = plane->GetNumVertices();
	m_dwPlaneNumFaces = plane->GetNumFaces();
	
	plane->Release();

	DWORD decl[] = {
		D3DVSD_STREAM(0),
		D3DVSD_REG(0, D3DVSDT_FLOAT3),
		D3DVSD_REG(3, D3DVSDT_FLOAT3),
		D3DVSD_END()
	};
	hr = CreateVSFromBinFile(m_spDevice.get(), decl, L"plane.vso", &m_dwPlaneVSH);
	if (FAILED(hr))
	{
		MessageBox(0, 0, L"CreateVSFromBinFile failed", 0);
		return E_FAIL;
	}
	hr = CreatePSFromBinFile(m_spDevice.get(), L"plane.pso", &m_dwPlanePSH);
	if (FAILED(hr))
	{
		MessageBox(0, 0, L"CreatePSFromBinFile failed", 0);
		return E_FAIL;
	}

	D3DXMATRIX Rx, Tz;
	D3DXMatrixRotationX(&Rx, D3DX_PI*0.5f);
	D3DXMatrixTranslation(&Tz, 0.0f, -3.0f, 0.0f);
	m_mtPlaneWorld = Rx * Tz;

	ID3DXMesh* occluder;
	CreateOccluder(m_spDevice.get(), &occluder);
	IDirect3DVertexBuffer8* vbOccluder;
	IDirect3DIndexBuffer8* ibOccluder;
	occluder->GetVertexBuffer(&vbOccluder);
	occluder->GetIndexBuffer(&ibOccluder);
	m_spOccluderVB.reset(vbOccluder, [](IDirect3DVertexBuffer8* vb){vb->Release(); });
	m_spOccluderIB.reset(ibOccluder, [](IDirect3DIndexBuffer8* ib){ib->Release(); });
	m_dwOccluderNumVertices = occluder->GetNumVertices();
	m_dwOccluderNumFaces = occluder->GetNumFaces();
	occluder->Release();

	hr = CreateVSFromBinFile(m_spDevice.get(), decl, L"occluder.vso", &m_dwOccluderVSH);
	if (FAILED(hr))
	{
		MessageBox(0, 0, L"CreateVSFromBinFile failed", 0);
		return E_FAIL;
	}
	hr = CreatePSFromBinFile(m_spDevice.get(), L"occluder.pso", &m_dwOccluderPSH);
	if (FAILED(hr))
	{
		MessageBox(0, 0, L"CreatePSFromBinFile failed", 0);
		return E_FAIL;
	}
	m_cOccluderTint = { 0.3f, 0.0f, 0.8f, 1.0f };
	D3DXMATRIX Rz, T;
	D3DXMatrixTranslation(&T, 5.1f * cosf(0.5), 0.0f, 5.1f * sinf(0.5));
	D3DXMatrixIdentity(&m_mtVolumeWorld);
	D3DXMatrixRotationZ(&Rz, 0.5f);
	m_mtOccluderWorld = T * Rz;

	ID3DXMesh* volume;
	CreateVolume(m_spDevice.get(), &volume);
	IDirect3DVertexBuffer8* vbVolume;
	IDirect3DIndexBuffer8* ibVolume;
	volume->GetVertexBuffer(&vbVolume);
	volume->GetIndexBuffer(&ibVolume);
	m_spVolumeVB.reset(vbVolume, [](IDirect3DVertexBuffer8* vb){vb->Release(); });
	m_spVolumeIB.reset(ibVolume, [](IDirect3DIndexBuffer8* ib){ib->Release(); });
	m_dwVolumeNumVertices = volume->GetNumVertices();
	m_dwVolumeNumFaces = volume->GetNumFaces();
	volume->Release();
	
	hr = CreateVSFromBinFile(m_spDevice.get(), decl, L"volume.vso", &m_dwVolumeVSH);
	if (FAILED(hr))
	{
		MessageBox(0, 0, L"CreateVSFromBinFile failed", 0);
		return E_FAIL;
	}
	hr = CreatePSFromBinFile(m_spDevice.get(), L"volume.pso", &m_dwVolumePSH);
	if (FAILED(hr))
	{
		MessageBox(0, 0, L"CreatePSFromBinFile failed", 0);
		return E_FAIL;
	}
	m_cVolumeTint = { 0.7f, 0.0f, 0.0f, 1.0f };

	D3DXMATRIX Sx;
	D3DXMatrixIdentity(&m_mtVolumeWorld);
	D3DXMatrixScaling(&Sx, 6.0f, 1.0f, 1.0f);
	D3DXMatrixRotationZ(&Rz, 0.5f);
	m_mtVolumeWorld = Sx * Rz;

	return S_OK;
}

HRESULT HelloShadowVolume::DeleteDeviceObjects()
{
	return S_OK;
}

HRESULT HelloShadowVolume::Render()
{
	m_spDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0, 1.0f, 0);
	if (SUCCEEDED( m_spDevice->BeginScene()))
	{
		
		m_spDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		D3DXMATRIX temp = m_mtPlaneWorld*m_mtView*m_mtProj;
		D3DXMatrixTranspose(&temp, &temp);
		m_spDevice->SetVertexShaderConstant(0, &temp, 4);
		m_spDevice->SetPixelShaderConstant(0, m_cPlaneTint, 1);
		m_spDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_spDevice->SetVertexShader(m_dwPlaneVSH);
		m_spDevice->SetPixelShader(m_dwPlanePSH);
		
		m_spDevice->SetStreamSource(0, m_spPlaneVB.get(), sizeof Vertex);
		m_spDevice->SetIndices(m_spPlaneIB.get(),0);
		m_spDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_dwPlaneNumVertices, 0, m_dwPlaneNumFaces);

		//m_spDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		temp = m_mtOccluderWorld*m_mtView*m_mtProj;
		D3DXMatrixTranspose(&temp, &temp);
		m_spDevice->SetVertexShaderConstant(0, &temp, 4);
		m_spDevice->SetPixelShaderConstant(0, m_cOccluderTint, 1);
		m_spDevice->SetVertexShader(m_dwOccluderVSH);
		m_spDevice->SetPixelShader(m_dwOccluderPSH);
		m_spDevice->SetStreamSource(0, m_spOccluderVB.get(), sizeof Vertex);
		m_spDevice->SetIndices(m_spOccluderIB.get(), 0);
		m_spDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_dwOccluderNumVertices, 0, m_dwOccluderNumFaces);

		//m_spDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_spDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		temp = m_mtVolumeWorld*m_mtView*m_mtProj;
		D3DXMatrixTranspose(&temp, &temp);
		m_spDevice->SetVertexShaderConstant(0, &temp, 4);
		m_spDevice->SetPixelShaderConstant(0, m_cVolumeTint, 1);
		m_spDevice->SetVertexShader(m_dwVolumeVSH);
		m_spDevice->SetPixelShader(m_dwVolumePSH);
		m_spDevice->SetStreamSource(0, m_spVolumeVB.get(), sizeof Vertex);
		m_spDevice->SetIndices(m_spVolumeIB.get(), 0);
		m_spDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_dwVolumeNumVertices, 0, m_dwVolumeNumFaces);

		m_spDevice->EndScene();
	}
	m_spDevice->Present(0, 0, 0, 0);
	return S_OK;

}

HRESULT HelloShadowVolume::FrameMove(FLOAT)
{
	D3DXMATRIX Ry;
	D3DXMatrixRotationY(&Ry, 0.01f);
	//D3DXMatrixMultiply(&m_mtVolumeWorld, &m_mtVolumeWorld, &Ry);
	
	
	return S_OK;

}

HRESULT HelloShadowVolume::FinalCleanup()
{
	return S_OK;

}

HRESULT HelloShadowVolume::CreateVolume(IDirect3DDevice8* pDevice, ID3DXMesh** ppOut)
{
	ID3DXMesh* pMesh;
	D3DXCreateBox(pDevice, 2.0f, 2.0f, 2.0f, &pMesh, NULL);
	*ppOut = pMesh;
	return S_OK;
}

HRESULT HelloShadowVolume::CreateOccluder(IDirect3DDevice8* pDevice, ID3DXMesh** ppOut)
{
	ID3DXMesh* pMesh;
	D3DXCreateBox(pDevice, 2.0f, 2.0f, 2.0f, &pMesh, NULL);
	*ppOut = pMesh;
	return S_OK;
}
