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

#include "MengeCore/Scene/Business.h"
#include "MengeCore/Core.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/Agents/Events/EventSystem.h"

#include "MengeCore/BFSM/Transitions/TargetProb.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/GoalSet.h"
#include "MengeCore/BFSM/Transitions/Transition.h"
#include "MengeCore/BFSM/State.h"
#include "MengeCore/Agents/SimulatorInterface.h"


using namespace std;
using namespace Menge::BFSM;


namespace Menge {
	/////////////////////////////////////////////////////////////////////
	//					Implementation of Business
	/////////////////////////////////////////////////////////////////////

	char* Business::dataInit() {
		int shopPopulation[30] = { 0 };
		for (int i = 0; i < 18; i++) { //18��goalset
			for (int j = 0; j < 10; j++) {//ÿ��goalset��10�ҵ�
				//i%3=0 1 2,��Ӧeat shop play���ֵ�
				shopPopulation[(i % 3) * 10 + j] += SIMULATOR->getBFSM()->getGoal(i, j)->getPopulation();
			}
		}

		static char message[100] = {0};
		char tmp[3];
		for (int i = 0; i < 30; i++) {
			sprintf(tmp, "%d", shopPopulation[i]);
			strcat(message, tmp);
			sprintf(tmp, "%c", ' ');
			strcat(message, tmp);
		}
		
		std::cout << "data init over:" << std::endl;
		for (int i = 0; i < 30; i++) {
			std::cout << shopPopulation[i] << " ";
		}
		cout << endl;
		

		return message;
	}

	//��fsm����Ϣ�����ĸ�������
	void Business::fsmToMartix() {

		//1.��state�ĸ��ʴ�������state����
		size_t stateNum = ACTIVE_FSM->getNodeCount();//һ����24��state
		//��coupon��state��i<12; ��coupon��state��12<=i<24
		//ֻ��choose��state����ѡ����ʵģ�i==0��ӦEaterChoose��i==4��ӦShopperChoose��i==8��ӦPalyerChoose
		for (int i = 0; i < stateNum; i++) {
			if (i % 4 == 0) {
				vector<Transition* > vec = ACTIVE_FSM->getNode(i)->getTransitions();
				Transition* transition = vec[0];
				ProbTarget* probTarget = (ProbTarget*)transition->getTarget();
				map <State*, float> ::iterator itTarget;
				itTarget = probTarget->_targets.begin();
				while (itTarget != probTarget->_targets.end()) {
					//cout << itTarget->first->getName() <<": "<< itTarget->first->getID() <<": " << itTarget->second << endl;
					if (i < 12) ProbStatesNormal->SetPoint(itTarget->first->getID() / 4, itTarget->first->getID() % 4 - 1, itTarget->second);
					else ProbStatesCoupon->SetPoint((itTarget->first->getID() - 12) / 4, (itTarget->first->getID() - 12) % 4 - 1, itTarget->second);
					//�����ַ�ʽ�ҵ����ʣ����������
					itTarget++;
				}
			}
		}
		//cout << "ProbStatesNormal and ProbStatesCoupon" << endl;
		//ProbStatesNormal->Show();
		//ProbStatesCoupon->Show();


		//2.��goal�ĸ��ʴ���goal����
		//һ����18��goalset id=0~17 ,ÿ��goalset��10��goal id=0~9
		std::map< size_t, GoalSet* > goalSets = ACTIVE_FSM->getGoalSets();
		for (int i = 0; i < 18; i++) {
			GoalSet* goalSet = goalSets[i];
			for (int j = 0; j < 10; j++) {
				Goal* goal = goalSet->getGoalByID(j);
				//cout << i <<": "<< goal ->getID()<< ": " <<goal->getWeight() << endl;
				if (i < 9) ProbGoalsNormal->SetPoint(i / 3, i % 3, j, goal->getWeight());//typeAgent,typeState,shopID,value
				else ProbGoalsCoupon->SetPoint((i - 9) / 3, (i - 9) % 3, j, goal->getWeight());
			}
		}
		//cout << "ProbGoalsNormal and ProbGoalsCoupon" << endl;
		//ProbGoalsNormal->Show();
		ProbGoalsCoupon->Show();


	}

