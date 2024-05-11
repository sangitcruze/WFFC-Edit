#pragma once
#include "DisplayChunk.h"
#include "DisplayObject.h"


using namespace DirectX::SimpleMath;

struct InputCommands;

enum class CameraMode {
    FreeCamera,
    FocusCamera
};

class CameraController
{
public:
    // Constructor with default parameters for move speed and camera rotation rate
    CameraController(float moveSpeed = 0.2f, float rotationRate = 2.3f);
    
    // Method to update the camera based on input commands
    void Update(const InputCommands& inputCommands);

    

    // Update the camera in Focus camera mode
    void FocusOnObject(const InputCommands& input_commands, std::vector<int> selectedObjects, std::vector<DisplayObject>* m_displayList);

    
  
    // Getter method to retrieve the view matrix & camera position
    Matrix GetViewMatix() const { return m_viewMatrix; }
    Vector3 GetPosition() const { return m_position; }
    // Camera mode
    CameraMode m_cameraMode;
private:
    // Initialization helper methods
    void Init();

    // Update helper methods
    void CreateUpdate(float deltaTime);

    // Handle mouse input for camera rotation
    void HandleMouse(const InputCommands& inputCommands);


   

    // Member variables
    float m_moveSpeed;          // Movement speed of the camera
    float m_rotationRate;       // Rotation rate of the camera
    float m_distance;
   float m_zoomSpeed;
    Vector3 m_position;         // Position of the camera
    Vector3 m_orientation;      // Orientation of the camera
    Vector3 m_lookAt;           // Point the camera is looking at
    Vector3 m_lookDirection;    // Direction the camera is looking at
    Vector3 m_rightDirection;   // Right vector of the camera
    Vector2 m_oldMouse;         // Previous mouse position
    Vector2 m_newMouse;         // Current mouse position
    Matrix m_viewMatrix;        // View matrix of the camera

    

};
