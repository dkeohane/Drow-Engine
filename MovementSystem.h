#include "Artemis\Artemis.h"
#include "VelocityComponent.h"
#include "PositionComponent.h"
#include "PlayerInputComponent.h"
#include "MovementComponent.h"

class MovementSystem : public artemis::EntityProcessingSystem 
{
public:
	MovementSystem()
	{
		addComponentType<MovementComponent>();
		addComponentType<VelocityComponent>();
		addComponentType<PositionComponent>();
		addComponentType<PlayerInputComponent>();

		currentMovementState = MovementComponent::IDLE;
	}

	virtual void initialize() 
	{
		movementMapper.init(*world);
		velocityMapper.init(*world);
		positionMapper.init(*world);
		playerInputMapper.init(*world);
	}

	/*
	bool InputHandlingSystem::isControllerConnected(int controllerID)
	{
		return sf::Joystick::isConnected(controllerID);
	}

	void InputHandlingSystem::connectController()
	{
		if (event.type == sf::Event::JoystickConnected)
		{
			this->controllerID = event.joystickConnect.joystickId;
		}
	}
	*/

	virtual void processEntity(artemis::Entity &e) 
	{
		joystick(e);
	}

	void joystick(artemis::Entity &e)
	{
		axisThreshold = playerInputMapper.get(e)->getAxisThreshold();
		gamepadID = playerInputMapper.get(e)->getGamepadID();

		xVal = sf::Joystick::getAxisPosition(playerInputMapper.get(e)->getGamepadID(), sf::Joystick::X);
		yVal = sf::Joystick::getAxisPosition(playerInputMapper.get(e)->getGamepadID(), sf::Joystick::Y);

		// Max 'Up value' for Joystick is -100 
		if(sf::Joystick::getAxisPosition(gamepadID, sf::Joystick::Y) < -this->axisThreshold)
		{
			if(currentMovementState != MovementComponent::UP) // Used to stop spamming the movement component with updates
			{
				currentMovementState = MovementComponent::UP;
				movementMapper.get(e)->setCurrentMovementState(MovementComponent::UP);
			}
			xVal = 0;
		}
		// Max 'Down value' for Joystick is 100 
		else if(sf::Joystick::getAxisPosition(gamepadID, sf::Joystick::Y) > this->axisThreshold)
		{
			if(currentMovementState != MovementComponent::DOWN)
			{
				currentMovementState = MovementComponent::DOWN;
				movementMapper.get(e)->setCurrentMovementState(MovementComponent::DOWN);
			}
			xVal = 0;
		}
		// Max 'Left value' for Joystick is -100
		else if(sf::Joystick::getAxisPosition(gamepadID, sf::Joystick::X) < -this->axisThreshold)
		{
			if(currentMovementState != MovementComponent::LEFT)
			{
				currentMovementState = MovementComponent::LEFT;
				movementMapper.get(e)->setCurrentMovementState(MovementComponent::LEFT);
			}
			
			yVal = 0;
		}
		// Max 'Right value' for Joystick is 100
		else if(sf::Joystick::getAxisPosition(gamepadID, sf::Joystick::X) > this->axisThreshold)
		{
			if(currentMovementState != MovementComponent::RIGHT)
			{
				currentMovementState = MovementComponent::RIGHT;
				movementMapper.get(e)->setCurrentMovementState(MovementComponent::RIGHT);
			}
			yVal = 0;
		} 
		else // IDLE
		{
			if(currentMovementState != MovementComponent::IDLE)
			{
				currentMovementState = MovementComponent::IDLE;
				movementMapper.get(e)->setCurrentMovementState(MovementComponent::IDLE);
			}
		}

		// Move if not IDLE
		if(movementMapper.get(e)->getCurrentMovementState() != MovementComponent::IDLE)
		{
			positionMapper.get(e)->setPosition(
				positionMapper.get(e)->getPosition() + sf::Vector2f(velocityMapper.get(e)->getVelocityX() * xVal / 100,
				velocityMapper.get(e)->getVelocityY() * yVal / 100));
		}
	}

	void dPad(artemis::Entity &e)
	{
		axisThreshold = playerInputMapper.get(e)->getAxisThreshold();
		gamepadID = playerInputMapper.get(e)->getGamepadID();

		xVal = sf::Joystick::getAxisPosition(playerInputMapper.get(e)->getGamepadID(), sf::Joystick::PovY);
		yVal = -sf::Joystick::getAxisPosition(playerInputMapper.get(e)->getGamepadID(), sf::Joystick::PovX);

		// Max 'Up value' for Dpad is 100
		if(sf::Joystick::getAxisPosition(gamepadID, sf::Joystick::PovX) > this->axisThreshold)
		{
			movementMapper.get(e)->setCurrentMovementState(MovementComponent::UP);
			xVal = 0;
		}
		// Max 'Down value' for Dpad is 100
		else if(sf::Joystick::getAxisPosition(gamepadID, sf::Joystick::PovX) < -this->axisThreshold)
		{
			movementMapper.get(e)->setCurrentMovementState(MovementComponent::DOWN);
			xVal = 0;
		}
		// Max 'Left value' for Dpad is -100
		else if(sf::Joystick::getAxisPosition(gamepadID, sf::Joystick::PovY) < -this->axisThreshold)
		{
			movementMapper.get(e)->setCurrentMovementState(MovementComponent::LEFT);
			yVal = 0;
		}
		// Max 'Right value' for Dpad is 100
		else if(sf::Joystick::getAxisPosition(gamepadID, sf::Joystick::PovY) > this->axisThreshold)
		{
			movementMapper.get(e)->setCurrentMovementState(MovementComponent::RIGHT);
			yVal = 0;
		}
		else
		{
			movementMapper.get(e)->setCurrentMovementState(MovementComponent::IDLE);
			xVal = 0;
			yVal = 0;
		}

		// Move if not IDLE
		if(movementMapper.get(e)->getCurrentMovementState() != MovementComponent::IDLE)
		{
			positionMapper.get(e)->setPosition(
				positionMapper.get(e)->getPosition() + sf::Vector2f(velocityMapper.get(e)->getVelocityX() * xVal / 100,
				velocityMapper.get(e)->getVelocityY() * yVal / 100));
		}
	}
private:
	float yVal;
	float xVal;
	float axisThreshold;
	int gamepadID;

	MovementComponent::MovementState currentMovementState;

	artemis::ComponentMapper<MovementComponent> movementMapper;
	artemis::ComponentMapper<VelocityComponent> velocityMapper;
	artemis::ComponentMapper<PositionComponent> positionMapper;
	artemis::ComponentMapper<PlayerInputComponent> playerInputMapper;
};