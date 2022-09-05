//TcpClient.exe

#include<stdio.h>
#include <Winsock2.h>
#include<WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include<iostream>
using namespace std;
#include<windows.h>

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
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//参数：协议类型、通信载体（字节流）、ip保护方式
	if (clientSocket == SOCKET_ERROR) {
		cout << "创建socket失败!" << GetLastError() << endl;
		//9.清楚版本信息
		WSACleanup();
		return -1;
	}
	cout << "socket创建成功！" << endl;

	//3.获取服务器协议地址族
	//就是一个结构体
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, "192.168.31.100", (void*)&addr.sin_addr.S_un.S_addr);
	//addr.sin_addr.S_un.S_addr = inet_addr("192.168.31.100");//局域网地址
	addr.sin_port = htons(9999);//设置端口号 小端转大端

	//4.连接服务器
	int r = connect(clientSocket, (sockaddr*)&addr,sizeof addr);
	if (r == -1) {
		cout << "连接服务器失败：" << GetLastError() << endl;
		//6.关闭socket
		closesocket(clientSocket);
		//7 清理版本信息
		WSACleanup();
		return -1;
	}
	cout << "服务器连接成功!" << endl;

	//5.通信
	char buff[1024];
	while (1) {
		cin >> buff;//用户输入信息
		send(clientSocket, buff, strlen(buff), NULL);//发送信息


	}

	//6.关闭socket
	closesocket(clientSocket);

	//7.清理协议版本信息
	WSACleanup();

	while (1);//停顿
	return 0;
}
