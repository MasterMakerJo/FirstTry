#include "PokerModelAnalysis.h"


void PokerModelAnalysis::Init()//初始化
{
	UpWrong = 0;
	DownWrong = 0;
	for (int i = 0; i < 8; i++)//一个二维数组，放牌型，可以增加，，不是说牌型不够吗
	{
		for (int j = 0; j < 40; j++)
		{
			iUtimate[i][j] = -1;
		}
	}
	for (int i = 0; i < 21; i++)//最多20张，用来初始化上家下家的牌的数组
	{
		iOneUtimate[i] = -1;
	}
	for (int i = 0; i < 21; i++)
	{
		iTwoUtimate[i] = -1;
	}
	for (int i = 0; i < 38; i++)//应该是除开自家手牌17张，剩下的牌放入一个数组
	{
		Buffer[i] = pMyDdz->iOnOtherHand[i];//把其他牌装入一个缓冲数组存放
	}
}//初始化牌型信息
PokerModelAnalysis::PokerModelAnalysis()//构造函数，ab变量的定义
{
	A = new SplitCard();
	B = new CardsInfo();
}
PokerModelAnalysis::~PokerModelAnalysis()//析构函数
{
}
void PokerModelAnalysis::GetOtherPokersCards(int *iUpCards, int *iDownCards, Ddz *pDdz)//获得其他玩家的牌
{
	pMyDdz = pDdz;
	Init();//调用初始化那个函数
	if (pMyDdz->iOTmax <= 2)//当前出牌手数小于2
	{
		GameStart();//调用两个函数
		Gaming();
	}
	else//否则就调用这一个
	{
		Gaming();
	}
	for (int i = 0; i < 21; i++)//循环，将上家的牌数组中的元素依次放入上家牌组
	{
		iUpCards[i] = iOneUtimate[i];//上家的牌，在recommend cards里面定义的
	}
	for (int i = 0; i < 21; i++)//循环，将下家的牌数组中的元素依次放入下家牌组
	{
		iDownCards[i] = iTwoUtimate[i];//下家的牌
	}
}

void PokerModelAnalysis::GameStart()
{
	//CardsValue(Hc);
	//int size = sizeof(iUtimate);
	//int i = 0;
	//memcpy(iMyUtimate, iUtimate, size);
	//if (i = 0)
	//{
	//	if (InitialSum > Standard2)//牌力值大于标准值2就是好牌
	//	{
	//		LordBest();
	//	}
	//	if (InitialSum >= Standard1 && InitialSum <= Standard2)//牌力值介于标准值1和标准值2之间就是一般的牌
	//	{
	//		Equality();
	//	}
	//	else
	//	{
	//		PartnerBest();
	//	}
	//}
	//else//烂牌
	//{
	//	Equality();
	//}
	for (int i = 0; i < 21; i++)//还是循环，循环两家必有的牌，将初值全都赋为－1
	{

		iOneDe[i] = -1;//上家必有得牌
	}
	for (int i = 0; i < 21; i++)
	{
		iTwoDe[i] = -1;//下家必有得牌
	}
	if (pMyDdz->cDir != pMyDdz->cLandlord)//当我不是地主的时候
	{
		Fendipai();//调这个函数
	}
}

void PokerModelAnalysis::Gaming()//执行这个函数，又是调一堆其他函数
{
	DeleteC();
	Analysis(2);
	Analysis(1);
	Random(Buffer);
}

