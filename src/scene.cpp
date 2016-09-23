#include <scene.h>

// Camera
Camera::Camera() : 
	Camera(glm::vec3(0.0f, 0.0f, 0.0f))
{}

Camera::Camera(const glm::vec3 &position,
	   		   const glm::vec3 &lookAt,
	   		   const glm::vec3 &up) :

	position(position),
	lookAt(lookAt),
	up(up)
{
	updateBaseView();
}

Camera::Camera(float positionX, float positionY, float positionZ,
			   float lookAtX, float lookAtY, float lookAtZ,
			   float upX, float upY, float upZ) :

	position(positionX, positionY, positionZ),
	lookAt(lookAtX, lookAtY, lookAtZ),
	up(upX, upY, upZ)
{
	updateBaseView();
}

void Camera::setCamera(const glm::vec3 &position,
				   	   const glm::vec3 &lookAt,
				   	   const glm::vec3 &up){

	this->position = position;
	this->lookAt = lookAt;
	this->up = up;
	updateBaseView();
}

void Camera::setCamera(float positionX, float positionY, float positionZ,
					   float lookAtX, float lookAtY, float lookAtZ,
					   float upX, float upY, float upZ){

	position = glm::vec3(positionX, positionY, positionZ);
	lookAt = glm::vec3(lookAtX, lookAtY, lookAtZ);
	up = glm::vec3(upX, upY, upZ);
	updateBaseView();
}

void Camera::setPosition(const glm::vec3 &position){
	this->position = position;
	updateBaseView();
}

void Camera::setPosition(float positionX, float positionY, float positionZ){
	position = glm::vec3(positionX, positionY, positionZ);
	updateBaseView();
}

void Camera::setLookAt(const glm::vec3 &lookAt){
	this->lookAt = lookAt;
	updateBaseView();
}

void Camera::setLookAt(float lookAtX, float lookAtY, float lookAtZ){
	lookAt = glm::vec3(lookAtX, lookAtY, lookAtZ);
	updateBaseView();
}

void Camera::setUp(const glm::vec3 &up){
	this->up = up;
	updateBaseView();
}

void Camera::setUp(float upX, float upY, float upZ){
	up = glm::vec3(upX, upY, upZ);
	updateBaseView();
}

void Camera::translate(float x, float y, float z){
	translation += glm::vec3(x, y, z);
	updateView();
}

void Camera::rotate(int xc, int yc, int zc, float degrees){
	if(xc) rotation.x += degrees;
	if(yc) rotation.y += degrees;
	if(zc) rotation.z += degrees;
	updateView();
}

void Camera::resetMatrix(){
	translation = glm::vec3();
	rotation = glm::vec3();
	m_view = base_view;
}

void Camera::updateBaseView(){
	glm::vec3 xc, yc, zc, pos;
	zc = glm::normalize(position - lookAt);
	xc = glm::normalize(glm::cross(up, zc));
	yc = glm::cross(zc, xc);
	pos = -position;

	base_view[0] = glm::vec4(xc, glm::dot(xc, pos));
	base_view[1] = glm::vec4(yc, glm::dot(yc, pos));
	base_view[2] = glm::vec4(zc, glm::dot(zc, pos));
	base_view[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	base_view = glm::transpose(base_view);

	updateView();
}

void Camera::updateView(){

	m_view = glm::translate(-translation) *
			 glm::rotate(-rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
			 glm::rotate(-rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
			 glm::rotate(-rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
			 base_view;

	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(m_view));
}