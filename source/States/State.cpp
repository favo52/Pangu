#include "States/State.h"
#include "States/StateStack.h"

namespace Pangu
{
	State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, bool& toggle) :
		window{ &window },
		textures{ &textures },
		fonts{ &fonts },
		toggle{ &toggle }
	{
	}

	State::State(StateStack& stack, Context context) :
		m_Stack{ &stack },
		m_Context{ context }
	{
	}

	State::~State()
	{
	}

	void State::RequestStackPush(States stateID)
	{
		m_Stack->PushState(stateID);
	}

	void State::RequestStackPop()
	{
		m_Stack->PopState();
	}

	void State::RequestStateClear()
	{
		m_Stack->ClearStates();
	}

	State::Context State::GetContext() const
	{
		return m_Context;
	}
}
