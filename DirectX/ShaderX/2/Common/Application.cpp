#include "Application.h"
#include <sstream>

CD3DApplication::CD3DApplication()
{
	Instance = (this);
}

CD3DApplication::~CD3DApplication()
{

}

HRESULT CD3DApplication::Initialze(HINSTANCE hInstance, int nCmdShow)
{
	LoadString(hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RACORX, m_szWindowClass, MAX_LOADSTRING);
	m_hInst = hInstance;
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow))
	{
		return S_FALSE;
	}
	HRESULT rc = OneTimeSceneInit();
	if (FAILED(rc)) {
		MessageBox(m_hWnd, L"OneTimeSceneInit error!",L"Error",0);
		return S_FALSE;
	}
	rc = InitDeviceObjects();
	if (FAILED(rc)) {
		MessageBox(m_hWnd, L"InitDeviceObjects error!", L"Error", 0);
		return S_FALSE;
	}
	return S_OK;
}

HRESULT CD3DApplication::CreateVSFromBinFile(IDirect3DDevice8* device, DWORD* dwDecl, TCHAR* strVSPath, DWORD* pHVS)
{
	TCHAR strFullPath[1024];
	TCHAR* strShortName;
	DWORD dwLen;

	dwLen = GetFullPathName(strVSPath, sizeof(strFullPath) / sizeof(TCHAR), strFullPath, &strShortName);

	if (dwLen == 0 || sizeof(strFullPath) / sizeof(TCHAR) <= dwLen) {
		MessageBox(m_hWnd, L"GetFullPathName Failed", L"Error", 0);
		return E_FAIL;
	}

	HANDLE hFile, hMap;
	char szBuffer[128];
	DWORD* pdwVS;
	HRESULT hr;

	hFile = CreateFile(strFullPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE != hFile) {
		if (GetFileSize(hFile, 0) > 0) {
			hMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
		} else {
			CloseHandle(hFile);
			MessageBox(m_hWnd, L"CreateVSFromBinFile: File is empty!", L"Error", 0);
			return E_FAIL;
		}
	} else{
		MessageBox(m_hWnd, L"CreateVSFromBinFile: Can't Find file!", L"Error", 0);
		return E_FAIL;
	}
	pdwVS = (DWORD*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	hr = device->CreateVertexShader(dwDecl, pdwVS, pHVS, 0);
	if (FAILED(hr)) {
		D3DXGetErrorStringA(hr, szBuffer, sizeof(szBuffer));
		MessageBox(m_hWnd, L"CreateVertexShader Failed!", L"Error", 0);
		return hr;
	}
	UnmapViewOfFile(pdwVS);
	CloseHandle(hMap);
	CloseHandle(hFile);
	return S_OK;
}

HRESULT CD3DApplication::CreatePSFromBinFile(IDirect3DDevice8* device, TCHAR* strVSPath, DWORD* phPS)
{
	TCHAR strFullPath[1024];
	TCHAR* strShortName;
	DWORD dwLen;

	dwLen = GetFullPathName(strVSPath, sizeof(strFullPath) / sizeof(TCHAR), strFullPath, &strShortName);

	if (dwLen == 0 || sizeof(strFullPath) / sizeof(TCHAR) <= dwLen) {
		MessageBox(m_hWnd, L"GetFullPathName Failed", L"Error", 0);
		return E_FAIL;
	}

	HANDLE hFile, hMap;
	char szBuffer[128];
	DWORD* pdwPS;
	HRESULT hr;

	hFile = CreateFile(strFullPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE != hFile) {
		if (GetFileSize(hFile, 0) > 0) {
			hMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
		}
		else {
			CloseHandle(hFile);
			MessageBox(m_hWnd, L"CreatePSFromBinFile: File is empty!", L"Error", 0);
			return E_FAIL;
		}
	}
	else{
		MessageBox(m_hWnd, L"CreatePSFromBinFile: Can't Find file!", L"Error", 0);
		return E_FAIL;
	}
	pdwPS = (DWORD*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	hr = device->CreatePixelShader(pdwPS, phPS);
	if (FAILED(hr)) {
		D3DXGetErrorStringA(hr, szBuffer, sizeof(szBuffer));
		MessageBox(m_hWnd, L"CreateVertexShader Failed!", L"Error", 0);
		return hr;
	}
	UnmapViewOfFile(pdwPS);
	CloseHandle(hMap);
	CloseHandle(hFile);
	return S_OK;
}

HRESULT CD3DApplication::CreateSphereMesh(LPDIRECT3DDEVICE8 pDevice, LPD3DXMESH* ppSphere)
{
	HRESULT hr;
	LPD3DXMESH pSphere, pClone, pOut;
	hr = D3DXCreateSphere(pDevice, 1.0f, 40, 40, &pSphere, NULL);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"D3DXCreateSphere Failed!", L"Error", 0);
		return E_FAIL;
	}
	hr = pSphere->CloneMeshFVF(D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1), pDevice, &pClone);
	pSphere->Release();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"CloneMeshFVF Failed!", L"Error", 0);
		return E_FAIL;
	}
	
	hr = D3DXComputeNormals(pClone, NULL);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"D3DXComputeNormals Failed!", L"Error", 0);
		return E_FAIL;
	}
	struct Vertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
		float bx, by, bz;
	};

	Vertex* pVertex = NULL;
	if (SUCCEEDED(pClone->LockVertexBuffer(0, reinterpret_cast<BYTE**>(&pVertex))))
	{
		DWORD numVertices = pClone->GetNumVertices();
		for (DWORD i = 0; i < numVertices; ++i)
		{
			//D3DXVECTOR3 temp;
			//D3DXVec3Normalize(&temp, &pVertex->normal);
			pVertex->u = atan2f(pVertex->ny, pVertex->nx) / (2.0f*D3DX_PI) + 0.5f;
			pVertex->v = 0.5f - asinf(-pVertex->nz) / D3DX_PI;
			std::stringstream log;
			log << i << ":\n";
			log << "x:" << pVertex->x << "\t\ty:" << pVertex->y << "\t\tz:" << pVertex->z << "\n";
			log << "nx:" << pVertex->nx << "\t\tny:" << pVertex->ny << "\t\tnz:" << pVertex->nz << "\n";
			//if (pVertex->u >= 1.0f || pVertex->v >= 1.0f)
				log << "u:" << pVertex->u << "\t\tv:" << pVertex->v << "\n";
			//if (pVertex->tangent.x != 0.0f || pVertex->tangent.y != 0.0f || pVertex->tangent.z != 0.0f)
				log << "bx:" << pVertex->bx << "\t\tby:" << pVertex->by << "\t\tbz:" << pVertex->bz << "\n";
			log << "--------------------------\n";
			//OutputDebugStringA(log.str().c_str());
			++pVertex;
		}
		pClone->UnlockVertexBuffer();
	}
	hr = pClone->CloneMeshFVF(D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1), pDevice, &pOut);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"CloneMeshFVF Failed!", L"Error", 0);
		return E_FAIL;
	}
	hr = D3DXComputeTangent(pClone, 0, pOut, 1, D3DX_COMP_TANGENT_NONE, true, NULL);
	if (FAILED(hr))
	{
		WCHAR szBuffer[512] = { 0 };
		D3DXGetErrorString(hr, szBuffer, sizeof(szBuffer));
		OutputDebugString(szBuffer);
		if (hr == D3DERR_INVALIDCALL) {
			MessageBox(m_hWnd, L"D3DXComputeTangent D3DERR_INVALIDCALL!", L"Error", 0);
		}
		else if (hr == E_OUTOFMEMORY) {
			MessageBox(m_hWnd, L"D3DXComputeTangent E_OUTOFMEMORY!", L"Error", 0);
		}
		return E_FAIL;
	}
	if (SUCCEEDED(pOut->LockVertexBuffer(0, reinterpret_cast<BYTE**>(&pVertex))))
	{
		DWORD numVertices = pOut->GetNumVertices();
		
		for (DWORD i = 0; i < numVertices; ++i)
		{
			std::stringstream log;
			log << i << ":\n";
			log << "x:" << pVertex->x << "\t\ty:" << pVertex->y << "\t\tz:" << pVertex->z << "\n";
			log << "nx:" << pVertex->nx << "\t\tny:" << pVertex->ny << "\t\tnz:" << pVertex->nz << "\n";
			log << "u:" << pVertex->u << "\t\tv:" << pVertex->v << "\n";
			//if (pVertex->tangent.x != 0.0f || pVertex->tangent.y != 0.0f || pVertex->tangent.z != 0.0f)
				log << "tx:" << pVertex->bx << "\t\tty:" << pVertex->by << "\t\ttz:" << pVertex->bz << "\n";
			log << "--------------------------\n";
			OutputDebugStringA(log.str().c_str());
			++pVertex;
		}
		pOut->UnlockVertexBuffer();
	}
	pClone->Release();
	DWORD* ab;
	if (SUCCEEDED(pOut->LockAttributeBuffer(D3DLOCK_DISCARD, &ab)))
	{
		DWORD numFace = pOut->GetNumFaces();
		for (DWORD iFace = 0; iFace < numFace; ++iFace)
		{
			ab[iFace] = 0;
		}
		pOut->UnlockAttributeBuffer();
	}
	*ppSphere = pOut;
	return S_OK;
}