void PokerModelAnalysis::Analysis(int t)
{
	int iCs[21];//放牌的数组啊，最大20张，21防止越界
	for (int i = 0; i < 21; i++)
	{
		iCs[i] = -1;//是依次循环然后全部赋值为－1
	}//把那个狗日的出的牌存到iCs里面去做判断
	if (t == 2)//等于2就是调用 Analysis(2);然后这是一个函数，1，2只是用来进入不同的if语句 等于2应该是介入上家的内容，等于1是介入下家的内容
	{
		for (int i = 0; i < 21; i++)
		{
			iCs[i] = pMyDdz->iOnTable[pMyDdz->iOTmax - 2][i];//已出牌数组（所有值初始化为-2）每行是一手牌，以-1结尾，Pass记为-1，162=54*3
		//将已出牌数组中对应的值放入这个数组中
		}
		DeleteB(iCs, 2);//然后调用这个delete函数
		if (B->IsType0Pass(iCs) == 1)//如果pass了就直接返回，这个是cardsinfo里面的
		{
			return;
		}
		if (B->IsType3Single(iCs) == 1)//是单牌 分析，就去调用后面的函数
		{
			DeleteB(iCs, 1);
			Type3(iCs, 2);
			return;
		}
		if (B->IsType4Double(iCs) == 1)//是对牌 分析
		{
			DeleteB(iCs, 1);
			Type3(iCs, 2);//不应该是type4吗
			return;
		}
		if (B->IsType6ThreeOne(iCs) == 1)//是三带一 分析
		{
			DeleteB(iCs, 1);
			Type6(iCs, 2);
			return;
		}
		//if (B->IsType7LinkSingle(iCs) == 1)//是单顺 分析
		//{
		//	Type7(iCs, 2);
		//	return;
		//}
	}
	if (t == 1)//t＝1，进到这个if语句中的内容，是当前出牌手数减1的那一行的i个元素的值，放入这个cs数组中
	{
		for (int i = 0; i < 21; i++)
		{
			iCs[i] = pMyDdz->iOnTable[pMyDdz->iOTmax - 1][i];
		}
		DeleteB(iCs, 1);//下面的if语句同上面内容，就是判断这个是什么牌型，但是用＝1来判断是搞什么
		if (B->IsType0Pass(iCs) == 1)
		{
			return;
		}
		if (B->IsType3Single(iCs) == 1)
		{
			DeleteB(iCs, 2);
			Type3(iCs, 1);//这个type3是用来赋值的
			return;
		}
		if (B->IsType4Double(iCs) == 1)
		{
			DeleteB(iCs, 2);
			Type3(iCs, 1);
			return;
		}
		if (B->IsType6ThreeOne(iCs) == 1)
		{
			DeleteB(iCs, 2);
			Type6(iCs, 1);
			return;
		}
		//if (B->IsType7LinkSingle(iCs) == 1)
		//{
		//	Type7(iCs, 1);
		//	return;
		//}
	}
}

void PokerModelAnalysis::Random(int *iBuffer)
{
	int i = 0;
	int j = 0;
	int one = 0;
	int two = 0;
	int k = 0;
	while (iOneDe[j] != -1)//只要满足上家必有的牌值不等于－1，就循环啊循环，也就是有值的时候
	{
		j++;
	}
	if (j > pMyDdz->iUpPlayerCards)//如果上家必有的牌大于上家手中的牌数，就把j的值又改为0
		j = 0;
	one = pMyDdz->iUpPlayerCards - j;//／／／／／／／／／／／记录除了必有的牌还有多少牌要分给上家，可是这个地方j不都等于0了吗
	j = 0;
	while (iTwoDe[j] != -1)//同样的内容，这个部分是对于下家的
	{
		j++;
	}
	if (j > pMyDdz->iDownPlayerCards)
		j = 0;
	two = pMyDdz->iDownPlayerCards - j;//记录除了必有的牌还有多少牌要分给下家
	i = one + two;//
	std::vector<int> vi(i);//这难道不是一个向量吗,这个我还真不知道是什么意思
	//srand(unsigned(time(NULL)));
	for (int n = 0; n<i; n++)
	{
		vi[n] = iBuffer[n];
	}
	random_shuffle(vi.begin(), vi.end());
	j = 0;
	vector<int>::iterator it;
	for (it = vi.begin(); it != vi.begin() + one; it++)//给上家随机分配
	{
		iOneUtimate[j] = *it;
		j++;
	}
	for (k = 0; iOneDe[k] != -1; k++)//把上家必有的牌扔进去
	{
		iOneUtimate[j] = iOneDe[k];
		j++;
	}
	j = 0;
	for (it = vi.begin() + one; it != vi.end(); it++)//给下家随机分配
	{
		iTwoUtimate[j] = *it;
		j++;
	}
	k = 0;
	while (iTwoDe[k] != -1)//把下家必有的牌扔进去
	{

		iTwoUtimate[j] = iTwoDe[k];
		j++;
		k++;
	}
	
}

void PokerModelAnalysis::Type3(int *iCs, int k)
{
	int a = iCs[0] / 4;//与4取整
	for (int i = 0; Buffer[i] != -1; i++)//循环那个buffer数组，只要有值就一直循环
	{
		if (a == Buffer[i] / 4)
		{
			if (k == 2)//等于2的时候是上家
			{
				int j = 0;
				while (iOneDe[j] != -1)
				{
					j++;
				}
				iOneDe[j] = Buffer[i];//将buffer数组中的内容放进上家必有的牌数组中
				for (int x = i; Buffer[x] != -1; x++)//这个是两重循环吗
				{
					Buffer[x] = Buffer[x + 1];//元素往前移
				}
				i--;//把提取出来的数据删除掉
			}
			if (k == 1)//等于1的时候是下家
			{
				int j = 0;
				while (iTwoDe[j] != -1)//只要不为－1，就一直往数组后续走
				{
					j++;
				}
				iTwoDe[j] = Buffer[i];//将缓冲数组的值赋给下家必有的牌
				for (int x = i; Buffer[x] != -1; x++)//内容同上面的一样
				{
					Buffer[x] = Buffer[x + 1];//还是往前移
				}
				i--;//把提取出来的数据删除掉
			}
		}
	}
	Random(Buffer);//执行random这个函数，这个是传址调用？
}

