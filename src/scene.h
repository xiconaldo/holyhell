#ifndef SCENE_ELEMENTS_H
#define SCENE_ELEMENTS_H

#include <GL/glew.h>
#include <loader.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * Conjunto de objetos que representam objetos da cena, tais
 * como personagens, câmera, fontes de luz e objetos estáticos.
 *
 * TODO:
 * 		-> Classe para objetos estáticos, que deve conter os vértices,
 * 		as normais e as coordenadas de textura dos objetos. Também é
 * 		necessário armazenar o local em disco onde se encontra estas
 * 		informações, além dos arquivos de textura. Propriedades
 * 		referentes a iluminação também ficam neste objeto. Além disso,
 * 		o objeto deve guardar os seus fatores de translação, rotação 
 * 		e escala, necessárias à criação da model.
 *
 * 		+ Classe para a câmera, que deve conter a posição da câmera
 * 		e os vetores que formam seu espaço, informações necessárias para
 * 		contruir a matriz view.
 *
 * 		- Classe para fontes de luz, informando o tipo de luz e os 
 * 		parâmetros da mesma, além de sua posição e posssível direção.
 *
 * 		- Classe personagem, semelhante aos objetos estáticos, com a 
 * 		diferença de apresentar métodos para controle do personagem 
 * 		através de interação.
 */

class Camera{
public:
	Camera();

	Camera(const glm::vec3 &position,
		   const glm::vec3 &lookAt = glm::vec3(0.0f, 0.0f, -1.0f),
		   const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f));

	Camera(float positionX, float positionY, float positionZ,
		   float lookAtX = 0.0f, float lookAtY = 0.0f, float lookAtZ = -1.0f,
		   float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f);

	void setCamera(const glm::vec3 &position,
			   	   const glm::vec3 &lookAt = glm::vec3(0.0f, 0.0f, -1.0f),
			   	   const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f));

	void setCamera(float positionX, float positionY, float positionZ,
				   float lookAtX = 0.0f, float lookAtY = 0.0f, float lookAtZ = -1.0f,
				   float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f);

	void setPosition(const glm::vec3 &position);

	void setPosition(float positionX, float positionY, float positionZ);

	void setLookAt(const glm::vec3 &lookAt = glm::vec3(0.0f, 0.0f, -1.0f));

	void setLookAt(float lookAtX = 0.0f, float lookAtY = 0.0f, float lookAtZ = -1.0f);

	void setUp(const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f));

	void setUp(float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f);

	void translate(float x, float y, float z);

	void rotate(int xc, int yc, int zc, float degrees);

	void resetMatrix();

private:
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 position;
	glm::vec3 lookAt;
	glm::vec3 up;

	glm::mat4 m_view;
	glm::mat4 base_view;

	void updateView();
	void updateBaseView();
};

#endif