#ifndef CAMERACOMPONENT_H_
#define CAMERACOMPONENT_H_

#include "Global.h"

class CameraComponent: public artemis::Component
{
public:
	CameraComponent(sf::RenderWindow* window, sf::View& view)
	{
		this->window = window;
		this->view = view;
		//view.setSize(viewSize);
		//assignViewToWindow();
	}

	sf::View* getView() { return &view; }
	void setView(sf::View val) { view = val; }

	sf::RenderWindow* getWindow() const { return window; }
	void setWindow(sf::RenderWindow* val) { window = val; }

	void assignViewToWindow(){ window->setView(view); }

private:
	sf::RenderWindow* window;
	sf::View view;
};

#endif