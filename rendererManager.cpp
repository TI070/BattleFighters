
#include "rendererManager.h"

ID3D11VertexShader* RendererManager::unlitTex_VertexShader;
ID3D11InputLayout* RendererManager::unlitTex_VertexLayout;

ID3D11VertexShader* RendererManager::vertexLight_VertexShader;
ID3D11InputLayout* RendererManager::vertexLight_VertexLayout;

ID3D11PixelShader* RendererManager::unlitTex_PixelShader;
ID3D11PixelShader* RendererManager::vertexLight_PixelShader;


ID3D11VertexShader* RendererManager::gradationVertexShader;
ID3D11InputLayout* RendererManager::gradationVertexLayout;
ID3D11PixelShader* RendererManager::gradationPixelShader;


ID3D11VertexShader* RendererManager::gradationColorVertexShader[GC_MAX];
ID3D11InputLayout* RendererManager::gradationColorVertexLayout[GC_MAX];
ID3D11PixelShader* RendererManager::gradationColorPixelShader[GC_MAX];

ID3D11VertexShader* RendererManager::instancingVertexShader;
ID3D11InputLayout* RendererManager::instancingVertexLayout;
ID3D11PixelShader* RendererManager::instancingPixelShader;