void PokerModelAnalysis::Type6(int *iCs, int k)//这个到底是用来判断什么的
{
	int a = 0;
	int b = 0;
	a = iCs[0] / 4;
	for (int i = 0; i < 4; i++)//循环的是这个数组中的内容
	{
		if (iCs[i] / 4 != a)//当前后两张牌不相等的时候，就将最新的进行运算的这张牌赋给b
		{
			b = iCs[i] / 4;
		}
	}
	if (k = 2)//对于2是用来处理上家部分的
	{
		for (int i = 0; Buffer[i] != -1; i++)
		{
			if (Buffer[i] / 4 == a)//当缓冲数组中的这张牌的点数和ics中的第一张相同的时候
			{
				int j = 0;
				while (iOneDe[j] != -1)//就开始循环上家必有的牌这个数组
				{
					j++;
				}
				iOneDe[j] = Buffer[i];//将缓冲数组中的值放到上家必有的牌数组中
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];//将元素前移，数组长度减小
				}
				i--;
			}
			if (Buffer[i] / 4 == b)//当此时这张牌和iCs[i] / 4;相等的时候，就执行同样的操作，当然，两条if语句只执行一条
			{
				int j = 0;
				while (iOneDe[j] != -1)
				{
					j++;
				}
				iOneDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
			}
		}
	}
	if (k = 1)//等于1是下家部分
	{
		for (int i = 0; Buffer[i] != -1; i++)//内容同上
		{
			if (Buffer[i] / 4 == a)
			{
				int j = 0;
				while (iTwoDe[j] != -1)
				{
					j++;
				}
				iTwoDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
			}
			if (Buffer[i] / 4 == b)
			{
				int j = 0;
				while (iTwoDe[j] != -1)
				{
					j++;
				}
				iTwoDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
void PokerModelAnalysis::Type7(int *iCs, int k)
{
	int a = 0;
	int b = 0;
	a = iCs[0] / 4 - 1;
	for (int i = 0; iCs[i] != -1; i++)
	{
		b = iCs[i] / 4 + 1;
	}
	if (k == 2)
	{
		int x = 0;
		while (iOneDe[x] != -1)
		{
			x++;
		}
		for (int i = 0; Buffer[i] != -1; i++)
		{
			if (Buffer[i] / 4 == a)
			{
				iOneDe[x] = Buffer[i];//当这个牌和a相等，就将它放到上家必有的牌数组中
				x++;//数组扩容
			}
		}
		if (b != 12)//当b不是2的时候
		{
			for (int i = 0; Buffer[i] != -1; i++)//就等于遍历buffer数组
			{
				if (Buffer[i] / 4 == b)//当b和buffer数组中的牌的牌值一样，就将buffer数组中的值赋给上家必有的牌数组中
				{
					iOneDe[x] = Buffer[i];
					x++;//并且数组扩容
				}
			}
		}
	}
	if (k == 1)//同上
	{
		int x = 0;
		while (iTwoDe[x] != -1)
		{
			x++;
		}
		for (int i = 0; Buffer[i] != -1; i++)
		{
			if (Buffer[i] / 4 == a)
			{
				iTwoDe[x] = Buffer[i];
				x++;
			}
		}
		if (b != 12)
		{
			for (int i = 0; Buffer[i] != -1; i++)
			{
				if (Buffer[i] / 4 == b)
				{
					iTwoDe[x] = Buffer[i];
					x++;
				}
			}
		}
	}
}
void PokerModelAnalysis::CardsValue(int *pCards)
{
	InitialSum = 0;//初始化和为0
	ConfirmCards(pCards);//调用确认牌函数

	if (iUtimate[0][0] != -1)//有没有对王   难道这个二维数组的第一个元素就是用来放对王的
	{
		InitialSum = InitialSum + 10;//增加牌力值，对王加10分
	}
	for (int i = 0; iUtimate[1][i] != -1; i++)//有没有有炸弹 有炸弹就往后推5格，前面四格放炸弹啊
	{
		if (iUtimate[1][i] == -2)//等于－2是什么状态
		{
			i++;
			continue;
		}
		else
		{
			InitialSum = InitialSum + 9;//增加牌力值，炸弹加9分
			i = i + 4;//往后移
		}
	}
	for (int i = 0; iUtimate[2][i] != -1; i = i + 4)//有没有三条 有三条往后推4格，就是三张相同的牌
	{
		InitialSum = InitialSum + 8;//增加牌力值
	}
	for (int i = 0; iUtimate[3][i] != -1; i++)//飞机
	{
		int j;
		for (j = 0; iUtimate[3][j] != -2; j = j + 3)//这个for循环拿来干嘛，又没有内容，是不是应该将后面的计算牌力值的语句放进去
		{
		}
		InitialSum = InitialSum + 7;//根据j值和首值编码计算牌力值
		i = i + j;//是i跳到下一个三顺的区域
	}
	for (int i = 0; iUtimate[4][i] != -1; i++)//有没有顺子 有顺子就进入循环
	{
		int j;
		for (j = 0; iUtimate[4][j] != -2; j++)//有顺子 用J值来检测顺子的长度（至少5个）
		{
			//这个里面应该写什么啊
		}
		InitialSum = InitialSum + 6;//根据j值和首值编码计算牌力值
		i = i + j;//使i跳到下一个顺子的区域
	}
	for (int i = 0; iUtimate[5][i] != -1; i++)
	{
		int j;
		for (j = 0; iUtimate[5][j] != -2; j = j + 2)//判断连对
		{
		}
		InitialSum = InitialSum + 5;//根据j值和首值编码计算牌力值
		i = i + j;//使i跳到下一个连对的区域
	}
	for (int i = 0; iUtimate[6][i] != -1; i = i + 3)//有没有对子 有对子就往后推3格
	{
		InitialSum = InitialSum + 4;//增加牌力值
	}
	for (int i = 0; iUtimate[7][i] != -1; i = i + 2)//有没有单牌 有单牌就往后推2格
	{
		InitialSum = InitialSum + 3;//增加牌力值
	}
}

void PokerModelAnalysis::LordBest()//给地主分配最好的牌
{
	//ConfirmCards(pMyDdz->iOnOtherHand);
	//Undone
	int k = 0;
	if (k = 0)//k＝0是下家当地主吗
	{
		for (int i = 0; i < 37; i++)//除开我手上的牌，剩37张
		{
			if (i < 17)//然后前面的17张
			{
				iOneUtimate[i] = pMyDdz->iOnOtherHand[i];//将其他手上的牌的前面17张都赋给上家的牌
			}
			else
			{
				iTwoUtimate[k] = pMyDdz->iOnOtherHand[i];// 剩下的20张都赋给了下家
			}
		}
	}
	else//k不等于0的时候。上下的内容不都一样吗
	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 20)
			{
				iTwoUtimate[i] = pMyDdz->iOnOtherHand[i];//这个又吧20张牌赋给下家了
			}
			else
			{
				iOneUtimate[k] = pMyDdz->iOnOtherHand[i];
				k++;
			}
		}
	}
}

