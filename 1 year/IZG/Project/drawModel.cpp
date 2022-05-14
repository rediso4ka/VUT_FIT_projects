/*!
 * @file
 * @brief This file contains functions for model rendering
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */
#include <student/drawModel.hpp>
#include <student/gpu.hpp>


void drawNode(GPUContext& ctx, Node const& node, Model const& model, glm::mat4 jednotkovaMatrice) {

	if (node.mesh >= 0) {
		Mesh const& mesh = model.meshes[node.mesh];

		ctx.prg.uniforms.uniform[1].m4 = jednotkovaMatrice * node.modelMatrix;
		ctx.prg.uniforms.uniform[2].m4 = glm::transpose(glm::inverse(jednotkovaMatrice * node.modelMatrix));
		ctx.prg.uniforms.uniform[5].v4 = mesh.diffuseColor;

		ctx.vao.vertexAttrib[0] = mesh.position;
		ctx.vao.vertexAttrib[1] = mesh.normal;
		ctx.vao.vertexAttrib[2] = mesh.texCoord;

		if (mesh.diffuseTexture >= 0) {
			ctx.prg.uniforms.textures[0] = model.textures[mesh.diffuseTexture];
			ctx.prg.uniforms.uniform[6].v1 = 1.f;
		}
		else {
			ctx.prg.uniforms.textures[0] = Texture{};
			ctx.prg.uniforms.uniform[6].v1 = 0.f;
		}

		draw(ctx, mesh.nofIndices);
	}

	for (size_t i = 0; i < node.children.size(); ++i) {
		drawNode(ctx, node.children[i], model, jednotkovaMatrice * node.modelMatrix);
	}

}

 /**
  * @brief This function renders a model
  *
  * @param ctx GPUContext
  * @param model model structure
  * @param proj projection matrix
  * @param view view matrix
  * @param light light position
  * @param camera camera position (unused)
  */
  //! [drawModel]
void drawModel(GPUContext& ctx, Model const& model, glm::mat4 const& proj, glm::mat4 const& view, glm::vec3 const& light, glm::vec3 const& camera) {
	(void)ctx;
	(void)model;
	(void)proj;
	(void)view;
	(void)light;
	(void)camera;
	/// \todo Tato funkce vykreslí model.<br>
	/// Vaším úkolem je správně projít model a vykreslit ho pomocí funkce draw (nevolejte drawImpl, je to z důvodu testování).
	/// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
    ctx.prg.vertexShader = drawModel_vertexShader;
	ctx.prg.fragmentShader = drawModel_fragmentShader;

	glm::mat4 jednotkovaMatrice = glm::mat4(1.f);
	for (size_t i = 0; i < model.roots.size(); ++i) {
		drawNode(ctx, model.roots[i], model, jednotkovaMatrice);
	}
}
//! [drawModel]

/**
 * @brief This function represents vertex shader of texture rendering method.
 *
 * @param outVertex output vertex
 * @param inVertex input vertex
 * @param uniforms uniform variables
 */
 //! [drawModel_vs]
void drawModel_vertexShader(OutVertex& outVertex, InVertex const& inVertex, Uniforms const& uniforms) {
	(void)outVertex;
	(void)inVertex;
	(void)uniforms;
	/// \todo Tato funkce reprezentujte vertex shader.<br>
	/// Vaším úkolem je správně trasnformovat vrcholy modelu.
	/// Bližší informace jsou uvedeny na hlavní stránce dokumentace.

}
//! [drawModel_vs]

/**
 * @brief This functionrepresents fragment shader of texture rendering method.
 *
 * @param outFragment output fragment
 * @param inFragment input fragment
 * @param uniforms uniform variables
 */
 //! [drawModel_fs]
void drawModel_fragmentShader(OutFragment& outFragment, InFragment const& inFragment, Uniforms const& uniforms) {
	(void)outFragment;
	(void)inFragment;
	(void)uniforms;
	/// \todo Tato funkce reprezentujte fragment shader.<br>
	/// Vaším úkolem je správně obarvit fragmenty a osvětlit je pomocí lambertova osvětlovacího modelu.
	/// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
}
//! [drawModel_fs]

