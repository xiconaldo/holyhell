#include <scene.h>

/**
 * Construtor da classe. Define os atributos padrão da câmera.
 */
Camera::Camera() : 
	Camera(glm::vec3(0.0f, 0.0f, 0.0f))
{}

/**
 * Construtor da classe. Define os atributos da câmera.
 * @param position posição da câmera.
 * @param lookAt   ponto para o qual a câmera aponta.
 * @param up       direção para a qual a parte de cima da câmera deve apontar.
 */
Camera::Camera(const glm::vec3 &position,
	   		   const glm::vec3 &lookAt,
	   		   const glm::vec3 &up) :

	position(position),
	lookAt(lookAt),
	up(up)
{
	resetMatrix();
}

/**
 * Construtor da classe. Define os atributos da câmera.
 * @param positionX componente x da posição da câmera.
 * @param positionY componente y da posição da câmera.
 * @param positionZ componente z da posição da câmera.
 * @param lookAtX   componente x do ponto para o qual a câmera aponta.
 * @param lookAtY   componente y do ponto para o qual a câmera aponta.
 * @param lookAtZ   componente z do ponto para o qual a câmera aponta.
 * @param upX       componente x do vetor direção para a qual a parte 
 *                  de cima da câmera deve apontar.
 * @param upY       componente y do vetor direção para a qual a parte 
 *                  de cima da câmera deve apontar.
 * @param upZ       componente z do vetor direção para a qual a parte 
 *                  de cima da câmera deve apontar.
 */
Camera::Camera(float positionX, float positionY, float positionZ,
			   float lookAtX, float lookAtY, float lookAtZ,
			   float upX, float upY, float upZ) :

	position(positionX, positionY, positionZ),
	lookAt(lookAtX, lookAtY, lookAtZ),
	up(upX, upY, upZ)
{
	resetMatrix();
}

/**
 * Redefine os atributos da câmera.
 * @param position posição da câmera.
 * @param lookAt   ponto para o qual a câmera aponta.
 * @param up       direção para a qual a parte de cima da câmera deve apontar.
 */
void Camera::setCamera(const glm::vec3 &position,
				   	   const glm::vec3 &lookAt,
				   	   const glm::vec3 &up){

	this->position = position;
	this->lookAt = lookAt;
	this->up = up;
	resetMatrix();
}

/**
 * Redefine os atributos da câmera.
 * @param positionX componente x da posição da câmera.
 * @param positionY componente y da posição da câmera.
 * @param positionZ componente z da posição da câmera.
 * @param lookAtX   componente x do ponto para o qual a câmera aponta.
 * @param lookAtY   componente y do ponto para o qual a câmera aponta.
 * @param lookAtZ   componente z do ponto para o qual a câmera aponta.
 * @param upX       componente x do vetor direção para a qual a parte 
 *                  de cima da câmera deve apontar.
 * @param upY       componente y do vetor direção para a qual a parte 
 *                  de cima da câmera deve apontar.
 * @param upZ       componente z do vetor direção para a qual a parte 
 *                  de cima da câmera deve apontar.
 */
void Camera::setCamera(float positionX, float positionY, float positionZ,
					   float lookAtX, float lookAtY, float lookAtZ,
					   float upX, float upY, float upZ){

	position = glm::vec3(positionX, positionY, positionZ);
	lookAt = glm::vec3(lookAtX, lookAtY, lookAtZ);
	up = glm::vec3(upX, upY, upZ);
	resetMatrix();
}

/**
 * Redefine a posição da câmera.
 * @param position posição da câmera.
 */
void Camera::setPosition(const glm::vec3 &position){
	this->position = position;
	resetMatrix();
}

/**
 * Redefine a posição da câmera.
 * @param positionX componente x da posição da câmera.
 * @param positionY componente y da posição da câmera.
 * @param positionZ componente z da posição da câmera.
 */
void Camera::setPosition(float positionX, float positionY, float positionZ){
	position = glm::vec3(positionX, positionY, positionZ);
	resetMatrix();
}