void PokerModelAnalysis::PartnerBest()//给队友分配最好的牌
{
	int k = 0;
	if (k = 0)
	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 20)
			{
				iTwoUtimate[i] = pMyDdz->iOnOtherHand[i];
			}
			else
			{
				iOneUtimate[k] = pMyDdz->iOnOtherHand[i];
				k++;
			}
		}
	}
	else
	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 17)
			{
				iOneUtimate[i] = pMyDdz->iOnOtherHand[i];
			}
			else
			{
				iTwoUtimate[k] = pMyDdz->iOnOtherHand[i];
				k++;
			}
		}
	}
}

void PokerModelAnalysis::Equality()//给两家分配差不多的牌
{
	int k = 0;
	int j = 0;
	for (int i = 0; i < 37; i++)
	{
		if (i % 2 == 0 && k < 17)
		{
			iOneUtimate[k] = pMyDdz->iOnOtherHand[i];
			k++;
		}
		else
		{
			iTwoUtimate[j] = pMyDdz->iOnOtherHand[i];
			j++;
		}
	}
}

void PokerModelAnalysis::SaveCards(int j, int *pCards)
{
	int *abc = A->startSplitCard(pCards, j);
	int i;
	for (i = 0; abc[i] != -1; i++)
	{
		iUtimate[j][i] = abc[i];
	}
	if (abc[0] != -1)
	{
		DeleteCards(abc);
	}
}

