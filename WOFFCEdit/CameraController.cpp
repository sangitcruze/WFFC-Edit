#include "CameraController.h"
#include "InputCommands.h"
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