// GameEngine.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

enum ObjectComponentType {
	DESCRIPTION,
	NUM_COMPONENTS
};

class ObjectComponent {

	ObjectComponentType type;

	public:
		ObjectComponent(ObjectComponentType type)
			: type(type)
		{};

		ObjectComponentType getType() {
			return type;
		};
};

class DescriptionObjectComponent : public ObjectComponent {

	std::string description;

	public:
		DescriptionObjectComponent()
			: ObjectComponent(ObjectComponentType::DESCRIPTION)
		{};
		DescriptionObjectComponent(std::string description)
			: ObjectComponent(ObjectComponentType::DESCRIPTION),
			description(description)
		{};

		std::string getDescription() {
			return description;
		};

		std::string setDescription(std::string description) {
			this->description = description;
		};
		


};

class GameObject {

	ObjectComponent* component[NUM_COMPONENTS];
	std::vector<GameObject*> children;
	GameObject* parent;

	public:
		GameObject()
			: parent(0) {};

		void addComponent(ObjectComponent& component) {
			this->component[component.getType()] = &component;
		};

		template<typename T> bool hasComponent() {
			return (bool)component[T().getType()];
		};

		template<typename T> T& getComponent() {
			return *(T*)(component[T().getType()]);
		};

		void setParent(GameObject& gameObject) {
			if (std::find(gameObject.children.begin(), gameObject.children.end(), this) == gameObject.children.end()) {
				gameObject.children.push_back(this);
				parent = &gameObject;
			}
		};

		void removeParent() {
			if (parent) {
				auto iter(std::find(parent->children.begin(), parent->children.end(), this));
				parent->children.erase(iter);
				parent = 0;
			}
		};

		std::vector<GameObject*>& getChildren() {
			return children;
		};

};

void renderObject(GameObject& gameObject) {
	
	if (gameObject.hasComponent<DescriptionObjectComponent>()) {
		DescriptionObjectComponent& descriptionComponent(gameObject.getComponent<DescriptionObjectComponent>());
		printf("Description: %s\n", descriptionComponent.getDescription().c_str());
	}

	for (GameObject* child : gameObject.getChildren()) {
		renderObject(*child);
	}

}


int main()
{
	GameObject superGameObject;
	GameObject helloObject;
	DescriptionObjectComponent superDescription("superDescription...\n");
	DescriptionObjectComponent descriptionComponent("Hello, World!\n");
	
	helloObject.addComponent(descriptionComponent);
	helloObject.setParent(superGameObject);
	superGameObject.addComponent(superDescription);

	renderObject(superGameObject);

    return 0;
}

