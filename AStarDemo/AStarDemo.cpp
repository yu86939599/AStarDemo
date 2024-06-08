#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

//行数  Y轴  竖着
#define ROWS   10
//列数  X轴  横着
#define COLS   10

//直线代价
#define ZXDJ  10
//斜线代价
#define XXDJ  14

//方向枚举
enum direct { p_up, p_down, p_left, p_right, p_lup, p_ldown, p_rup, p_rdown };


struct MyPoint {
	int row, col; //数组下标，表示在数组（地图)中的位置
	int f, g, h; //量化评估，f最终权重，g当前已经使用的权重，h当前点到终点的直线距离
};

//树的节点类型
struct treeNode {
	MyPoint				pos;		//当前节点数据
	vector<treeNode*>	childs;		//存放孩子节点指针的 容器(数组)
	treeNode*			pParent;	//指向父节点
};

//创建树节点的函数
treeNode* createTreeNode(MyPoint pos);

//判断一个点能不能走
bool canWalk(int map[ROWS][COLS], bool pathMap[ROWS][COLS], MyPoint pos);

//计算H值
int getH(MyPoint pos, MyPoint endPos);


int main() {
	//1 墙     0 路
	int map[ROWS][COLS] = {
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
		{ 0, 1, 0, 1, 1, 0, 0, 0, 0, 0 },
		{ 0, 1, 1, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }
	};

	//起点 
	MyPoint begPos = { 1, 1 };//begPos.row begPos.col
	//终点
	MyPoint endPos = { 6, 7 };

	bool pathMap[ROWS][COLS] = { 0 };//0 false 没有走过 1 true 走过

	//起点标记为走过
	pathMap[begPos.row][begPos.col] = true;


	//树结构
	treeNode* pRoot = NULL;

	//起点成为树的根节点
	pRoot = createTreeNode(begPos);

	//数组
	vector<treeNode*> buff;
	vector<treeNode*>::iterator it; //实时变化
	vector<treeNode*>::iterator itMin; //记录f最小的
	//当前点，从起点开始
	treeNode* pCurrent = pRoot;
	//一个个找出来
	treeNode* pChild = NULL;

	bool isFindEnd = false;

	while (1) {
		//4.1 周围八个点 检索一遍
		for (int i = 0; i < 8; i++) {
			pChild = createTreeNode(pCurrent->pos);
			switch (i) {
			case p_up:
				pChild->pos.row--;
				pChild->pos.g += ZXDJ;
				break;
			case p_down:
				pChild->pos.row++;
				pChild->pos.g += ZXDJ;
				break;
			case p_left:
				pChild->pos.col--;
				pChild->pos.g += ZXDJ;
				break;
			case p_right:
				pChild->pos.col++;
				pChild->pos.g += ZXDJ;
				break;
			case p_lup:
				pChild->pos.col--;
				pChild->pos.row--;
				pChild->pos.g += XXDJ;
				break;
			case p_ldown:
				pChild->pos.col--;
				pChild->pos.row++;
				pChild->pos.g += XXDJ;
				break;
			case p_rup:
				pChild->pos.col++;
				pChild->pos.row--;
				pChild->pos.g += XXDJ;
				break;
			case p_rdown:
				pChild->pos.col++;
				pChild->pos.row++;
				pChild->pos.g += XXDJ;
				break;
			}//end of switch (i){
			if (canWalk(map, pathMap, pChild->pos)) {//能走的 
				// 计算出来h f值
				pChild->pos.h = getH(pChild->pos, endPos);

				pChild->pos.f = pChild->pos.g + pChild->pos.h;
				//入树
				pCurrent->childs.push_back(pChild);//认子 ，新节点成为老节点的孩子，指针下移指向新创建的孩子节点
				pChild->pParent = pCurrent;//认父，老姐点成为新街店的父，新创建的孩子节点的父指针指向根节点（第一次时）
				//入数组 ，放到数组里
				buff.push_back(pChild);
			}
			else {//不能走
				//释放
				//delete pChild;
			}
		}//end of for (int i = 0; i < 8; i++){
		//4.2 走 从数组里删掉
		//找出f最小的
		itMin = buff.begin();//假设第一个最小
		for (it = buff.begin(); it != buff.end(); it++) {
			itMin = ((*itMin)->pos.f < (*it)->pos.f) ? itMin : it;
		}
		//走
		pCurrent = *itMin;
		//标记走过
		pathMap[pCurrent->pos.row][pCurrent->pos.col] = true;
		//从数组里删掉，删掉走过的点
		buff.erase(itMin);
		//4.3 开始下一次循环   直到遇到终点为止，遇到终点或者整个地图都没走到终点 循环结束
		if (pCurrent->pos.row == endPos.row &&
			pCurrent->pos.col == endPos.col) {
			isFindEnd = true;
			break;
		}

		//地图找遍了还没找到终点
		if (buff.size() == 0) break;

	}//end of while(1){

	//找到终点了
	if (isFindEnd) {
		printf("找到终点了!\n");
		printf("path:");
		while (pCurrent) {
			printf("(%d,%d)", pCurrent->pos.row,
				pCurrent->pos.col);

			pCurrent = pCurrent->pParent;
		}
		printf("\n");
	}


	while (1);
	return 0;
}

//计算H值
int getH(MyPoint pos, MyPoint endPos) {
	int x = (endPos.col > pos.col) ? (endPos.col - pos.col) :
		(pos.col - endPos.col);
	int y = (endPos.row > pos.row) ? (endPos.row - pos.row) :
		(pos.row - endPos.row);

	return (x + y)*ZXDJ;
}

//判断一个点能不能走
bool canWalk(int map[ROWS][COLS], bool pathMap[ROWS][COLS], MyPoint pos) {
	//1 是否在地图范围内
	if (pos.row < 0 || pos.row >= ROWS ||
		pos.col < 0 || pos.col >= COLS) return false;
	//2 是否走过
	if (pathMap[pos.row][pos.col]) return false;
	//3 是否障碍
	if (map[pos.row][pos.col]) return false;

	return true;
}
//创建树节点的函数
treeNode* createTreeNode(MyPoint pos) {
	treeNode* pNew = new treeNode;
	memset(pNew, 0, sizeof(treeNode));//全部赋值为0

	pNew->pos = pos;

	return pNew;
}