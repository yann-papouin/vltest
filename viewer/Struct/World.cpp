//! \file World.cpp implementation of the World class.

#include "World.h"
#include "instance.h"
#include "reference.h"

// Default constructor
World::World()
: m_pWorldHandle(new WorldHandle())
, m_pRoot(new Occurence())
{
	m_pRoot->setWorldHandle(m_pWorldHandle);
	//qDebug() << "World::World() : " << (*m_pNumberOfWorld) << " " << this;
}

// Copy constructor
World::World(const World& world)
: m_pWorldHandle(world.m_pWorldHandle)
, m_pRoot(world.m_pRoot)
{
	//qDebug() << "World::World() : " << (*m_pNumberOfWorld) << " " << this;
	// Increment the number of world
	m_pWorldHandle->increment();
}

World::~World()
{

	// Decrement the number of world
	m_pWorldHandle->decrement();
	//qDebug() << "World::World() : " << (*m_pNumberOfWorld) << " " << this;
	if (m_pWorldHandle->isOrphan())
	{
		// this is the last World, delete the root product and collection
		//m_pWorldHandle->collection()->clear(); // Clear collection first (performance)
		delete m_pRoot;
		delete m_pWorldHandle;
	}
}

// Merge this world with another world
void World::mergeWithAnotherWorld(World& anotherWorld)
{
//	qDebug() << "World::mergeWithAnotherWorld";
	Occurence* pAnotherRoot= anotherWorld.rootOccurence();
	if (pAnotherRoot->childCount() > 0)
	{
		std::vector<Occurence*> childs= pAnotherRoot->children();
		const int size= childs.size();
		for (int i= 0; i < size; ++i)
		{
			m_pRoot->addChild(childs.at(i)->clone(m_pWorldHandle, false));
		}
		m_pRoot->updateChildrenAbsoluteMatrix();
	}
	else
	{
		m_pRoot->addChild(anotherWorld.rootOccurence()->clone(m_pWorldHandle, false));
	}
}

// Assignment operator
World& World::operator=(const World& world)
{
	if (this != &world)
	{
		// Decrement the number of world
		m_pWorldHandle->decrement();
		if (m_pWorldHandle->isOrphan())
		{
			// this is the last World, delete the root product and collection
			//m_pWorldHandle->collection()->clear(); // Clear collection first (performance)
			delete m_pRoot;
			delete m_pWorldHandle;
		}
		m_pRoot= world.m_pRoot;
		m_pWorldHandle= world.m_pWorldHandle;
		m_pWorldHandle->increment();
	}
	return *this;
}
