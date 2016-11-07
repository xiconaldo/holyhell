#ifndef SCENE_ELEMENTS_H
#define SCENE_ELEMENTS_H

#include <GL/glew.h>
#include <loader.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define  KTX_OPENGL 1
#include <ktx.h>
#include <input.h>

/**
 * Conjunto de objetos que representam objetos da cena, tais
 * como personagens, câmera, fontes de luz e objetos estáticos.
 *
 * TODO:
 * 		+ Classe para objetos estáticos, que deve conter os vértices,
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
 * 		+ Classe personagem, semelhante aos objetos estáticos, com a 
 * 		diferença de apresentar métodos para controle do personagem 
 * 		através de interação.
 */

// Camera
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

	void bindProgram(GLuint program);

	void adjust(GLuint program); // NOVO

	void loadBuffers(GLuint program); // NOVO

private:

	glm::vec3 position;
	glm::vec3 lookAt;
	glm::vec3 up;

	glm::mat4 m_view;
	GLuint view_location;
	GLuint program;
	GLuint shader_storage_buffer;

	//void updateView();
	void updateViewMatrix();
};



// Object
class Object{
public:
	Object();
	void loadData(const std::string& object_name, const std::string& text_name = "");
	void localRotate(int xc, int yc, int zc, float degrees);
	void localTranslate(float x, float y, float z);
	void localScale(float x, float y, float z);
	void localScale(float xyz);
	void rotate(int xc, int yc, int zc, float degrees);
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	void scale(float xyz);
	float x();
	float y();
	float z();
	void bindProgram(GLuint program);
	void resetMatrix();
	void draw();
	void verifyTextError(KTX_error_code_t code);
	static void setBaseDataLocation(const std::string& location);
	static void setBaseTextLocation(const std::string& location);

protected:
	
	std::string data_location;
	static std::string base_data_location;
	static std::string base_text_location;
	int triangle_count;
	std::vector<Vertex> data;

	glm::mat4 m_model = glm::mat4(1.0f);

	GLuint model_location;
	GLuint vertex_location;
	GLuint normal_location;
	GLuint uv_location;

	GLuint vertex_buffer;
	GLuint vao;
	GLuint program;

	GLuint text_gl_name;
	GLuint text_map;
	GLuint samp;
	GLenum text_target;

	void updateModelMatrix();
};


// Terreno
class Terrain : public Object{
public:
	Terrain();
	void loadData(const std::string& object_name, const std::string& text_name, const std::string& text_map_name);
	void draw();

private:
};


//Grama
class Grass : public Object{
public :
	Grass();
	void draw();
};

// Player
class Player : public Object{
public:
	Player();
	void draw();
	void adjustCamera(GLuint program);
};


#endif