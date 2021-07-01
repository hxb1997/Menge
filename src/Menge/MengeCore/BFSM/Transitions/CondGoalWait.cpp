/*

License

Menge
Copyright ?and trademark ?2012-14 University of North Carolina at Chapel Hill. 
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation 
for educational, research, and non-profit purposes, without fee, and without a 
written agreement is hereby granted, provided that the above copyright notice, 
this paragraph, and the following four paragraphs appear in all copies.

This software program and documentation are copyrighted by the University of North 
Carolina at Chapel Hill. The software program and documentation are supplied "as is," 
without any accompanying services from the University of North Carolina at Chapel 
Hill or the authors. The University of North Carolina at Chapel Hill and the 
authors do not warrant that the operation of the program will be uninterrupted 
or error-free. The end-user understands that the program was developed for research 
purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS 
BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS 
DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE 
AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY 
DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY STATUTORY WARRANTY 
OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND 
THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS HAVE NO OBLIGATIONS 
TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Any questions or comments should be sent to the authors {menge,geom}@cs.unc.edu

*/

#include "MengeCore/BFSM/Transitions/CondGoalWait.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/Runtime/Logger.h"

#include "MengeCore/Core.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/BFSM/State.h"
#include "MengeCore/Agents/SimulatorInterface.h"

namespace Menge {

	namespace BFSM {

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of GoalWaitCondition
		///////////////////////////////////////////////////////////////////////////

		GoalWaitCondition::GoalWaitCondition(): Condition(),_distSq(0.f) {
		}

		///////////////////////////////////////////////////////////////////////////

		GoalWaitCondition::GoalWaitCondition( const GoalWaitCondition & cond ) : Condition(cond) {
			_distSq = cond._distSq;
			/*for (int i = 0; i < SIMULATOR->getNumAgents(); i++) {
				_lock.lockWrite();
				_triggerTimes.insert(std::map< size_t, float >::value_type(i, 0));
				_reachedAgents.insert(std::map< size_t, float >::value_type(i, false));
				_lock.releaseWrite();
			}*/

		}
		///////////////////////////////////////////////////////////////////////////
		//agent��test conditionMet,Ȼ��onenter��Ȼ���޸�currnode
		void GoalWaitCondition::onEnter(Agents::BaseAgent* agent) {
			
			if (_triggerTimes.find(agent->_id) != _triggerTimes.end()) { //�������
				_lock.lockWrite();
				_triggerTimes[agent->_id] = 0;//��ֵ��ʱ��Ҫ��������ʼ��Ϊ0
				_reachedAgents[agent->_id] = false;//��ʼ��Ϊfalse
				_lock.releaseWrite();
			}
			else { //��������ڣ�
				_triggerTimes.insert(std::map< size_t, float >::value_type(agent->_id, 0));
				_reachedAgents.insert(std::map< size_t, float >::value_type(agent->_id, false));
				//insert��kv���Ѵ��ڵ�����£����Ե�ǰinsert���
			}
			
		}
		///////////////////////////////////////////////////////////////////////////

		bool GoalWaitCondition::conditionMet( Agents::BaseAgent * agent, const Goal * goal ) {
			float distSq = goal->squaredDistance( agent->_pos );
			bool reached = (distSq <= _distSq);

			if (reached) {
				_lock.lockWrite();
				_reachedAgents[agent->_id] = true;
				_lock.releaseWrite();
			}

			if (_reachedAgents[agent->_id] && _triggerTimes.find(agent->_id)->second==0) {//�ѵ���Ҽ�ʱδ��ʼ
				if (PROJECTNAME == BUSINESSREALITY) {

					float timeReached = 0.0;
					int goalType = goal->getID() / 10; //0 1 2 ��Ӧeat shop play
					switch (goalType) {
						case 0:timeReached = Menge::SIM_TIME + 30; break;
						case 1:timeReached = Menge::SIM_TIME + 20; break;
						case 2:timeReached = Menge::SIM_TIME + 50; break;
					}
					_lock.lockWrite();
					_triggerTimes[agent->_id] = timeReached;//��ֵ��ʱ��Ҫ����
					_lock.releaseWrite();

				}	
			}

			if (_reachedAgents[agent->_id] && Menge::SIM_TIME > _triggerTimes.find(agent->_id)->second) {
				return true;
			}

			

			return false;
		}

		///////////////////////////////////////////////////////////////////////////

		Condition * GoalWaitCondition::copy() {
			return new GoalWaitCondition( *this );
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of GoalWaitCondFactory
		/////////////////////////////////////////////////////////////////////

		GoalWaitCondFactory::GoalWaitCondFactory() : ConditionFactory() {
			_distanceID = _attrSet.addFloatAttribute( "distance", false /*required*/,
													  0.f /*default*/ );
		}

		///////////////////////////////////////////////////////////////////////////

		bool GoalWaitCondFactory::setFromXML( Condition * condition, TiXmlElement * node,
										  const std::string & behaveFldr ) const {
			GoalWaitCondition* gCond = dynamic_cast<GoalWaitCondition* >( condition );
			assert( gCond != 0x0 &&
					"Trying to set the properties of a goal condition on an incompatible object" );

			if ( !ConditionFactory::setFromXML( gCond, node, behaveFldr ) ) return false;

			gCond->setMinDistance( _attrSet.getFloat( _distanceID ) );
			return true;
		}

	}	// namespace BFSM 
}	// namespace Menge
