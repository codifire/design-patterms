#pragma once
#include <algorithm>
#include <vector>
#include <functional>

namespace composite_pattern
{
	class CarSparePart
	{
	public:
		using ChildPartItr = std::vector<CarSparePart*>::iterator;
		ChildPartItr Invalid;

		virtual void draw() {}
		virtual ChildPartItr getChildIterator(unsigned int i = 0) { return Invalid; }
		virtual CarSparePart* getParent() { return parentPart; }
		virtual void setParent(CarSparePart* _ptr) { parentPart = _ptr; }
		virtual void addChild(CarSparePart*) {}
		virtual void removeChild(CarSparePart*) {}

	private:
		CarSparePart* parentPart = nullptr;
	};

	// leaves
	class Graphics : public CarSparePart
	{
	public:
		void draw() override {}
	};

	class Physics : public CarSparePart
	{
	public:
		void draw() override {}
	};

	// composite
	class BodyParts: public CarSparePart
	{
	public:
		BodyParts()
		{}

		void draw() override
		{
			for_each(childParts.begin(), childParts.end(), std::bind(&CarSparePart::draw, std::placeholders::_1));
		}

		virtual ChildPartItr getChildIterator(unsigned int i) override
		{
			return childParts.begin() + i;
		}

		virtual void addChild(CarSparePart* newPart)
		{
			childParts.push_back(newPart);
			newPart->setParent(this);
		}
		virtual void removeChild(CarSparePart* existingPart)
		{
			existingPart->setParent(nullptr);
			std::remove(childParts.begin(), childParts.end(), existingPart);
		}

	private:
		std::vector<CarSparePart*> childParts { };
	};

	class Tyres : public CarSparePart
	{
		/*
			implementation details
		*/
	};

	class GlassWindow : public CarSparePart
	{
		/*
			implementation details
		*/
	};

	class Engine : public CarSparePart
	{
		/*
			implementation details
		*/
	};

	void MakeCar()
	{
		Tyres frontLeft, frontRight, backLeft, backRight;
		frontLeft.addChild(new Graphics());
		frontRight.addChild(new Graphics());
		backLeft.addChild(new Graphics());
		backRight.addChild(new Graphics());
		
		frontLeft.addChild(new Physics());
		frontRight.addChild(new Physics());
		backLeft.addChild(new Physics());
		backRight.addChild(new Physics());

		GlassWindow front, sidefrontLeft, sidefrontRight, sideBackLeft, sideBackRight, back;
		front.addChild(new Graphics());
		sidefrontLeft.addChild(new Graphics());
		sidefrontRight.addChild(new Graphics());
		sideBackLeft.addChild(new Graphics());
		sideBackRight.addChild(new Graphics());
		back.addChild(new Graphics());

		BodyParts body;
		body.addChild(new Graphics());
		body.addChild(&frontLeft);
		body.addChild(&frontRight);
		body.addChild(&backLeft);
		body.addChild(&backRight);
		body.addChild(&front);
		body.addChild(&sidefrontLeft);
		body.addChild(&sidefrontRight);
		body.addChild(&sideBackLeft);
		body.addChild(&sideBackRight);
		body.addChild(&back);

		CarSparePart car;
		car.addChild(&body);
	}
}