/////////////////////////////////////////////////////////////////////////////
// Name:        Camera.h
// Controls the assembly and render of various drawing objects used by the
// draw routines in the TestGLContext class
/////////////////////////////////////////////////////////////////////////////

#ifndef _CAMERA_H
#define _CAMERA_H

// Std. Includes
//#include <vector>

// GL Includes
//#include <GL/glew.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  3.0f;
const GLfloat SENSITIVTY =  0.25f;
const GLfloat ZOOM       =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    //glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    glm::vec3 orig_pos;  // our original position...this should probably be const.

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
        GLfloat yaw = YAW, GLfloat pitch = PITCH);
 
    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);
 
    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime = (0.1f / SPEED));

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    // // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    // void ProcessMouseScroll(GLfloat yoffset);

    void ResetCameraView();  // resets the camera view back to the original position

    void setCameraState(bool state) {cameraIsActive = state; return;}
    bool getCameraState() {return cameraIsActive;}

    void setPos(glm::vec3 pos) {Position = pos; return;} // sets the camera position
    glm::vec3 getPos() { return Position; }              // return ths camera position

    // view related matrices
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

private:
    glm::vec3 Position;

    bool cameraIsActive;
    void updateCameraVectors();
};

#endif // _CAMERA_H