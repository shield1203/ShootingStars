#pragma once

#include "happyhttp.h"
#include <cstdio>
#include <cstring>
#include<iostream>
#include<vector>

#ifdef _WIN32
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <ws2tcpip.h>
#endif

class RankBoard {
public:
	std::string name;
	int nScore;

	float fYPos = 800.0f;
};

class CRank {
public:
	static int nDataCount;

	static std::vector<RankBoard*> Ranking;

	void GetInsert(std::string name, std::string score) {
		happyhttp::Connection conn("203.232.193.170", 3000);
		conn.setcallbacks([](const happyhttp::Response* r, void* userdata) { nDataCount = 0; },
			[](const happyhttp::Response* r, void* userdata, const unsigned char* data, int n) { nDataCount += n; },
			[](const happyhttp::Response* r, void* userdata) {}, 0);

		std::string insert = "/insert?name=&score=";
		insert.insert(13, name);
		insert.append(score);
		conn.request("GET",insert.c_str() , 0, 0, 0);

		while (conn.outstanding())
			conn.pump();
	}

	void GetList()
	{
		happyhttp::Connection conn("203.232.193.170", 3000);
		conn.setcallbacks([](const happyhttp::Response* r, void* userdata) { nDataCount = 0; },
			[](const happyhttp::Response* r, void* userdata, const unsigned char* data, int n) 
			{ 
				std::string str(reinterpret_cast<const char*>(data), n);
				if (str != "ENDGAME") {
					/*std::vector<RankBoard*>::iterator iter;
					for (iter = Ranking.begin(); iter != Ranking.end();) {
						iter = Ranking.erase(iter);
						delete[] * iter;
					}*/

					int nindex = 1;

					for (int i = 1; i <= 10; i++) {
						RankBoard* rank = new RankBoard();
						rank->name.empty();
						std::string strScore;
						strScore.empty();

						int acount = 0;
						while (acount != 5) {
							if (data[nindex] == '"') {
								acount++;
							}
							nindex++;
						}

						for (int i = 0; i < 3; i++) {
							rank->name.push_back(data[nindex]);
							nindex++;
						}

						nindex += 10;

						short scoreIndex = 0;
						while (data[nindex] != '}') {
							strScore.push_back(data[nindex]);;
							scoreIndex++;
							nindex++;
						}
						nindex += 2;

						rank->nScore = atoi(strScore.c_str()); // 문자열 숫자로 변환
						Ranking.push_back(rank);
					}
				}

				nDataCount += n; 
			},
			[](const happyhttp::Response* r, void* userdata) {}, 0);

		conn.request("GET", "/list", 0, 0, 0);

		while (conn.outstanding())
			conn.pump();
	}

	void RequestList()
	{
#ifdef _WIN32
		WSAData wsaData;
		int code = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (code != 0)
		{
			fprintf(stderr, "shite. %d\n", code);
			return;
		}
#endif //_WIN32
		try
		{
			GetList();
		}

		catch (happyhttp::Wobbly& e)
		{
			system("pause");
			fprintf(stderr, "Exception:\n%s\n", e.what());
		}

#ifdef _WIN32
		WSACleanup();
#endif // _WIN32

		return;
	}

	void RequestInsert(std::string name, std::string score)
	{
#ifdef _WIN32
		WSAData wsaData;
		int code = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (code != 0)
		{
			fprintf(stderr, "shite. %d\n", code);
			return;
		}
#endif //_WIN32
		try
		{
			GetInsert(name, score);
		}

		catch (happyhttp::Wobbly& e)
		{
			system("pause");
			fprintf(stderr, "Exception:\n%s\n", e.what());
		}

#ifdef _WIN32
		WSACleanup();
#endif // _WIN32

		return;
	}

	CRank() {

	}
	
	~CRank() {
		std::vector<RankBoard*>::iterator iter;
		/*for (iter = Ranking.begin(); iter != Ranking.end();) {
			iter = Ranking.erase(iter);
			delete[] *iter;
		}*/
		Ranking.clear();
	}
};