

#include "MengeCore/Socket.h"
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

	SOCKET Socket::socketInit(char* ip, int host) {
		WSADATA wsadata;
		WSAStartup(MAKEWORD(2, 1), &wsadata);//��һ��������socket�汾���ڶ���������socketͨ������������socket�汾��Ϣ

		//c++��Ϊ�ͻ��ˣ������׽���/��/����
		SOCKET  clientPython;
		//��ʼ���׽�������
		struct sockaddr_in adr_pythons;
		adr_pythons.sin_family = AF_INET;
		adr_pythons.sin_addr.s_addr = inet_addr(ip);
		adr_pythons.sin_port = htons(host);//���ӷ����12347�˿�

		std::cout << "socket start" << std::endl;
		clientPython = socket(AF_INET, SOCK_STREAM, 0);
		connect(clientPython, (struct sockaddr*)&adr_pythons, sizeof(adr_pythons));

		return clientPython;
	}

	
	int* Socket::socketGetCouponBusiness(char* message) {
		//1.socket��ʼ������������
		char* ip = "127.0.0.1";
		int host = 12347;
		SOCKET clientPython = socketInit(ip,host);

		std::cout << "send msg to python: " << message << std::endl;
		send(clientPython, message, strlen(message) * sizeof(char), 0);//������Ϣ��python��server

		//2.socket��������
		char buffer[100];
		int len;
		cout << "socket listen start and wait for msg" << endl;
		if ((len = recv(clientPython, buffer, 100 * sizeof(char), 0)) < 0)
			perror("recv");
		buffer[len] = '\0';


		//3.������յ���buffer��ת��Ϊres[7]���洢��������
		static int res[7] = { 0 };
		string s2;
		stringstream sstream(buffer);
		stringstream tmp;
		int i = 0;
		while (getline(sstream, s2, ' ')) {
			tmp << s2;
			tmp >> res[i];
			tmp.clear();
			//cout << "res " << i << ":" << res[i] << endl; 
			i++;
		}

		closesocket(clientPython);
		cout << "socket over" << endl;
	
		return res;
	}
		
	int* Socket::socketGetCouponBusinessReality(char* message) {
		//1.socket��ʼ������������
		char* ip = "10.28.164.209";
		int host = 12347;
		SOCKET clientPython = socketInit(ip,host);


		std::cout << "send msg to python: " << message << std::endl;
		send(clientPython, message, strlen(message) * sizeof(char), 0);//������Ϣ��python��server

		//2.socket��������
		char buffer[100];
		int len;
		cout << "socket listen start and wait for msg" << endl;
		if ((len = recv(clientPython, buffer, 100 * sizeof(char), 0)) < 0)
			perror("recv");
		buffer[len] = '\0';


		//3.������յ���buffer��ת��Ϊres[3]���洢��������
		static int res[3] = { 0 };
		string s2;
		stringstream sstream(buffer);
		stringstream tmp;
		int i = 0;
		while (getline(sstream, s2, ' ')) {
			tmp << s2;
			tmp >> res[i];
			tmp.clear();
			//cout << "res " << i << ":" << res[i] << endl; 
			i++;
		}

		closesocket(clientPython);
		cout << "socket over" << endl;

		return res;
	}

}




