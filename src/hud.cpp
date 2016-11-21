#include <scene.h>

/**
 * Carrega os dados do objeto em um buffer.
 * @param object_name nome do arquivo onde se encontram os dados.
 */
void HUD::loadData(const std::string& text_name){

	glGenSamplers(1, &samp);
	glSamplerParameteri(samp, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samp, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindSampler(text_target, samp);
	
	glActiveTexture(GL_TEXTURE);
	KTX_error_code_t error = ktxLoadTextureN((base_text_location + text_name).c_str(), &text_gl_name, &text_target, NULL, NULL, NULL, NULL, NULL);
	verifyTextError(error);
}

/**
 * Contrutor vazio.
 */
HUD::HUD(){}

void HUD::draw(){
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(text_target, text_gl_name);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

}