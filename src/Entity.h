#pragma once
#include "defines.h"

class Entity
{
public:
	Entity(): mName("") {}
	Entity(const std::string& name): mName(name) {}
	virtual ~Entity() {}

	const std::string & GetName() const { return mName; }
	void SetName(const std::string& name) { mName = name; }
	bool CompareName(const std::string& name) { return mName == name; }

	virtual void Destroy() = 0;
	
protected:
	std::string mName;

private:
	Entity(const Entity&);
	Entity(Entity&&);
	Entity & operator=(const Entity&);
	Entity & operator=(Entity&&);

};