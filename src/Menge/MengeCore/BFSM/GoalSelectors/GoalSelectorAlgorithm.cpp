

#include "MengeCore/BFSM/GoalSelectors/GoalSelectorAlgorithm.h"
#include "MengeCore/resources/Graph.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/GoalSet.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/Agents/SimulatorInterface.h"

#include <cassert>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of AlgorithmGoalSelector
		/////////////////////////////////////////////////////////////////////

		using namespace std;

		bool AlgorithmGoalSelector::_flag;
		map <size_t, Goal*> AlgorithmGoalSelector::_bestGoals;

		AlgorithmGoalSelector::AlgorithmGoalSelector() {
			AlgorithmGoalSelector::_flag = false;
			AlgorithmGoalSelector::_bestGoals.clear();
		}

		Goal* AlgorithmGoalSelector::getGoal(const Agents::BaseAgent* agent) const {
			if (_flag == false) {
				//�ж�ϵͳ����
				assert(agent != 0x0 && "AlgorithmGoalSelector requires a valid base agent!");
				const size_t GOAL_COUNT = _goalSet->size();
				if (GOAL_COUNT == 0) {
					logger << Logger::ERR_MSG;
					logger << "AlgorithmGoalSelector was unable to provide a goal for agent ";
					logger << agent->_id << ".  There were no available goals in the goal set.";
					return 0x0;
				}

				const size_t NUM_AGENT = Menge::SIMULATOR->getNumAgents();
				const size_t NUM_GOAL = _goalSet->size();
				size_t agent_id;
				float_t speed;
				float_t distance;
				float_t time_predict;
				size_t i = 0;
				size_t j = 0;
				//�洢ÿ��goal������agent-time map����СΪNUM_GOAL key=goalID value=map<agentID,time>
				map<size_t, map<size_t, float_t>> map_goal;
				//�洢ÿ��agent��i���ڵ�time����СΪNUM_AGENT key=agentID value=time
				map<size_t, float_t> map_agent;

				for (i = 0; i < NUM_GOAL; i++) { //����ÿ������
					j = 0;
					for (j = 0; j < NUM_AGENT; j++) { //����ÿ��agent
						Agents::BaseAgent* agentTest = Menge::SIMULATOR->getAgent(j);//��ǰagent
						speed = agentTest->_prefSpeed;//��ʼ�ٶ�
						//��graph.h�������˾�̬��Ա������������A*�㷨
						distance = Graph::graphLoad->getPathLenth(agentTest, _goalSet->getIthGoal(i));//�õ�·������
						time_predict = distance / speed + 50;//�����ǹ�ʽ��ʵ�֣��������������ʱ��֪����ô����
						//��agent��i���ڵ�time��������˳��
						map_agent.insert(pair < size_t, float_t >(j, time_predict));
					}
					map_goal.insert(pair < size_t, map<size_t, float_t> >(i, map_agent));
					map_agent.clear();
				}

 				Goal* bestGoal = _goalSet->getIthGoal(0);

				map <size_t, map<size_t, float_t>> ::iterator itGoal;
				map < size_t, float_t > ::iterator itAgent;
				map_agent = map_goal.begin()->second; //ȡ����һ��goal��map
				itAgent = map_agent.begin();

				//�ҵ�һ��map_agent�������Ԫ�ؽ����������
				while (itAgent != map_agent.end()) {  //����ÿ��agent
					agent_id = itAgent->first;
					time_predict = itAgent->second;
					itGoal = map_goal.begin();
					itGoal++;
					bestGoal = _goalSet->getIthGoal(0);
					while (itGoal != map_goal.end()) {  //����ÿ��goal
						if (itGoal->second.find(agent_id)->second <= time_predict) {//�ҵ�ʱ����ٵĳ�����
							time_predict = itGoal->second.find(agent_id)->second;
							bestGoal = _goalSet->getIthGoal(itGoal->first);
						}
						itGoal++;
					}
					_bestGoals.insert(pair <size_t, Goal*>(agent_id, bestGoal));
					itAgent++;
				}
				_flag=true;
			}
			
			return _bestGoals.find(agent->_id)->second;
		}


	}	// namespace BFSM
 
}	// namespace Menge
