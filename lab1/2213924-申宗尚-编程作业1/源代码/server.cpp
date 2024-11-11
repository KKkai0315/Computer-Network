#include<iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <winsock2.h>  // �׽��ֽӿ�
#include <Ws2tcpip.h>  // �׽��ֺ�����2
#include <time.h>
#include<map>
#include<thread>
#pragma comment (lib, "ws2_32.lib")  // �׽���ʵ��

#define PORT 8000  // �����˿�
#define MaxBufSize 1024  // ��󻺳�����С
using namespace std;
time_t t;
char time_str[64];
// �û�״̬
map<SOCKET, int> user_map;  // ���е�int���Ϊ1��Ϊ����
SOCKET sockSrv;
bool isServerRunning = true;
// �̴߳���������
DWORD WINAPI handlerRequest(LPVOID lparam);

// �˳������̺߳���
void ServerInputThread() {
	string input;
	while (isServerRunning) {
		getline(cin, input);
		if (input == "exit" || input == "Exit" || input == "EXIT" 
			|| input == "quit" || input == "Quit" || input == "QUIT")
		{  // �˳��ж�
			isServerRunning = false;
			closesocket(sockSrv); // �رշ�����socket
			break;
		}
	}
}

int main()
{
	cout << "                    _oo0oo_" << endl;
	cout << "                   088888880" << endl;
	cout << "                   88\" . \"88" << endl;
	cout << "                   (| -_- |)" << endl;
	cout << "                    0\\ = /0" << endl;
	cout << "                 ___/'---'\\___" << endl;
	cout << "               .' \\\\\\\\|     |// '." << endl;
	cout << "              / \\\\\\\\|||  :  |||// \\\\" << endl;
	cout << "             /_ ||||| -:- |||||- \\\\" << endl;
	cout << "            |   | \\\\\\\\\\  -  /// |   |" << endl;
	cout << "            | \\_|  ''\\---/''  |_/ |" << endl;
	cout << "            \\  .-\\__  '-'  __/-.  /" << endl;
	cout << "          ___'. .'  /--.--\\  '. .'___" << endl;
	cout << "       .\"\" '<  '.___\\_<|>_/___.' >'  \"\"." << endl;
	cout << "      | | : '-  \'.;'\ _ /';.'/ - ' : | |" << endl;
	cout << "      \\  \\ '_.   \\_ __\\ /__ _/   .-' /  /" << endl;
	cout << "  ====='-.____'.___ \\_____/___.-'____.-'=====" << endl;
	cout << "                    '=---='" << endl;
	cout << endl;
	cout << "===============================================" << endl;
	cout << "          ���汣��    iii    ��������" << endl;
	cout << "          �����޷�           ��������" << endl;
	cout << "               ����������v2.0 " << endl;
	cout << "                    By KKkai" << endl;
	cout << "===============================================" << endl;
	cout << "==================�����׼��===================" << endl;
	//========================��ʼ��Socket DLL=========================
	WORD wVersionRequested = MAKEWORD(2, 2);  // �汾����MAKEWORD(�ΰ汾��, ���汾��)
	WSAData wsadata;  // �׽���ʵ�ֵ���ϸ��Ϣ
	int err = WSAStartup(wVersionRequested, &wsadata);  // err��WSAStartup�����ķ���ֵ�����������
	if (err != NO_ERROR) {
		cout << "[����] ��ʼ��Socket DLLʧ�ܣ��������: " << WSAGetLastError() << endl;
		cout << "===============================================" << endl;
		system("pause");
		return 1;
	}
	cout << "[ϵͳ��ʾ] ��ʼ��Socket DLL�ɹ���" << endl;
	//============================����Socket===========================
	sockSrv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // ����ʵ�����InternetЭ��汾4��IPv4����ַ�塢��ʽ�׽����Լ�TCPЭ��
	if (sockSrv == INVALID_SOCKET) {
		cout << "[����] ����Socketʧ�ܣ��������: " << WSAGetLastError() << endl;
		cout << "===============================================" << endl;
		WSACleanup();  // �ͷ� Socket DLL ��Դ
		system("pause");
		return 1;
	}
	cout << "[ϵͳ��ʾ] ����Socket�ɹ���" << endl;
	//===================��ʼ����������ַ��bind���󶨣�=================
	SOCKADDR_IN serverAddr;  // ����˵�ַ��Ϣ�����ݽṹIPv4
	serverAddr.sin_family = AF_INET;  // Э���壬IPv4��ַ��
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr.s_addr);  // ��ַ
	serverAddr.sin_port = htons(PORT);  // �˿ں�
	err = bind(sockSrv, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (err == SOCKET_ERROR) {
		cout << "[����] bind ��ʧ�ܣ��������: " << WSAGetLastError() << endl;
		cout << "===============================================" << endl;
		closesocket(sockSrv);
		WSACleanup();
		system("pause");
		return 1;
	}
	cout << "[ϵͳ��ʾ] bind �󶨶˿� " << PORT << " �ɹ���" << endl;
	//============================���� listen==========================
	err = listen(sockSrv, SOMAXCONN); // ���������г���ΪSOMAXCONN
	if (err == SOCKET_ERROR)
	{
		cout << "[����] ����ʧ�ܣ��������: " << WSAGetLastError() << endl;
		cout << "===============================================" << endl;
		system("pause");
		return 1;
	}
	cout << "[ϵͳ��ʾ] �����ɹ���" << endl;
	// ���������׼�������ʾ
	cout << "===============================================" << endl;
	// ��ȡʱ��
	t = time(NULL);
	ctime_s(time_str, sizeof(time_str), &t);
	cout << "[ϵͳ��ʾ] ������׼����ɣ��ȴ��ͻ�������......" << endl;
	cout << "[ϵͳʱ��] " << time_str;
	cout << "===============================================" << endl;
	thread inputThread(ServerInputThread);
	cout << "***********************************************" << endl;
	cout << "        �˳�˵����" << endl;
	cout << "        $ exit/Exit/EXIT" << endl;
	cout << "        $ quit/Quit/QUIT" << endl;
	cout << "        $ �رճ���" << endl;
	cout << "-----------------------------------------------" << endl;
	cout << "===================ϵͳ��־====================" << endl;

	while (isServerRunning) {
		SOCKADDR_IN clientAddr;
		int len = sizeof(SOCKADDR);
		// ���պ���accept()
		SOCKET sockAccept = accept(sockSrv, (SOCKADDR*)&clientAddr, &len);
		if (!isServerRunning) {
			// Exit command received, don't accept new connections
			break;
		}
		if (sockAccept == INVALID_SOCKET) {
			cout << "[����] accept ʧ�ܣ��������: " << WSAGetLastError() << endl;
			cout << "===============================================" << endl;
			closesocket(sockSrv);
			WSACleanup();
			system("pause");
			return 1;
		}
		else {
			// ��ȡʱ��
			t = time(NULL);
			ctime_s(time_str, sizeof(time_str), &t);
			// ��SocketתΪ�ַ�����Ϊ�û�ID
			char user_id[10];
			strcpy_s(user_id, to_string(sockAccept).data());
			cout << "[ϵͳ��־] " << time_str;
			cout << "[ϵͳ��־] �û�[" << user_id << "]�Ѽ�������!" << endl;
			cout << "-----------------------------------------------" << endl;
			// �����̴߳���Client
			HANDLE hThread = CreateThread(NULL, NULL, handlerRequest, LPVOID(sockAccept), 0, NULL);
			// �ر��߳�����
			CloseHandle(hThread);
		}
	}

	inputThread.join(); // �ȴ������߳�
	//============================�ر�Socket===========================
	closesocket(sockSrv); 
	WSACleanup();
	return 0;
}


