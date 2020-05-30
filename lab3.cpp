#include "common.h"

using namespace std;

//You should only code here.Don't edit any other files in this 
int func1(int amount, vector<int>& coins)
{	
	vector<int> dp;
	dp.resize(amount + 1);
	dp[0] = 1;
	for (int i = 1; i <= amount; i++)
		dp[i] = 0;

	for (int i = 1; i < coins.size(); i++) {
		for (int j = 0; j <= amount; j++) {
			int tmp = j - coins[i - 1];
			if (tmp >= 0) {
				//状态转移方程
				dp[j] = dp[j] + dp[tmp];
			}
		}
	}

	return dp[amount];

}

int func2(int amount, vector< vector<int> >& conquer)
{
	//dp[i][j]代表i, j是否可以遇到决斗
	vector< vector<int> > dp;
	//enemy为决斗对象
	int enemy;
	int num = 0;
	//初始化，每个人都可以和身边的人决斗
	for (int i = 0; i < amount; i++)
		dp[i][(i + 1) % amount] = 1;
	
	//每相邻i个人的两人能否发生决斗
	for (int i = 1; i < amount; i++) {
		for (int self = 0; self < amount; self++) {
			int enemy;
			for (enemy = (self + 1) % amount; enemy != (self + i + 1) % amount; enemy = (enemy + 1) % amount) {
				dp[self][(i+1 + self) % amount] = dp[self][(i+1 + self) % amount] || (dp[self][enemy] &&
					dp[enemy][(i+1 + self) % amount] && (conquer[self][enemy] || conquer[(i+1 + self) % amount][enemy]));
			}
		}
	}
	//能遇到自己即可以吃鸡
	for (int i = 0; i < amount; i++) {
		if (dp[i][i] == 1)
			num++;
	}

	return num;
}


double func3(int n,int hp,vector<int>& damage,vector<int>& edges) {
    //概率矩阵
	vector< vector<double> > dp;
	//记录节点相连的路径
	vector< vector<int> > path;
	//方程组常数项
	vector<double> b;
	//方程组系数矩阵
	vector< vector<double> > a;
	//解集
	vector<double> x;
	//damage为0的节点与方程组中未知数位置的映射
	vector<int> map;
	//反映射
	vector<int> map1;

	//初始化
	dp.resize(hp);
	for (int i = 0; i < hp; i++) {
		dp[i].resize(n);
	}
	path.resize(n);
	map1.resize(n);

	//读入edges
	for (int i = 0; i < edges.size(); i += 2) {
		path[edges[i+1] - 1].push_back(edges[i]);
		path[edges[i] - 1].push_back(edges[i + 1]);
	}

	//map初始化
	for (int i = 0; i < n; i++) {
		if (damage[i] == 0) {
			map1[i] = map.size();
			map.push_back(i);
		}
	}

	//根据hp开始大循环，hp逐一减少，填写这一hp层的dp矩阵
	for (int tmpHp = hp; tmpHp != 0; tmpHp--) {
		for (int i = 0; i < n; i++) {
			if (damage[i] != 0 && tmpHp + damage[i] <= hp) {
				for (int j = 0; j < path[i].size(); j++) {
					//状态转移方程
					if (path[i][j] != n)
						dp[tmpHp][i] += dp[tmpHp + damage[i]][path[i][j] - 1] / path[path[i][j] - 1].size();
				}
				
			}
		}
		//a, b, x初始化
		a.resize(map.size());
		for (int i = 0; i < map.size(); i++) {
			a[i].resize(map.size());
		}
		b.resize(map.size());
		x.resize(map.size());
		//得到a, b的值
		for (int i = 0; i < map.size(); i++) {
			a[i][i] = 1;
			if (i == 0 && tmpHp == hp)
				b[i] = 1;
			for (int j = 0; j < path[map[i]].size(); j++) {
				//damage不为0的加到常数项
				if (damage[path[map[i]][j] - 1] != 0) {
					b[i] += (double)dp[tmpHp][path[map[i]][j] - 1] / path[path[map[i]][j] - 1].size();
				}
				//damage为0加到系数矩阵
				else {
					if (path[map[i]][j] != n)
						a[i][map1[path[map[i]][j] - 1]] = -(double)1 / path[path[map[i]][j] - 1].size();
				}
			}
		}
		
		//gauss消元
		for (int i = 0; i < map.size(); i++) {
			int ind = i;
			for (int j = i + 1; j < map.size(); j++) {
				if (abs(a[j][i]) > abs(a[ind][i])) {
					ind = i;
				}
			}
			if (abs(a[ind][i]) < eps) {
				return 0;
			}
			if (ind != i) {
				swap(b[i], b[ind]);
				for (int j = 1; j < map.size(); j++) {
					swap(a[i][j], a[ind][j]);
				}
			}
			for (int j = i + 1; j < map.size(); j++) {
				double d = a[j][i] / a[i][i];
				b[j] -= b[i] * d;
				for (int k = i; k < map.size(); k++) {
					a[j][k] -= a[i][k] * d;
				}
			}
		}
		//计算x[i]的值
		for (int i = map.size() - 1; i >= 0; i--) {
			for (int j = i + 1; j < map.size(); j++) {
				b[i] -= x[j] * a[i][j];
			}
			x[i] = b[i] / a[i][i];
		}
		//赋值给这一hp层的dp概率
		for (int i = 0; i < map.size(); i++) {
			dp[tmpHp][map[i]] = x[i];
		}
		a.clear();
		b.clear();
		x.clear();
	}

	double p = 0;
	//累加得总概率
	for (int i = 0; i < hp+1; i++) {
		p += dp[i][n - 1];
	}
    return p;
}