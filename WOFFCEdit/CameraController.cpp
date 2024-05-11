#include "CameraController.h"
#include "InputCommands.h"
#include "TextureManager.h"
using namespace DirectX;

CameraController::CameraController(float moveSpeed, float rotationRate) : m_moveSpeed(moveSpeed), m_rotationRate(rotationRate) // Constructor
{
	Init(); // Initialize camera parameters
}
// Initialize camera parameters
void CameraController::Init()
{
	// Set initial camera position
	m_position = Vector3(0.0f, 3.7f, -3.5f);
	m_orientation = Vector3::Zero;
	m_lookAt = Vector3::Zero;
	m_lookDirection = Vector3::Zero;
	m_rightDirection = Vector3::Zero;
}

void CameraController::CreateUpdate(float deltaTime)
{
	m_viewMatrix = Matrix::CreateLookAt(m_position, m_lookAt, Vector3::UnitY);
}

// Update the camera based on input commands
void CameraController::Update(const InputCommands& input_commands)
{
	// Handle mouse input
	HandleMouse(input_commands);



	// Handle rotation input
	if (input_commands.rotRight)
	{
		m_orientation.y -= m_rotationRate;
	}
	if (input_commands.rotLeft)
	{
		m_orientation.y += m_rotationRate;
	}

	// Calculate look direction based on orientation angles
	m_lookDirection.x = sin((m_orientation.y) * XM_PI / 180);
	m_lookDirection.z = cos((m_orientation.y) * XM_PI / 180);

	// Normalize the look direction vector
	m_lookDirection.Normalize();

	// Calculate the right direction vector
	m_lookDirection.Cross(Vector3::UnitY, m_rightDirection);

	// Handle movement input
	if (input_commands.forward)
	{
		m_position += m_lookDirection * m_moveSpeed;
	}
	if (input_commands.back)
	{
		m_position -= m_lookDirection * m_moveSpeed;
	}
	if (input_commands.right)
	{
		m_position += m_rightDirection * m_moveSpeed;
	}
	if (input_commands.left)
	{
		m_position -= m_rightDirection * m_moveSpeed;
	}
	// Update the look at position
	m_lookAt = m_position + m_lookDirection;

	// Update the view matrix
	m_viewMatrix = Matrix::CreateLookAt(m_position, m_lookAt, Vector3::UnitY);
}

// Handle mouse input for camera rotation
void CameraController::HandleMouse(const InputCommands& input_commands)
{
	m_newMouse = Vector2(input_commands.mouseX, input_commands.mouseY);

	// Check if right mouse button is pressed
	if (input_commands.RMB)
	{
		// Calculate mouse movement
		Vector2 difference = m_newMouse - m_oldMouse;
		difference.Normalize();
		// Update orientation based on mouse movement
		if (difference.x != 0 || difference.y != 0)
		{
			// yaw
			m_orientation.y -= m_rotationRate * difference.x;
			// pitch
			m_orientation.x -= m_rotationRate * difference.y;
		}
		// Calculate new look direction based on orientation angles
		float cosP = cos(m_orientation.x * XM_PI / 180);
		float cosY = cos(m_orientation.y * XM_PI / 180);
		float sinP = sin(m_orientation.x * XM_PI / 180);
		float sinY = sin(m_orientation.y * XM_PI / 180);

		m_lookDirection.x = sinY * cosP;
		m_lookDirection.y = sinP;
		m_lookDirection.z = cosP * -cosY;
		// Update the look at position
		m_lookAt = m_position + m_lookDirection;
		// Update the view matrix
		m_viewMatrix = Matrix::CreateLookAt(m_position, m_lookAt, Vector3::UnitY);
	}

	// Update old mouse position
	m_oldMouse = m_newMouse;
}


void CameraController::FocusOnObject(const InputCommands& input_commands, std::vector<int>selectedObjects, std::vector<DisplayObject>* m_displayList)
{
	// Check if the button to activate focus camera mode is held
	if (input_commands.switchCameraMode)
	{

		// Check if there's a selected object to focus on
		if (!selectedObjects.empty())
		{
			// Get the position of the selected object
			Vector3 centerOfRotation = (*m_displayList)[selectedObjects[0]].m_position;

			// Calculate the difference in mouse position
			Vector2 mouseDifference = m_newMouse - m_oldMouse;
			mouseDifference.Normalize();

			// Adjust the orientation based on mouse movement
			m_orientation.y -= mouseDifference.x * m_rotationRate;
			m_orientation.x -= mouseDifference.y * m_rotationRate;

			// Calculate the new camera position based on the orientation and distance from the center of rotation
			float distance = 1;
			m_position.x = centerOfRotation.x + distance * sin(m_orientation.x * XM_PI / 180) * cos(m_orientation.y * XM_PI / 180);
			m_position.y = centerOfRotation.y + distance * sin(m_orientation.y * XM_PI / 180);
			m_position.z = centerOfRotation.z + distance * cos(m_orientation.x * XM_PI / 180) * cos(m_orientation.y * XM_PI / 180);

			// Update the look at position
			m_lookAt = centerOfRotation;

			// Update the view matrix
			m_viewMatrix = Matrix::CreateLookAt(m_position, m_lookAt, Vector3::UnitY);
		}
		else
		{
			// If the button to activate focus camera mode is not held, switch back to normal camera mode
			HandleMouse(input_commands);
		}
	}

}

void CameraController::SpinCamera(const InputCommands& input_commands, float deltaTime)
{
	// Check if the button to activate camera spinning is held
	if (input_commands.spinCamera)
	{
		// Set the rotation speed (degrees per second)
		float spinSpeed = 1.0f;

		// Calculate the angular rotation for this frame based on the spin speed and delta time
		float spinAngle = spinSpeed * deltaTime;

		// Update the orientation to spin around the vertical axis
		m_orientation.y += spinAngle;

		// Update the view matrix without recalculating position and look-at
		m_viewMatrix = Matrix::CreateRotationY(m_orientation.y * XM_PI / 180) * Matrix::CreateTranslation(m_position);
	}
	else
	{
		// If the button to activate focus camera mode is not held, switch back to normal camera mode
		HandleMouse(input_commands);

	}
}
