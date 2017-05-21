#include "PokerModelAnalysis.h"


void PokerModelAnalysis::Init()//��ʼ��
{
	UpWrong = 0;
	DownWrong = 0;
	for (int i = 0; i < 8; i++)//һ����ά���飬�����ͣ��������ӣ�������˵���Ͳ�����
	{
		for (int j = 0; j < 40; j++)
		{
			iUtimate[i][j] = -1;
		}
	}
	for (int i = 0; i < 21; i++)//���20�ţ�������ʼ���ϼ��¼ҵ��Ƶ�����
	{
		iOneUtimate[i] = -1;
	}
	for (int i = 0; i < 21; i++)
	{
		iTwoUtimate[i] = -1;
	}
	for (int i = 0; i < 38; i++)//Ӧ���ǳ����Լ�����17�ţ�ʣ�µ��Ʒ���һ������
	{
		Buffer[i] = pMyDdz->iOnOtherHand[i];//��������װ��һ������������
	}
}//��ʼ��������Ϣ
PokerModelAnalysis::PokerModelAnalysis()//���캯����ab�����Ķ���
{
	A = new SplitCard();
	B = new CardsInfo();
}
PokerModelAnalysis::~PokerModelAnalysis()//��������
{
}
void PokerModelAnalysis::GetOtherPokersCards(int *iUpCards, int *iDownCards, Ddz *pDdz)//���������ҵ���
{
	pMyDdz = pDdz;
	Init();//���ó�ʼ���Ǹ�����
	if (pMyDdz->iOTmax <= 2)//��ǰ��������С��2
	{
		GameStart();//������������
		Gaming();
	}
	else//����͵�����һ��
	{
		Gaming();
	}
	for (int i = 0; i < 21; i++)//ѭ�������ϼҵ��������е�Ԫ�����η����ϼ�����
	{
		iUpCards[i] = iOneUtimate[i];//�ϼҵ��ƣ���recommend cards���涨���
	}
	for (int i = 0; i < 21; i++)//ѭ�������¼ҵ��������е�Ԫ�����η����¼�����
	{
		iDownCards[i] = iTwoUtimate[i];//�¼ҵ���
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
	//	if (InitialSum > Standard2)//����ֵ���ڱ�׼ֵ2���Ǻ���
	//	{
	//		LordBest();
	//	}
	//	if (InitialSum >= Standard1 && InitialSum <= Standard2)//����ֵ���ڱ�׼ֵ1�ͱ�׼ֵ2֮�����һ�����
	//	{
	//		Equality();
	//	}
	//	else
	//	{
	//		PartnerBest();
	//	}
	//}
	//else//����
	//{
	//	Equality();
	//}
	for (int i = 0; i < 21; i++)//����ѭ����ѭ�����ұ��е��ƣ�����ֵȫ����Ϊ��1
	{

		iOneDe[i] = -1;//�ϼұ��е���
	}
	for (int i = 0; i < 21; i++)
	{
		iTwoDe[i] = -1;//�¼ұ��е���
	}
	if (pMyDdz->cDir != pMyDdz->cLandlord)//���Ҳ��ǵ�����ʱ��
	{
		Fendipai();//���������
	}
}

void PokerModelAnalysis::Gaming()//ִ��������������ǵ�һ����������
{
	DeleteC();
	Analysis(2);
	Analysis(1);
	Random(Buffer);
}

void PokerModelAnalysis::Analysis(int t)
{
	int iCs[21];//���Ƶ����鰡�����20�ţ�21��ֹԽ��
	for (int i = 0; i < 21; i++)
	{
		iCs[i] = -1;//������ѭ��Ȼ��ȫ����ֵΪ��1
	}//���Ǹ����յĳ����ƴ浽iCs����ȥ���ж�
	if (t == 2)//����2���ǵ��� Analysis(2);Ȼ������һ��������1��2ֻ���������벻ͬ��if��� ����2Ӧ���ǽ����ϼҵ����ݣ�����1�ǽ����¼ҵ�����
	{
		for (int i = 0; i < 21; i++)
		{
			iCs[i] = pMyDdz->iOnTable[pMyDdz->iOTmax - 2][i];//�ѳ������飨����ֵ��ʼ��Ϊ-2��ÿ����һ���ƣ���-1��β��Pass��Ϊ-1��162=54*3
		//���ѳ��������ж�Ӧ��ֵ�������������
		}
		DeleteB(iCs, 2);//Ȼ��������delete����
		if (B->IsType0Pass(iCs) == 1)//���pass�˾�ֱ�ӷ��أ������cardsinfo�����
		{
			return;
		}
		if (B->IsType3Single(iCs) == 1)//�ǵ��� ��������ȥ���ú���ĺ���
		{
			DeleteB(iCs, 1);
			Type3(iCs, 2);
			return;
		}
		if (B->IsType4Double(iCs) == 1)//�Ƕ��� ����
		{
			DeleteB(iCs, 1);
			Type3(iCs, 2);//��Ӧ����type4��
			return;
		}
		if (B->IsType6ThreeOne(iCs) == 1)//������һ ����
		{
			DeleteB(iCs, 1);
			Type6(iCs, 2);
			return;
		}
		//if (B->IsType7LinkSingle(iCs) == 1)//�ǵ�˳ ����
		//{
		//	Type7(iCs, 2);
		//	return;
		//}
	}
	if (t == 1)//t��1���������if����е����ݣ��ǵ�ǰ����������1����һ�е�i��Ԫ�ص�ֵ���������cs������
	{
		for (int i = 0; i < 21; i++)
		{
			iCs[i] = pMyDdz->iOnTable[pMyDdz->iOTmax - 1][i];
		}
		DeleteB(iCs, 1);//�����if���ͬ�������ݣ������ж������ʲô���ͣ������ã�1���ж��Ǹ�ʲô
		if (B->IsType0Pass(iCs) == 1)
		{
			return;
		}
		if (B->IsType3Single(iCs) == 1)
		{
			DeleteB(iCs, 2);
			Type3(iCs, 1);//���type3��������ֵ��
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
	while (iOneDe[j] != -1)//ֻҪ�����ϼұ��е���ֵ�����ڣ�1����ѭ����ѭ����Ҳ������ֵ��ʱ��
	{
		j++;
	}
	if (j > pMyDdz->iUpPlayerCards)//����ϼұ��е��ƴ����ϼ����е��������Ͱ�j��ֵ�ָ�Ϊ0
		j = 0;
	one = pMyDdz->iUpPlayerCards - j;//������������������������¼���˱��е��ƻ��ж�����Ҫ�ָ��ϼң���������ط�j��������0����
	j = 0;
	while (iTwoDe[j] != -1)//ͬ�������ݣ���������Ƕ����¼ҵ�
	{
		j++;
	}
	if (j > pMyDdz->iDownPlayerCards)
		j = 0;
	two = pMyDdz->iDownPlayerCards - j;//��¼���˱��е��ƻ��ж�����Ҫ�ָ��¼�
	i = one + two;//
	std::vector<int> vi(i);//���ѵ�����һ��������,����һ��治֪����ʲô��˼
	//srand(unsigned(time(NULL)));
	for (int n = 0; n<i; n++)
	{
		vi[n] = iBuffer[n];
	}
	random_shuffle(vi.begin(), vi.end());
	j = 0;
	vector<int>::iterator it;
	for (it = vi.begin(); it != vi.begin() + one; it++)//���ϼ��������
	{
		iOneUtimate[j] = *it;
		j++;
	}
	for (k = 0; iOneDe[k] != -1; k++)//���ϼұ��е����ӽ�ȥ
	{
		iOneUtimate[j] = iOneDe[k];
		j++;
	}
	j = 0;
	for (it = vi.begin() + one; it != vi.end(); it++)//���¼��������
	{
		iTwoUtimate[j] = *it;
		j++;
	}
	k = 0;
	while (iTwoDe[k] != -1)//���¼ұ��е����ӽ�ȥ
	{

		iTwoUtimate[j] = iTwoDe[k];
		j++;
		k++;
	}
	
}

void PokerModelAnalysis::Type3(int *iCs, int k)
{
	int a = iCs[0] / 4;//��4ȡ��
	for (int i = 0; Buffer[i] != -1; i++)//ѭ���Ǹ�buffer���飬ֻҪ��ֵ��һֱѭ��
	{
		if (a == Buffer[i] / 4)
		{
			if (k == 2)//����2��ʱ�����ϼ�
			{
				int j = 0;
				while (iOneDe[j] != -1)
				{
					j++;
				}
				iOneDe[j] = Buffer[i];//��buffer�����е����ݷŽ��ϼұ��е���������
				for (int x = i; Buffer[x] != -1; x++)//���������ѭ����
				{
					Buffer[x] = Buffer[x + 1];//Ԫ����ǰ��
				}
				i--;//����ȡ����������ɾ����
			}
			if (k == 1)//����1��ʱ�����¼�
			{
				int j = 0;
				while (iTwoDe[j] != -1)//ֻҪ��Ϊ��1����һֱ�����������
				{
					j++;
				}
				iTwoDe[j] = Buffer[i];//�����������ֵ�����¼ұ��е���
				for (int x = i; Buffer[x] != -1; x++)//����ͬ�����һ��
				{
					Buffer[x] = Buffer[x + 1];//������ǰ��
				}
				i--;//����ȡ����������ɾ����
			}
		}
	}
	Random(Buffer);//ִ��random�������������Ǵ�ַ���ã�
}

void PokerModelAnalysis::Type6(int *iCs, int k)//��������������ж�ʲô��
{
	int a = 0;
	int b = 0;
	a = iCs[0] / 4;
	for (int i = 0; i < 4; i++)//ѭ��������������е�����
	{
		if (iCs[i] / 4 != a)//��ǰ�������Ʋ���ȵ�ʱ�򣬾ͽ����µĽ�������������Ƹ���b
		{
			b = iCs[i] / 4;
		}
	}
	if (k = 2)//����2�����������ϼҲ��ֵ�
	{
		for (int i = 0; Buffer[i] != -1; i++)
		{
			if (Buffer[i] / 4 == a)//�����������е������Ƶĵ�����ics�еĵ�һ����ͬ��ʱ��
			{
				int j = 0;
				while (iOneDe[j] != -1)//�Ϳ�ʼѭ���ϼұ��е����������
				{
					j++;
				}
				iOneDe[j] = Buffer[i];//�����������е�ֵ�ŵ��ϼұ��е���������
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];//��Ԫ��ǰ�ƣ����鳤�ȼ�С
				}
				i--;
			}
			if (Buffer[i] / 4 == b)//����ʱ�����ƺ�iCs[i] / 4;��ȵ�ʱ�򣬾�ִ��ͬ���Ĳ�������Ȼ������if���ִֻ��һ��
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
	if (k = 1)//����1���¼Ҳ���
	{
		for (int i = 0; Buffer[i] != -1; i++)//����ͬ��
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
				iOneDe[x] = Buffer[i];//������ƺ�a��ȣ��ͽ����ŵ��ϼұ��е���������
				x++;//��������
			}
		}
		if (b != 12)//��b����2��ʱ��
		{
			for (int i = 0; Buffer[i] != -1; i++)//�͵��ڱ���buffer����
			{
				if (Buffer[i] / 4 == b)//��b��buffer�����е��Ƶ���ֵһ�����ͽ�buffer�����е�ֵ�����ϼұ��е���������
				{
					iOneDe[x] = Buffer[i];
					x++;//������������
				}
			}
		}
	}
	if (k == 1)//ͬ��
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
	InitialSum = 0;//��ʼ����Ϊ0
	ConfirmCards(pCards);//����ȷ���ƺ���

	if (iUtimate[0][0] != -1)//��û�ж���   �ѵ������ά����ĵ�һ��Ԫ�ؾ��������Ŷ�����
	{
		InitialSum = InitialSum + 10;//��������ֵ��������10��
	}
	for (int i = 0; iUtimate[1][i] != -1; i++)//��û����ը�� ��ը����������5��ǰ���ĸ��ը����
	{
		if (iUtimate[1][i] == -2)//���ڣ�2��ʲô״̬
		{
			i++;
			continue;
		}
		else
		{
			InitialSum = InitialSum + 9;//��������ֵ��ը����9��
			i = i + 4;//������
		}
	}
	for (int i = 0; iUtimate[2][i] != -1; i = i + 4)//��û������ ������������4�񣬾���������ͬ����
	{
		InitialSum = InitialSum + 8;//��������ֵ
	}
	for (int i = 0; iUtimate[3][i] != -1; i++)//�ɻ�
	{
		int j;
		for (j = 0; iUtimate[3][j] != -2; j = j + 3)//���forѭ�����������û�����ݣ��ǲ���Ӧ�ý�����ļ�������ֵ�����Ž�ȥ
		{
		}
		InitialSum = InitialSum + 7;//����jֵ����ֵ�����������ֵ
		i = i + j;//��i������һ����˳������
	}
	for (int i = 0; iUtimate[4][i] != -1; i++)//��û��˳�� ��˳�Ӿͽ���ѭ��
	{
		int j;
		for (j = 0; iUtimate[4][j] != -2; j++)//��˳�� ��Jֵ�����˳�ӵĳ��ȣ�����5����
		{
			//�������Ӧ��дʲô��
		}
		InitialSum = InitialSum + 6;//����jֵ����ֵ�����������ֵ
		i = i + j;//ʹi������һ��˳�ӵ�����
	}
	for (int i = 0; iUtimate[5][i] != -1; i++)
	{
		int j;
		for (j = 0; iUtimate[5][j] != -2; j = j + 2)//�ж�����
		{
		}
		InitialSum = InitialSum + 5;//����jֵ����ֵ�����������ֵ
		i = i + j;//ʹi������һ�����Ե�����
	}
	for (int i = 0; iUtimate[6][i] != -1; i = i + 3)//��û�ж��� �ж��Ӿ�������3��
	{
		InitialSum = InitialSum + 4;//��������ֵ
	}
	for (int i = 0; iUtimate[7][i] != -1; i = i + 2)//��û�е��� �е��ƾ�������2��
	{
		InitialSum = InitialSum + 3;//��������ֵ
	}
}

void PokerModelAnalysis::LordBest()//������������õ���
{
	//ConfirmCards(pMyDdz->iOnOtherHand);
	//Undone
	int k = 0;
	if (k = 0)//k��0���¼ҵ�������
	{
		for (int i = 0; i < 37; i++)//���������ϵ��ƣ�ʣ37��
		{
			if (i < 17)//Ȼ��ǰ���17��
			{
				iOneUtimate[i] = pMyDdz->iOnOtherHand[i];//���������ϵ��Ƶ�ǰ��17�Ŷ������ϼҵ���
			}
			else
			{
				iTwoUtimate[k] = pMyDdz->iOnOtherHand[i];// ʣ�µ�20�Ŷ��������¼�
			}
		}
	}
	else//k������0��ʱ�����µ����ݲ���һ����
	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 20)
			{
				iTwoUtimate[i] = pMyDdz->iOnOtherHand[i];//����ְ�20���Ƹ����¼���
			}
			else
			{
				iOneUtimate[k] = pMyDdz->iOnOtherHand[i];
				k++;
			}
		}
	}
}

