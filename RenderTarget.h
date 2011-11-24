#pragma once
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11tex.h>

class RenderTarget
{
public:
	bool Init(ID3D11Device* device,int width,int height,DXGI_FORMAT format,bool createDepth);
	void Release();
	ID3D11RenderTargetView* GetTargetView();
	ID3D11DepthStencilView* GetDepthView();
	ID3D11ShaderResourceView* GetResource();
	void SetTarget(ID3D11DeviceContext* devicecontext);
	void Clear(ID3D11DeviceContext* devicecontext,float r,float g,float b,float a);
private:
	ID3D11RenderTargetView* view;
	ID3D11ShaderResourceView* resource;
	bool CreateDepthStencilView(ID3D11Device* device, int width,int height);
	int width;
	int height;
	ID3D11DepthStencilView* renderTargetDepth;
};