	//���ĸ������е���Ϣ����fsm
	void Business::MartixToFsm() {

		//ˢ��fsm����
		//��state�����еĸ��ʸ�ֵ��fsm
		size_t stateNum = ACTIVE_FSM->getNodeCount();//һ����24��state
		//��coupon��state��i<12; ��coupon��state��12<=i<24
		//ֻ��choose��state����ѡ����ʵģ�i==0��ӦEaterChoose��i==4��ӦShopperChoose��i==8��ӦPalyerChoose
		for (int i = 0; i < stateNum; i++) {
			if (i % 4 == 0) {
				vector<Transition* > vec = ACTIVE_FSM->getNode(i)->getTransitions();
				Transition* transition = vec[0];
				ProbTarget* probTarget = (ProbTarget*)transition->getTarget();
				map <State*, float> ::iterator itTarget;
				itTarget = probTarget->_targets.begin();//_targets��map��������state����ָ�������state
				while (itTarget != probTarget->_targets.end()) {
					//��һά���˿����ͣ��ڶ�ά��ѡ��state���ͣ�ֵ������
					//if (i < 12)  itTarget->second = ProbStatesNormal->Point(itTarget->first->getID() / 4, itTarget->first->getID() % 4 - 1);
					//else itTarget->second = ProbStatesCoupon->Point((itTarget->first->getID() - 12) / 4, (itTarget->first->getID() - 12) % 4 - 1);
					if (i >= 12)itTarget->second = Business::ProbStatesCoupon->Point((itTarget->first->getID() - 12) / 4, (itTarget->first->getID() - 12) % 4 - 1);
					itTarget++;
				}
			}
		}

		//��goal�����еĸ��ʸ�ֵ��fsm
		std::map< size_t, GoalSet* > goalSets = ACTIVE_FSM->getGoalSets();
		for (int i = 0; i < 18; i++) {
			GoalSet* goalSet = goalSets[i];
			for (int j = 0; j < 10; j++) {
				Goal* goal = goalSet->getGoalByID(j);
				//��һά���˿����ͣ��ڶ�ά����ǰstate���ͣ�����ά������id��ֵ��ѡ����̵ĸ���
				//if (i < 9) goal->setWeight(ProbGoalsNormal->Point(i / 3, i % 3, j));
				//else goal->setWeight(ProbGoalsCoupon->Point((i - 9) / 3, (i - 9) % 3, j));
				//cout << "test" << endl;
				//cout << Business::ProbGoalsCoupon->Point(0,0,0) << endl;
				
				if (i >= 9) {
					//cout << (i - 9) / 3 << " " << (i - 9) % 3 << endl;
					//cout << Business::ProbGoalsCoupon->Point((i - 9) / 3, (i - 9) % 3, j) << endl;
					goal->setWeight(Business::ProbGoalsCoupon->Point((i - 9) / 3, (i - 9) % 3, j));
				}
					
					
			}
		}
	}

	/////////////////////////////////////////////////////////////////////
	//					Implementation of BusinessReality
	/////////////////////////////////////////////////////////////////////
	char* BusinessReality::dataInit() {
		int shopPopulation[30] = { 0 };
		
		for (int i = 0; i < 30; i++) {//ÿ��goalset��10�ҵ�
			//i%3=0 1 2,��Ӧeat shop play���ֵ�
			shopPopulation[i] += SIMULATOR->getBFSM()->getGoal(0, i)->getPopulation();
		}
		

		static char message[100] = { 0 };
		stringstream ss;
		for (int i = 0; i < 30; i++) {
			ss << shopPopulation[i]<<" ";
		}
		strcpy(message, ss.str().c_str());

		std::cout << "data init over:" << std::endl;
		for (int i = 0; i < 30; i++) {
			std::cout << shopPopulation[i] << " ";
		}
		cout << endl;


		return message;

	}


	void BusinessReality::martixInit() {
		std::map<int, int> mapArea = {
			{0,1},{1,1},{2,1},{3,1},{10,1},{25,1},
			{4,2},{11,2},
			{20,3},{21,3},{22,3},{29,3},{19,3},
			{24,4},{16,4},{27,4},{17,4},{9,4},
			{8,5},{23,5},{18,5},{7,5},
			{12,6},{13,6},{14,6},{5,6},{26,6},{6,6},{28,6},
			{15,7}
		};
		int areaShopNum[7] = { 6,2,5,5,4,7,1 };
		int shopArea = 0;
		for (int i = 0; i < 30; i++) {//��ÿһ�ҵ����
			int shopArea = mapArea.find(i)->second; //�õ������
			int size = areaShopNum[shopArea - 1];//������ĵ�����
			for (int j = 0; j < 30; j++) { //��ÿһ�ҵ�
				int shopType = j / 10;//Ŀ���������
				int shopID = j % 10;//Ŀ�����id
				if (mapArea.find(j)->second == shopArea) {//�������ͬ����ĵ�
					BusinessReality::ProbGoals->SetPoint(0, i, shopType, shopID, static_cast<float>(20) / size);
					BusinessReality::ProbGoals->SetPoint(1, i, shopType, shopID, static_cast<float>(20) / size);
					BusinessReality::ProbGoals->SetPoint(2, i, shopType, shopID, static_cast<float>(20) / size);
				}
				else {
					BusinessReality::ProbGoals->SetPoint(0, i, shopType, shopID, 10 / (static_cast<float>(30) - size));
					BusinessReality::ProbGoals->SetPoint(1, i, shopType, shopID, 10 / (static_cast<float>(30) - size));
					BusinessReality::ProbGoals->SetPoint(2, i, shopType, shopID, 10 / (static_cast<float>(30) - size));
				}
			}
		}
	
	}
}