// �̴߳�����
DWORD WINAPI handlerRequest(LPVOID lparam)
{
	SOCKET ClientSocket = (SOCKET)lparam;
	user_map[ClientSocket] = 1;  // ��1��Ϊ����
	char RecvBuf[MaxBufSize];  // ������Ϣ�Ļ�����
	char SendBuf[MaxBufSize];  // ������Ϣ�Ļ�����
	// ��SocketתΪ�ַ�����Ϊ�û�ID
	char user_id[10];
	strcpy_s(user_id, to_string(ClientSocket).data());

	//=============================���� send===========================
	send(ClientSocket, user_id, 10, 0);

	// ѭ�����ܿͻ�������
	int recvResult;
	int sendResult;
	//==============================ѭ������===========================
	do {
		// ���� recv
		recvResult = recv(ClientSocket, RecvBuf, MaxBufSize, 0);
		if (recvResult > 0) {
			strcpy_s(SendBuf, "�û�[");
			string ClientID = to_string(ClientSocket);
			strcat_s(SendBuf, ClientID.data());
			strcat_s(SendBuf, "]: ");
			strcat_s(SendBuf, RecvBuf);

			// ��ȡʱ��
			t = time(NULL);
			ctime_s(time_str, sizeof(time_str), &t);
			cout << "[ϵͳ��־] " << time_str;
			cout << "[ϵͳ��־] �û�[" << ClientID << "]����Ϣ��" << RecvBuf << endl;
			cout << "-----------------------------------------------" << endl;
			for (auto it : user_map) {
				if (it.first != ClientSocket && it.second == 1) {
					sendResult = send(it.first, SendBuf, MaxBufSize, 0);
					if (sendResult == SOCKET_ERROR) {
						cout << "[����] ��Ϣ����ʧ�ܣ��������: " << WSAGetLastError() << endl;
						cout << "-----------------------------------------------" << endl;
					}
				}
			}
		}
	} while (recvResult != SOCKET_ERROR);

	// ��ȡʱ��
	t = time(NULL);
	ctime_s(time_str, sizeof(time_str), &t);
	// ��ȡ�û�ID
	string ClientID = to_string(ClientSocket);
	user_map[ClientSocket] = 0;
	cout << "[ϵͳ��־] " << time_str;
	cout << "[ϵͳ��־] �û�[" << ClientID << "]���˳�����..." << endl;
	cout << "-----------------------------------------------" << endl;
	closesocket(ClientSocket);
	return 0;
}