void PokerModelAnalysis::DeleteCards(int *iResult)
{
	int k = 0;
	for (int i = 0; iResult[i] != -1; i++)
	{
		if (iResult[i] != -2)
		{
			for (k = 0; k < 21; k++)
			{
				if (iResult[i] == Hc[k])
				{
					for (; k < 20; k++)
					{
						Hc[k] = Hc[k + 1];
					}
					break;
				}
			}
		}
	}
}

void PokerModelAnalysis::ConfirmCards(int *pCards)//确认牌？
{
	SaveCards(0, pCards);
	SaveCards(1, pCards);
	SaveCards(2, pCards);
	SaveCards(3, pCards);
	SaveCards(4, pCards);
	SaveCards(5, pCards);
	SaveCards(6, pCards);
	SaveCards(7, pCards);
}

void PokerModelAnalysis::DeleteB(int *iCs, int a)
{
	if (a == 2)//也是判断相等，然后将元素前移，同其他地方一样啊，有什么用
	{
		for (int i = 0; iTwoDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iTwoDe[i] == iCs[j])
				{
					for (int k = i; iTwoDe[k] != -1; k++)
					{
						iTwoDe[k] = iTwoDe[k + 1];
					}
					i--;
					j = 0;
				}
			}
		}
		for (int i = 0; iOneDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iOneDe[i] == iCs[j])
				{
					for (int k = i; iOneDe[k] != -1; k++)
					{
						iOneDe[k] = iOneDe[k + 1];
					}
					UpWrong++;
					i--;
					j = 0;
				}
			}
		}
	}
	if (a == 1)
	{
		for (int i = 0; iTwoDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iTwoDe[i] == iCs[j])
				{
					for (int k = i; iTwoDe[k] != -1; k++)
					{
						iTwoDe[k] = iTwoDe[k + 1];
					}
					DownWrong++;
					i--;
					j = 0;
				}
			}
		}
		for (int i = 0; iOneDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iOneDe[i] == iCs[j])
				{
					for (int k = i; iOneDe[k] != -1; k++)
					{
						iOneDe[k] = iOneDe[k + 1];
					}
					i--;
					j = 0;
				}
			}
		}
	}
}

void PokerModelAnalysis::Fendipai()
{
	if (pMyDdz->cLandlord + 1 == pMyDdz->cDir)//地主是我方上家，就是地主的方位再顺时针移动一个，等于我所在的方位，所以我的上家就是地主
	{
		for (int i = 0; i < 3; i++)
		{
			iOneDe[i] = pMyDdz->iLef[i];//依次将3张底牌放入上家必有的牌数组中
		}
		for (int i = 0; Buffer[i] != -1; i++)//只要缓冲区数组的值不为－1，就一直循环，二维数组的循环
		{
			for (int j = 0; j < 3; j++)//此处也是循环三张底牌
			{
				if (Buffer[i] == pMyDdz->iLef[j])//当底牌数组中的元素等于缓冲区数组的元素时
				{
					for (int k = i; Buffer[k] != -1; k++)
					{
						Buffer[k] = Buffer[k + 1];//往前移
					}
					i--;
				}
			}
		}
	}
	if (pMyDdz->cLandlord - 1 == pMyDdz->cDir)//同理，我的下家是地主
	{
		for (int i = 0; i < 3; i++)//依次将3张底牌放入下家必有的牌数组中
		{
			iTwoDe[i] = pMyDdz->iLef[i];
		}
		for (int i = 0; Buffer[i] != -1; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (Buffer[i] == pMyDdz->iLef[j])
				{
					for (int k = i; Buffer[k] != -1; k++)
					{
						Buffer[k] = Buffer[k + 1];
					}
					i--;
				}
			}
		}
	}
}

void PokerModelAnalysis::DeleteC()
{
	for (int i = 0; iOneDe[i] != -1; i++)
	{
		for (int j = 0; Buffer[j] != -1; j++)
		{
			if (Buffer[j] == iOneDe[i])//二重循环嘛，当上家必有的牌和缓冲区的牌相等时，就将缓冲区的元素依次往前移
			{
				for (int k = j; Buffer[k] != -1; k++)
				{
					Buffer[k] = Buffer[k + 1];
				}
				i--;
				j = 0;
			}
		}
	}
	for (int i = 0; iTwoDe[i] != -1; i++)//内容同上，此处是针对下家而言的
	{
		for (int j = 0; Buffer[j] != -1; j++)
		{
			if (Buffer[j] == iTwoDe[i])
			{
				for (int k = j; Buffer[k] != -1; k++)
				{
					Buffer[k] = Buffer[k + 1];
				}
				i--;
				j = 0;
			}
		}
	}
}