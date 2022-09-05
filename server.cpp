//TcpServer_plus.exe

#include<stdio.h>
#include <Winsock2.h>
#include<WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include<iostream>
using namespace std;
#include<windows.h>

SOCKADDR_IN cAddr = { 0 };
int len = sizeof cAddr;
SOCKET clientSocket[1001];//同时最多容纳1000人在线

void chat(int n) {
	char buff[1024];
	int r;
	while (1) {
		r = recv(clientSocket[n], buff, 1023, NULL);
		if (r > 0) {
			buff[r] = 0;
			//cout << inet_ntoa(cAddr.sin_addr) << "发来的消息: " << buff;
			printf("%s发来的信息：%s\n", inet_ntop(AF_INET, (void*)&cAddr.sin_addr, buff, 1023), buff);
		}
	}
}

int main() {

	//1.确定协议版本
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);//2.2版本
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2) {
		cout << "确认版本失败!\n" << endl;
		return -1;
	}
	cout << "确认版本成功!" << endl;

	//2.创建Socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//参数：协议类型、通信载体（字节流）、ip保护方式
	if (serverSocket == SOCKET_ERROR) {
		cout << "创建socket失败!" << GetLastError() << endl;
		//9.清楚版本信息
		WSACleanup();
		return -1;
	}
	cout << "socket创建成功！" << endl;

	//3.创建服务器协议地址族
	//就是一个结构体
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, "192.168.31.100", (void*)&addr.sin_addr.S_un.S_addr);
	//addr.sin_addr.S_un.S_addr = inet_addr("192.168.31.100");//局域网地址
	addr.sin_port = htons(9999);//设置端口号 小端转大端

	//4.绑定
	int r = bind(serverSocket, (struct sockaddr*)&addr, sizeof addr);
	if (r == -1) {
		cout << "绑定失败!" << GetLastError() << endl;
		//8.关闭socket
		closesocket(serverSocket);
		//9.清除版本信息
		WSACleanup();
		return -1;
	}
	cout << "绑定成功！" << endl;

	//5.监听
	r = listen(serverSocket, 10);
	if (r == -1) {
		cout << "监听失败!" << GetLastError() << endl;
		//8.关闭socket
		closesocket(serverSocket);
		//9.清除版本信息
		WSACleanup();
		return -1;
	}
	cout << "监听成功！" << endl;

	//6.等待客户端连接（多客户端连接）
	for (int i = 0; i < 1001; i++) {
		clientSocket[i] = accept(serverSocket, (sockaddr*)&cAddr, &len);//获取客户端的socket
			if (clientSocket[i] == SOCKET_ERROR) {
			cout << "客户端连接失败：" << GetLastError() << endl;
			//关闭socket
			closesocket(serverSocket);
			//9.清楚版本信息
			WSACleanup();
			return -1;
		}
		cout << "客户端连接成功！" << endl;


		//7 通信  多线程实现 chat函数实现
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)chat, (LPVOID)i, NULL, NULL);//多线程实现,加单用可以，但在后期真实项目中需要IO复用进行多用户创建连接
	}
		

	//8.关闭socket
	closesocket(serverSocket);

	//9.清理协议版本信息
	WSACleanup();

	while (1);//停顿
	return 0;
}