HRESULT CD3DApplication::CreatePlane(LPDIRECT3DDEVICE8 pDevice, float fSize, WORD strip, LPD3DXMESH* ppPlane)
{
	ID3DXMesh* plane;
	WORD numfaces = strip*strip * 2;
	DWORD numvertices = (strip + 1)*(strip + 1);
	DWORD decl[] = {
		D3DVSD_STREAM(0),
		D3DVSD_REG(D3DVSDE_POSITION, D3DVSDT_FLOAT3),
		D3DVSD_REG(D3DVSDE_NORMAL, D3DVSDT_FLOAT3),
		D3DVSD_END()
	};
	D3DXCreateMesh(
		numfaces,
		numvertices,
		D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
		decl, 
		pDevice, 
		&plane);
	struct Vertex
	{
		Vertex(float  _x, float _y, float _z) { x = _x; y = _y; z = _z; }
		float x, y, z;
		float nx, ny, nz;
	};
	Vertex* v;
	float unitSize = fSize / strip;
	float offset = -fSize / 2;
	if (SUCCEEDED(plane->LockVertexBuffer(0, (BYTE**)&v)))
	{
		std::stringstream log;
		for (DWORD y = 0; y <= strip; ++y)
		{
			for (DWORD x = 0; x <= strip; ++x)
			{
				log << "vertex:" << y*(strip + 1) + x  << "\n";
				v[y*(strip + 1) + x] = Vertex(offset + x*unitSize, offset + y*unitSize, 0.0f);
				v[y*(strip + 1) + x].nz = 1.0f;
			}
		}
		//OutputDebugStringA(log.str().c_str());
	}
	WORD *indices;
	if (SUCCEEDED(plane->LockIndexBuffer(0, (BYTE**)&indices)))
	{
		std::stringstream log;
		for (WORD f = 0; f < numfaces/2; ++f)
		{
			WORD row = f / strip;
			WORD col = f % strip;
			WORD i0 = col + (strip+1)*row;
			WORD i1 = i0 + 1;
			WORD i2 = i0 + strip + 1;
			WORD i3 = i2 + 1;
			indices[f * 6 + 0] = i0;
			indices[f * 6 + 1] = i1;
			indices[f * 6 + 2] = i2;
			indices[f * 6 + 3] = i2;
			indices[f * 6 + 4] = i1;
			indices[f * 6 + 5] = i3;
			log << "row:" << row << " col:" << col << "\n";
			log << "indices:" << i0 << " " << i1 << " " << i2 << " " << i2 << " " << i1 << " " << i3 << "\n";
		}
		//OutputDebugStringA(log.str().c_str());
	}
	*ppPlane = plane;
	plane->AddRef();
	return S_OK;
}

