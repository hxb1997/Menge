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

#include "MengeCore/Agents/Events/TimeEvtTrigger.h"

#include "MengeCore/Agents/Events/EventSystem.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/BFSM/State.h"

#include "MengeCore/BFSM/Transitions/TargetProb.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/GoalSet.h"
#include "MengeCore/MatrixMy.h"
#include "MengeCore/BFSM/Transitions/Transition.h"
#include "MengeCore/Agents/SimulatorInterface.h"
#include "MengeCore/Agents/SimulatorState.h"
#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Scene/Business.h"
#include "MengeCore/Socket.h"

#include "MengeCore/Core.h"
#include "MengeVis/Viewer/GLViewer.h"

#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

#include <sstream>

//extern MengeVis::Viewer::GLViewer* SimViewer;

namespace Menge {

	using namespace std;
	using namespace Menge::BFSM;

	
	//extern const MatrixDim2* Business::ProbStatesNormal;
	//extern const MatrixDim2* Business::ProbStatesCoupon;
	//extern const MatrixDim3* Business::ProbGoalsNormal;
	//extern const MatrixDim3* Business::ProbGoalsCoupon;
	extern char* Business::dataInit();
	extern int* Socket::socketGetCouponBusiness(char* message);
	extern int* Socket::socketGetCouponBusinessReality(char* message);

	float TimeReachedTrigger::_timeSimulate = 10;
	float TimeReachedTrigger::_lastTimestamp = 0;

	/////////////////////////////////////////////////////////////////////
	//					Implementation of StateEvtTrigger
	/////////////////////////////////////////////////////////////////////

	TimeEvtTrigger::TimeEvtTrigger() : EventTrigger() {
	}

	/////////////////////////////////////////////////////////////////////

	void TimeEvtTrigger::finalize() {
		
	}
	
	/////////////////////////////////////////////////////////////////////
	//					Implementation of StateEvtTriggerFactory
	/////////////////////////////////////////////////////////////////////

	TimeEvtTriggerFactory::TimeEvtTriggerFactory() : EventTriggerFactory() {
		_timeSimulateIndex = _attrSet.addFloatAttribute( "timeSimulate", true, 0.0 );//���÷���ʱ��
	}


	/////////////////////////////////////////////////////////////////////

	bool TimeEvtTriggerFactory::setFromXML( EventTrigger * trigger, TiXmlElement * node,
											 const std::string & specFldr ) const {
		TimeEvtTrigger* tTrigger = dynamic_cast<TimeEvtTrigger* >( trigger );
		assert(tTrigger != 0x0 &&
				"Trying to set attributes of a state event trigger on an incompatible object" );

		if ( ! EventTriggerFactory::setFromXML(tTrigger, node, specFldr ) ) return false;

		TimeReachedTrigger::_timeSimulate = _attrSet.getFloat(_timeSimulateIndex);

		return true;
	}

	/////////////////////////////////////////////////////////////////////
	//					Implementation of StatePopIncreaseTrigger
	/////////////////////////////////////////////////////////////////////

	// _lastPop is initialized to a ridiculously high number in order to keep
	//	the event from triggering upon initialization.  The first call to
	//	testCondition will bring it back down.
	TimeReachedTrigger::TimeReachedTrigger() : TimeEvtTrigger() {
	}

	/////////////////////////////////////////////////////////////////////

	void TimeReachedTrigger::finalize() {
		_lastTimestamp = Menge::SIM_TIME;//��ɺ��ȡ��ǰ�ķ���ʱ��
	}

	/////////////////////////////////////////////////////////////////////

	bool TimeReachedTrigger::testCondition(Agents::SimulatorInterface * sim, BFSM::FSM * fsm) { //��ⴥ������

		if ((Menge::SIM_TIME - _lastTimestamp) > _timeSimulate) {
			cout << "trigger condition met at :"<<Menge::SIM_TIME << endl;
			_lastTimestamp = Menge::SIM_TIME;
			MengeVis::SimViewer->_pause = !MengeVis::SimViewer->_pause;
			cout << "pause" << endl;


			if (PROJECTNAME ==BUSINESS || PROJECTNAME == BUSINESSLEARNING) {
				//������Ҫ0.��ʼ������ 1.socket�������� 2.socket����python���� 3.���ݶ������ú����ı���ʾ��� 4.ˢ��fsm����
				//0.��ʼ����Ҫ���͵����ݣ�����������
				//char* message = Business::dataInit();
				char* message = Business::dataInit();

				//1.socket�������� 2.��������
				int* res = Socket::socketGetCouponBusiness(message);
				memset(message, 0, sizeof(message) / sizeof(char));


				//3.�����յ��Ķ��������ú����ı���ʾ���
				
				//[[0,0,1],[0,0,5],3] ��������̣���3����Ⱥ��5�ŵ��̣�����3
				int typeAgent = 0;
				int shopID = 0;
				int strength = 0;
				int typeShop = 0;
				for (int i = 0; i < 7; i++) {
					cout << i << ":" << res[i] << endl;
					if (i < 3 && res[i]!=0) {
						typeShop = i;
					}
					else if (i < 6 && res[i] != 0) {
						typeAgent = i - 3;
						shopID = typeShop * 10 + res[i] - 1;
					}
					else {
						strength = res[i];
					}
				}
				cout << "typeAgent" << typeAgent << "shopID"<< shopID<< "strength"<< strength << endl;
				//������ʾۼ�
				Business::ProbGoalsCoupon->Aggregate(typeAgent, shopID, strength);
				//cout << "ProbStatesNormal and ProbStatesCoupon" << endl;
				//Business::ProbStatesNormal->Show();
				//Business::ProbStatesCoupon->Show();
				//cout << "ProbGoalsNormal and ProbGoalsCoupon" << endl;
				//Business::ProbGoalsNormal->Show();
				Business::ProbGoalsCoupon->Show();
				//���ݽ��յ�����Ϣ�޸ľ������
				Business::MartixToFsm();

				//���¿�ʼ
				MengeVis::SimViewer->_pause = !MengeVis::SimViewer->_pause;
				

			}
			else if (PROJECTNAME ==BUSINESSREALITY) {
				//0.��ʼ����Ҫ���͵�����  1.��ʼ��socket 2.�������ݸ�python 3.����action 4.��actionת��Ϊ���ʾ���
				//0 ��ʼ�����͵�����
				char* message = BusinessReality::dataInit();
				cout << "message: " << message << endl;

				//1 2 3 ��ʼ��socket ���͡���������
				int* res = Socket::socketGetCouponBusinessReality(message);
				memset(message, 0, sizeof(message) / sizeof(char));
				cout << "typeAgent: " << res[0] << "  shopID:  " << res[1] << "  strength: " << res[2] << endl;

				//4 ��actionת��Ϊ���ʾ���
				BusinessReality::ProbGoals->Aggregate(res[0], res[1], res[2]);
				//BusinessReality::ProbGoals->Show();
				//���ݽ��յ�����Ϣ�޸ľ������

				MengeVis::SimViewer->_pause = !MengeVis::SimViewer->_pause;

			}
			else{//ÿ��һ��ʱ��ȷ��һ��Ŀ�ĵ�
                const size_t AGT_COUNT = sim->getNumAgents();
                for ( size_t a = 0; a < AGT_COUNT; ++a ) {
                    Agents::BaseAgent * agt = sim->getAgent( a );
                    State * pState = fsm->getCurrentState(agt);
                    pState->leave(agt);
                    pState->enter(agt);
                }
			}
			return true;
		}

		return false;
	}

}	// namespace Menge 