/**
 * Redefine o ponto para o qual a câmera aponta.
 * @param lookAt ponto para o qual a câmera aponta.
 */
void Camera::setLookAt(const glm::vec3 &lookAt){
	this->lookAt = lookAt;
	resetMatrix();
}

/**
 * Redefine o ponto para o qual a câmera aponta.
 * @param lookAtX   componente x do ponto para o qual a câmera aponta.
 * @param lookAtY   componente y do ponto para o qual a câmera aponta.
 * @param lookAtZ   componente z do ponto para o qual a câmera aponta.
 */
void Camera::setLookAt(float lookAtX, float lookAtY, float lookAtZ){
	lookAt = glm::vec3(lookAtX, lookAtY, lookAtZ);
	resetMatrix();
}

/**
 * Redefine a direção que indica a parte de cima da câmera.
 * @param up direção para a qual a parte de cima da câmera deve apontar.
 */
void Camera::setUp(const glm::vec3 &up){
	this->up = up;
	resetMatrix();
}

/**
 * Redefine a direção que indica a parte de cima da câmera.
 * @param upX       componente x do vetor direção para a qual a parte 
 *                  de cima da câmera deve apontar.
 * @param upY       componente y do vetor direção para a qual a parte 
 *                  de cima da câmera deve apontar.
 * @param upZ       componente z do vetor direção para a qual a parte 
 *                  de cima da câmera deve apontar.
 */
void Camera::setUp(float upX, float upY, float upZ){
	up = glm::vec3(upX, upY, upZ);
	resetMatrix();
}

/**
 * Seta a translação que deve ser feita pela câmera em relação a 
 * seus eixos (view space). A translação ocorre antes da 
 * rotação.
 * @param x translação em x em graus.
 * @param y translação em y em graus.
 * @param z translação em z em graus.
 */
void Camera::translate(float x, float y, float z){
	m_view = glm::translate(-glm::vec3(x, y, z)) * m_view;
	updateViewMatrix();
}

/**
 * Seta a rotação que deve ser feita pela câmera em relação a seus 
 * eixos (view spcace). A rotação ocorre após a translação.
 * @param xc      indica se a câmera deve (!0) ou não deve (0) ser rotacionada
 *                no eixo x.
 * @param yc      indica se a câmera deve (!0) ou não deve (0) ser rotacionada
 *                no eixo y.
 * @param zc      indica se a câmera deve (!0) ou não deve (0) ser rotacionada
 *                no eixo z.
 * @param degrees ângulo em radianos que a câmera deve rotacionar nos eixos especificados.
 */
void Camera::rotate(int xc, int yc, int zc, float degrees){
	m_view = glm::rotate(-degrees, glm::vec3(xc, yc, zc)) * m_view;
	updateViewMatrix();
}

/**
 * Atualiza a matriz gerada pelos atributos da câmera (posição, direção 
 * e up) e pelas transformações de translação e rotação definidas.
 */
void Camera::resetMatrix(){
	glm::vec3 xc, yc, zc, pos;
	zc = glm::normalize(position - lookAt);
	xc = glm::normalize(glm::cross(up, zc));
	yc = glm::cross(zc, xc);
	pos = -position;

	m_view[0] = glm::vec4(xc, glm::dot(xc, pos));
	m_view[1] = glm::vec4(yc, glm::dot(yc, pos));
	m_view[2] = glm::vec4(zc, glm::dot(zc, pos));
	m_view[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	m_view = glm::transpose(m_view);

	updateViewMatrix();
}

/**
 * Faz com que a câmera seja efetivamente usada no programa especificado.
 * @param program identificador do programa ao qual a câmera será adicionada.
 */
void Camera::bindProgram(GLuint program){
	view_location = glGetUniformLocation(program, "view");
	updateViewMatrix();
}

/**
 * Associa os dados da cópia local da matriz view com a 
 * variável uniforme especificada previamente.
 */
void Camera::updateViewMatrix(){
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(m_view));
}