#include "RenderTarget.h"


bool RenderTarget::Init(ID3D11Device* device,int width,int height,DXGI_FORMAT format,bool createDepth)
{
	HRESULT hr;
	
	//create render target texture
	//crea la texture per il render target
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	ID3D11Texture2D* targetTex;
	hr=device->CreateTexture2D(&desc,NULL,&targetTex);

	if(hr!=S_OK)
		return false;

	//create render target view from texture
	//crea il render target view dalla texture
	D3D11_RENDER_TARGET_VIEW_DESC DescRT;
	DescRT.Format = format;
	DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	DescRT.Texture2DArray.MipSlice = 0;

	hr=device->CreateRenderTargetView( targetTex, &DescRT, &view);
	
	
	if(hr!=S_OK)
	{
		targetTex->Release();
		return false;
	}

	// Create the shader-resource view from the texture
	//crea lo shader resource view dalla texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = desc.Format;
	srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;

	hr=device->CreateShaderResourceView( targetTex, &srDesc, &resource );

	targetTex->Release();

	if(hr!=S_OK)
	{
		view->Release();
		return false;
	}

	//crea il depth stencil
	//create depth stencil
	renderTargetDepth=NULL;
	if(createDepth)
	{
		if(!CreateDepthStencilView(device, width,height))
		{
			view->Release();
			resource->Release();
			return false;
		}
	}
	
	this->width=width;
	this->height=height;
	return true;
}


bool RenderTarget::CreateDepthStencilView(ID3D11Device* device, int width,int height)
{
	// crea depth stencil
	//create the depth stencil
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    
	ID3D11Texture2D *pDepthBuffer;

	if( FAILED(device->CreateTexture2D( &descDepth, NULL, &pDepthBuffer )))
        return false;

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

	//depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//depthStencilViewDesc.Texture2D.MipSlice = 0;


    HRESULT hr= device->CreateDepthStencilView( pDepthBuffer, &descDSV, &renderTargetDepth );
	if(FAILED(hr))
		return false;
    pDepthBuffer->Release();    

	return true;
}


void RenderTarget::Release()
{
	if(this->view)
	{
		view->Release();
		resource->Release();
	}
}

ID3D11RenderTargetView* RenderTarget::GetTargetView()
{
	return view;
}

ID3D11ShaderResourceView* RenderTarget::GetResource()
{
	return resource;
}

ID3D11DepthStencilView* RenderTarget::GetDepthView()
{
	return renderTargetDepth;
}

void RenderTarget::SetTarget(ID3D11DeviceContext* devicecontext)
{
	//set the targets
	//imposta i targets
	devicecontext->OMSetRenderTargets( 1, &this->view,this->renderTargetDepth);

	//imposta il viewport
	//set viewport
	//imposta il viewport, l'area visibile
	//imposta il viewport
    D3D11_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    devicecontext->RSSetViewports( 1, &vp );
}

void RenderTarget::Clear(ID3D11DeviceContext* devicecontext, float r,float g,float b,float a)
{
	//pulisce i 2 buffer
	//clear the 2 buffer
	float ClearColor[4] = { r, g, b, a};
	devicecontext->ClearRenderTargetView(this->view,ClearColor);
	if(renderTargetDepth)
		devicecontext->ClearDepthStencilView(this->renderTargetDepth,D3D11_CLEAR_DEPTH,1,0);
}