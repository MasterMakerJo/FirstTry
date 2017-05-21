/*******************************************************************
*
*  文件名称: MonteCarloSearchEngine.h
*  简要描述:使用蒙特卡洛搜索树算法搜牌
*
*  创建日期:2016-6
*  作者:范晓梦
*  说明:koudingkouding@gmail.com 有疑问请邮箱
******************************************************************/
//修改记录：
//修改人：杨丽颖
//修改日期：2017/04/21
//修改内容：补充完善
#pragma once
#include <windows.h>
#include <list>
#include <map>
#include "RecommendCards.h"
#include<math.h>
//#include <spdlog.h>
#define MaxRunTime 10//允许执行蒙特卡洛算法的最大时间
//平台有约定，超过15秒认为AI自动放弃，所以只能运行10秒
class MonteCarloSearchEngine
{
public:
	MonteCarloSearchEngine();
	~MonteCarloSearchEngine();
	void Init();
	void SearchAGoodMove(Ddz *pDdz);
	int RunMonteCarloSimulation(int node);
	Ddz *pRealDdz;
	int iAllPlayTimes;//总共执行蒙特卡洛模拟的次数
	int iMaxDepth;//最大深度
	fDdz *pMyDdzBoard;
	fDdz *pUpDdzBoard;
	fDdz *pDownDdzBoard;
	int iTotalPlay;//总共模拟的次数
	int iMyUpPlayerCards[21];
	int iMyDownPlayerCards[21];
	int ibestMove[21];//最好的出牌方法
	RecommendCards *pRecomCards;
	//蒙特卡洛树节点
	struct MTCNode
	{
		int node;//节点下标
		int iWin;//本阵营模拟赢次数
		int iPlay;//本节点被访问次数
		float fUCB1;//UCB1值，初始值为-1 UCB是什么
		float fAverage;//平均胜利值吧？
		int iCards[21];//该节点出牌
	};
	int iNodesCount;
	int iLastValidCards[21];//最后一次有效出牌
	MTCNode *nodesList;
	//int iPlaArr[20][21];
	CardsInfo *pCinfo;
	int iDepthPos[4];//每层节点在iPLArr起始位置下标
	//
	// 计算并更新本节点UCB1值
	void UpdateUCB1(MTCNode nodes[], int node, int Winner, int iTotalPlay);
	void Update(int node, int iDepth);
	// 产生节点，并返回节点数
	int MakeMove(int iDepth);
	// 将该层生成的节点添加到列表中
	//int AddDepthNodesInList(int iDepth);
	int IsGameOver();
	void GetOtherPlayerCards(int* iUpPlayerCards, int* iDownPlayerCards);//随机
	void InitBoard();
	// 左开右闭
	int GetRandomNumFromRange(int iStart, int iEnd,int iDepth);
	int nodeSeed;
	/////测试用的数据
	int AIWin;
	int UpWin;
	int DownWin;

};