BOOL CD3DApplication::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hwnd = CreateWindow(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if (!hwnd)
	{
		DWORD error = GetLastError();
		return FALSE;
	}
	m_hWnd = hwnd;
	RECT size;
	GetWindowRect(m_hWnd, &size);
	m_iWidth = size.right - size.left;
	m_iHeight = size.bottom - size.top;
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	return TRUE;
}

ATOM CD3DApplication::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RACORX));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_RACORX);
	wcex.lpszClassName = m_szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

LRESULT CD3DApplication::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		//m_spInput->KeyDown(static_cast<unsigned int>(wParam));
		return 0;
		break;
	case WM_KEYUP:
		return 0;
		break;
	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return CD3DApplication::Instance->MessageHandler(hWnd, msg, wParam, lParam);
	}
	}
}

CD3DApplication* CD3DApplication::Instance = nullptr;

HRESULT CD3DApplication::Run()
{
	MSG msg;
	bool done = false, result = true;

	HACCEL hAccelTable;
	ZeroMemory(&msg, sizeof MSG);

	hAccelTable = LoadAccelerators(m_hInst, MAKEINTRESOURCE(IDC_RACORX));

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		Frame();
		Sleep(10);
	}
	return S_OK;
}

HRESULT CD3DApplication::ShutDown()
{
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;
	m_hInst = NULL;
	return S_OK;
}
