#pragma once

#include "main.h"
#include "renderer.h"

enum GRADATIONCOLOR {

	GC_RED,
	GC_GREEN,
	GC_BLUE,
	GC_YELLOW,
	GC_MAX
};

class RendererManager {

public:

	static ID3D11VertexShader* unlitTex_VertexShader;
	static ID3D11InputLayout* unlitTex_VertexLayout;

	static ID3D11VertexShader* vertexLight_VertexShader;
	static ID3D11InputLayout* vertexLight_VertexLayout;

	static ID3D11PixelShader* unlitTex_PixelShader;
	static ID3D11PixelShader* vertexLight_PixelShader;

	static ID3D11VertexShader* gradationVertexShader;
	static ID3D11InputLayout* gradationVertexLayout;
	static ID3D11PixelShader* gradationPixelShader;

	static ID3D11VertexShader* gradationColorVertexShader[GC_MAX];
	static ID3D11InputLayout* gradationColorVertexLayout[GC_MAX];
	static ID3D11PixelShader* gradationColorPixelShader[GC_MAX];


	static ID3D11VertexShader* instancingVertexShader;
	static ID3D11InputLayout* instancingVertexLayout;
	static ID3D11PixelShader* instancingPixelShader;



	static void Init() {

		Renderer::CreateVertexShader(&unlitTex_VertexShader, &unlitTex_VertexLayout, "unlitTextureVS.cso");
		Renderer::CreatePixelShader(&unlitTex_PixelShader, "unlitTexturePS.cso");

		
		Renderer::CreateVertexShader(&vertexLight_VertexShader, &vertexLight_VertexLayout, "vertexLightingVS.cso");
		Renderer::CreatePixelShader(&vertexLight_PixelShader, "vertexLightingPS.cso");

		Renderer::CreateVertexShader(&gradationVertexShader,&gradationVertexLayout, "gradationShaderVS.cso");
		Renderer::CreatePixelShader(&gradationPixelShader, "gradationShaderPS.cso");

		Renderer::CreateVertexShader(&gradationColorVertexShader[GC_RED], &gradationColorVertexLayout[GC_RED], "gradationRedShaderVS.cso");
		Renderer::CreatePixelShader(&gradationColorPixelShader[GC_RED], "gradationRedShaderPS.cso");

		Renderer::CreateVertexShader(&gradationColorVertexShader[GC_GREEN], &gradationColorVertexLayout[GC_GREEN], "gradationGreenShaderVS.cso");
		Renderer::CreatePixelShader(&gradationColorPixelShader[GC_GREEN], "gradationGreenShaderPS.cso");

		Renderer::CreateVertexShader(&gradationColorVertexShader[GC_BLUE], &gradationColorVertexLayout[GC_BLUE], "gradationBlueShaderVS.cso");
		Renderer::CreatePixelShader(&gradationColorPixelShader[GC_BLUE], "gradationBlueShaderPS.cso");

		Renderer::CreateVertexShader(&gradationColorVertexShader[GC_YELLOW], &gradationColorVertexLayout[GC_YELLOW], "gradationYellowShaderVS.cso");
		Renderer::CreatePixelShader(&gradationColorPixelShader[GC_YELLOW], "gradationYellowShaderPS.cso");

		Renderer::CreateVertexShader(&instancingVertexShader, &instancingVertexLayout, "instancingShaderVS.cso");
		Renderer::CreatePixelShader(&instancingPixelShader, "instancingShaderPS.cso");

	}


	static void UnlitShaderSet(ID3D11VertexShader* pVS, ID3D11InputLayout* pIL, ID3D11PixelShader* pPS) {

		pVS = unlitTex_VertexShader;
		pIL = unlitTex_VertexLayout;
		pPS = unlitTex_PixelShader;

	}

	static ID3D11VertexShader* GetUnlitVS() { return unlitTex_VertexShader;}
	static ID3D11InputLayout* GetUnlitIL() { return unlitTex_VertexLayout; }
	static ID3D11PixelShader* GetUnlitPS() { return unlitTex_PixelShader; }

	static ID3D11VertexShader* GetlitVS() { return vertexLight_VertexShader; }
	static ID3D11InputLayout* GetlitIL() { return vertexLight_VertexLayout; }
	static ID3D11PixelShader* GetlitPS() { return vertexLight_PixelShader; }

	static ID3D11VertexShader* GetGdVS() { return gradationVertexShader; }
	static ID3D11InputLayout* GetGdIL() { return gradationVertexLayout; }
	static ID3D11PixelShader* GetGdPS() { return gradationPixelShader; }

	
	static ID3D11VertexShader* GetGdColorVS(GRADATIONCOLOR s_gc) { return gradationColorVertexShader[s_gc]; }
	static ID3D11InputLayout* GetGdColorIL(GRADATIONCOLOR s_gc) { return gradationColorVertexLayout[s_gc]; }
	static ID3D11PixelShader* GetGdColorPS(GRADATIONCOLOR s_gc) { return gradationColorPixelShader[s_gc]; }

	static ID3D11VertexShader* GetInstanceVS() { return instancingVertexShader; }
	static ID3D11InputLayout* GetInstanceIL() { return instancingVertexLayout; }
	static ID3D11PixelShader* GetInstancePS() { return instancingPixelShader; }


};