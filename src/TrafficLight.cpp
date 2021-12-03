#include "TrafficLight.h"
#include "Logging.h"

TrafficLight::TrafficLight() {
  timeAccumulator = 0;
  currentColor = GREEN;
}

void TrafficLight::reset()
{
	Logging::debug("[TrafficLight::reset] Resetting raffic light");
	timeAccumulator = 0;
	currentColor = GREEN;
}

void TrafficLight::update(float fElapsedTime) {
  // TODO
	timeAccumulator += fElapsedTime;
	if (currentColor == GREEN && timeAccumulator >= 7.0f)
	{
		Logging::debug("[TrafficLight::update] Traffic light turns yellow");
		currentColor = YELLOW;
		timeAccumulator = 0;
	}
	else if (currentColor == YELLOW && timeAccumulator >= 3.0f)
	{
		Logging::debug("[TrafficLight::update] Traffic light turns red");
		currentColor = RED;
		timeAccumulator = 0;
	}
	else if (currentColor == RED && timeAccumulator >= 3.0f)
	{
		Logging::debug("[TrafficLight::update] Traffic light turns green");
		currentColor = GREEN;
		timeAccumulator = 0;
	}
}

void TrafficLight::draw(olc::PixelGameEngine* pge) {
  // TODO
	if (currentColor == GREEN) 
	{
		// draw green light
		sprite = new olc::Sprite("assets/graphics/light_green.jpg");
		if (sprite)
			pge->DrawSprite(pos, sprite);
	}
	else if (currentColor == YELLOW) 
	{
		// draw yellow light
		sprite = new olc::Sprite("assets/graphics/light_yellow.jpg");
		if (sprite)
			pge->DrawSprite(pos, sprite);
	}
	else 
	{
		// draw red light
		sprite = new olc::Sprite("assets/graphics/light_red.jpg");
		if (sprite)
			pge->DrawSprite(pos, sprite);
	}
}

TrafficLightColor TrafficLight::getColor() const {
  return currentColor;
}
