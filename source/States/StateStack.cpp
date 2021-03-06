#include "States/StateStack.h"

#include <cassert>

namespace Pangu
{
	StateStack::StateStack(State::Context context) :
		m_Stack{},
		m_PendingList{},
		m_Context{ context },
		m_Factories{}
	{
	}

	void StateStack::Update(sf::Time dt)
	{
		// Iterate from top to bottom, stop as soon as update() returns false
		for (auto itr = m_Stack.rbegin(); itr != m_Stack.rend(); ++itr)
		{
			if (!(*itr)->Update(dt))
				break;
		}

		ApplyPendingChanges();
	}

	void StateStack::Draw()
	{
		// Draw all active states from bottom to top
		for (State::StatePtr& state : m_Stack)
			state->Draw();
	}

	void StateStack::HandleEvent(const sf::Event& event)
	{
		// Iterate from top to bottom, stop as soon as handleEvent() returns false
		for (auto itr = m_Stack.rbegin(); itr != m_Stack.rend(); ++itr)
		{
			if (!(*itr)->HandleEvent(event))
				break;
		}

		ApplyPendingChanges();
	}

	void StateStack::PushState(States stateID)
	{
		m_PendingList.push_back(PendingChange(Action::Push, stateID));
	}

	void StateStack::PopState()
	{
		m_PendingList.push_back(PendingChange(Action::Pop));
	}

	void StateStack::ClearStates()
	{
		m_PendingList.push_back(PendingChange(Action::Clear));
	}

	bool StateStack::IsEmpty() const
	{
		return m_Stack.empty();
	}

	State::StatePtr StateStack::CreateState(States stateID)
	{
		auto found = m_Factories.find(stateID);
		assert(found != m_Factories.end());

		return found->second();
	}

	void StateStack::ApplyPendingChanges()
	{
		for (PendingChange change : m_PendingList)
		{
			switch (change.action)
			{
			case Action::Push:
				m_Stack.push_back(CreateState(change.stateID));
				break;

			case Action::Pop:
				m_Stack.pop_back();
				break;

			case Action::Clear:
				m_Stack.clear();
				break;
			}
		}

		m_PendingList.clear();
	}

	StateStack::PendingChange::PendingChange(Action action, States stateID) :
		action{ action },
		stateID{ stateID }
	{
	}
}