void PokerModelAnalysis::PartnerBest()//�����ѷ�����õ���
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

void PokerModelAnalysis::Equality()//�����ҷ��������
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

void PokerModelAnalysis::ConfirmCards(int *pCards)//ȷ���ƣ�
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
	if (a == 2)//Ҳ���ж���ȣ�Ȼ��Ԫ��ǰ�ƣ�ͬ�����ط�һ��������ʲô��
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
	if (pMyDdz->cLandlord + 1 == pMyDdz->cDir)//�������ҷ��ϼң����ǵ����ķ�λ��˳ʱ���ƶ�һ�������������ڵķ�λ�������ҵ��ϼҾ��ǵ���
	{
		for (int i = 0; i < 3; i++)
		{
			iOneDe[i] = pMyDdz->iLef[i];//���ν�3�ŵ��Ʒ����ϼұ��е���������
		}
		for (int i = 0; Buffer[i] != -1; i++)//ֻҪ�����������ֵ��Ϊ��1����һֱѭ������ά�����ѭ��
		{
			for (int j = 0; j < 3; j++)//�˴�Ҳ��ѭ�����ŵ���
			{
				if (Buffer[i] == pMyDdz->iLef[j])//�����������е�Ԫ�ص��ڻ����������Ԫ��ʱ
				{
					for (int k = i; Buffer[k] != -1; k++)
					{
						Buffer[k] = Buffer[k + 1];//��ǰ��
					}
					i--;
				}
			}
		}
	}
	if (pMyDdz->cLandlord - 1 == pMyDdz->cDir)//ͬ���ҵ��¼��ǵ���
	{
		for (int i = 0; i < 3; i++)//���ν�3�ŵ��Ʒ����¼ұ��е���������
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
			if (Buffer[j] == iOneDe[i])//����ѭ������ϼұ��е��ƺͻ������������ʱ���ͽ���������Ԫ��������ǰ��
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
	for (int i = 0; iTwoDe[i] != -1; i++)//����ͬ�ϣ��˴�������¼Ҷ��Ե�
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