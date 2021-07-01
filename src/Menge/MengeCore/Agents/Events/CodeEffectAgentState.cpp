#include "MengeCore/Agents/Events/CodeEffectAgentState.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/Events/EventException.h"
#include "MengeCore/Agents/StateSelectors/StateSelectorDatabase.h"
#include "MengeCore/BFSM/State.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/Core.h"
#include <cstdlib>
#include <ctime>

namespace Menge {

	using Agents::BaseAgent;
	using Agents::StateSelectorDB;
	using BFSM::State;
	using std::string;

	/////////////////////////////////////////////////////////////////////
	//					Implementation of CodeEffectAgentState
	/////////////////////////////////////////////////////////////////////

	CodeEffectAgentState::CodeEffectAgentState() : AgentEventEffect() {}

	/////////////////////////////////////////////////////////////////////

	CodeEffectAgentState::~CodeEffectAgentState() {
		
	}

	/////////////////////////////////////////////////////////////////////

	void CodeEffectAgentState::agentEffect(BaseAgent * agent) {

		if (PROJECTNAME ==BUSINESS || PROJECTNAME == BUSINESSLEARNING) {
			//�Լ�����ķ�����agentָ����һ��state
			//���磺���Ż�ȯ��agent����ǰstate=0~11����ָ������һ��ת�Ƶ��ض�state���Ż�ȯstate����ڣ�
			State * currState = ACTIVE_FSM->getCurrentState(agent);
			size_t tmp = currState->getID();

			//�������ǵ��߼���һ���������agent�Ѿ����Ż�state��ʱ�����½����Ż�state��tmp<12��
			//����1��agent�յ��Ż�ȯ������ѡ��ǰ����state����һ�ҵ��̣�eat��Ȼ��eat�������̱仯��
			//��������agent�����Ż�ȯ�ĸ��ʣ���ʱ����Ϊ0.5
			//if( tmp<12 && 0.5 >(rand() % 100) * 0.01 ){
			if (tmp<12) {
				State* nextState = ACTIVE_FSM->getNode(tmp + 12);
				currState->leave(agent);
				nextState->enter(agent);
				ACTIVE_FSM->setCurrentState(agent, nextState->getID());
			}
		}
		
		else if (PROJECTNAME == BUSINESSREALITY) {
			//do nothing
		}
		
	}


	

	/////////////////////////////////////////////////////////////////////
	//					Implementation of EventEffectAgentStateFactory
	/////////////////////////////////////////////////////////////////////

	CodeEffectAgentStateFactory::CodeEffectAgentStateFactory() : EventEffectFactory() {
		
	}

	/////////////////////////////////////////////////////////////////////
	
	bool CodeEffectAgentStateFactory::setFromXML(EventEffect * effect, TiXmlElement * node,
		const std::string & behaveFldr) const {
		CodeEffectAgentState* aEffect = dynamic_cast<CodeEffectAgentState* >(effect);
		assert(aEffect != 0x0 && "Trying to set agent state event effect on an "
			"incompatible object");

		// This parses the target
		if (!EventEffectFactory::setFromXML(aEffect, node, behaveFldr)) return false;

		// Extract the StateSelector from the XML.
		auto selectorNode = node->FirstChildElement("StateSelector");
		assert(selectorNode != nullptr && "The 'set_agent_state' event effect requries a "
			"StateSelector child.");
		
		
		return true;
	}

}	// namespace Menge
