#include "MatrixMy.h"

namespace Menge {

	using namespace std;

	/////////////////////////////////////////////////////////////////////
	//					Implementation of MatrixDim2
	/////////////////////////////////////////////////////////////////////
	void MatrixDim2::initialize() {//初始化矩阵大小
		p = new float* [rows_num];//分配rows_num个指针
		for (int i = 0; i < rows_num; ++i) {
			p[i] = new float[cols_num];//为p[i]进行动态内存分配，大小为cols
		}
	}

	//声明一个值全部为value的矩阵
	MatrixDim2::MatrixDim2(int rows, int cols, float value)
	{
		rows_num = rows;
		cols_num = cols;
		initialize();
		for (int i = 0; i < rows_num; i++) {
			for (int j = 0; j < cols_num; j++) {
				p[i][j] = value;
			}
		}
	}

	//析构函数
	MatrixDim2::~MatrixDim2() {
		for (int i = 0; i < rows_num; ++i) {
			delete[] p[i];
		}
		delete[] p;
	}

	//矩阵显示
	void MatrixDim2::Show() const {
		//cout << rows_num <<" "<<cols_num<< endl;//显示矩阵的行数和列数
		for (int i = 0; i < rows_num; i++) {
			for (int j = 0; j < cols_num; j++) {
				cout << p[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
	

	//返回矩阵第i行第j列的数
	float MatrixDim2::Point(int i, int j) const {
		return this->p[i][j];
	}

	//设置矩阵第i行第j列的值
	void MatrixDim2::SetPoint(int i, int j, float value) {
		p[i][j] = value;
	}

	//读取矩阵行列数
	int MatrixDim2::row_size() const {
		return rows_num;
	}
	int MatrixDim2::col_size() const {
		return cols_num;
	}

	/////////////////////////////////////////////////////////////////////
	//					Implementation of MatrixDim3
	/////////////////////////////////////////////////////////////////////
	void MatrixDim3::initialize() {//初始化矩阵大小
		p = new float** [dimension_a];//分配dimension_a个指针
		for (int i = 0; i < dimension_a; ++i) {
			p[i] = new float* [dimension_b];
			for (int j = 0; j < dimension_b; ++j) {
				p[i][j] = new float[dimension_c];//为p[i][j]进行动态内存分配，大小为dimension_c
			}
		}
	}


	//声明一个值全部为value的矩阵
	MatrixDim3::MatrixDim3(int a, int b, int c, float value)
	{
		dimension_a = a;
		dimension_b = b;
		dimension_c = c;
		initialize();
		for (int i = 0; i < dimension_a; i++) {
			for (int j = 0; j < dimension_b; j++) {
				for (int k = 0; k < dimension_c; k++) {
					p[i][j][k] = value;
				}
			}
		}
	}

	//析构函数
	MatrixDim3::~MatrixDim3() {
		for (int i = 0; i < dimension_a; ++i) {
			for (int j = 0; j < dimension_b; j++) {
				delete[] p[i][j];
			}
			delete[] p[i];
		}
		delete[] p;
	}

	//矩阵显示
	void MatrixDim3::Show() const {
		//cout << dimension_a <<" "<< dimension_b <<" "<< dimension_c << endl;
		for (int i = 0; i < dimension_a; i++) {
			for (int j = 0; j < dimension_b; j++) {
				for (int k = 0; k < dimension_c; k++) {
					cout << p[i][j][k] << " ";
				}
			}
			cout << " " << endl;
		}
		cout <<" "<< endl;
	}

	//返回矩阵第i行第j列的数
	float MatrixDim3::Point(int i, int j, int k) const{
		//cout << "point: "<<p[i][j][k] << endl;
		return this->p[i][j][k];
	}

	//设置矩阵i,j,k的值
	void MatrixDim3::SetPoint(int i, int j,int k, float value) {
		p[i][j][k] = value;
	}

	//读取矩阵三维尺寸
	int MatrixDim3::dimensionA_size() const {
		return dimension_a;
	}
	int MatrixDim3::dimensionB_size() const {
		return dimension_b;
	}

	int MatrixDim3::dimensionC_size() const {
		return dimension_c;
	}

	//typeAgent:0 1 2   shopID:0~29   strength:0 1 2 3
	void MatrixDim3::Aggregate(int typeAgent, int shopID, int strength) {
		float temp = this->Point(typeAgent, shopID / 10, shopID % 10);
		for (int k = 0; k < this->dimensionC_size(); k++) { //shopID
			if (typeAgent == shopID / 10) {
				//给游客发倾向类型的优惠券，例如给吃喝型游客发吃喝型的店铺优惠券
				//strength = 1: choose_shop = 0.163, not_choose_shop = 0.093
				//strength = 2: choose_shop = 0.226, not_choose_shop = 0.086
				//strength = 3: choose_shop = 0.289, not_choose_shop = 0.079
				if (k == shopID % 10) this->SetPoint(typeAgent, shopID / 10, k, temp + temp * 0.07 * strength * 9);
				else this->SetPoint(typeAgent, shopID / 10, k, this->Point(typeAgent, shopID / 10, k) * (1 - 0.07 * strength));
			}

			else {
				//给游客发非倾向类型的优惠券，例如给吃喝型游客发吃喝型的店铺优惠券
				//strenth = 1: choose_shop = 0.145, not_choose_shop = 0.095
				//strenth = 2: choose_shop = 0.19, not_choose_shop = 0.09
				//strenth = 3: choose_shop = 0.235, not_choose_shop = 0.085
				if (k == shopID % 10) this->SetPoint(typeAgent, shopID / 10, k, temp + temp * 0.05 * strength * 9);
				else this->SetPoint(typeAgent, shopID / 10, k, this->Point(typeAgent, shopID / 10, k) * (1 - 0.05 * strength));

			}
		}
	}


	/////////////////////////////////////////////////////////////////////
	//					Implementation of MatrixDim4
	/////////////////////////////////////////////////////////////////////
	void MatrixDim4::initialize() {//初始化矩阵大小
		p = new float*** [dimension_a];//分配dimension_a个指针
		for (int i = 0; i < dimension_a; ++i) {
			p[i] = new float** [dimension_b];
			for (int j = 0; j < dimension_b; ++j) {
				p[i][j] = new float* [dimension_c];
				for (int k = 0; k < dimension_c; ++k) {
					p[i][j][k] = new float [dimension_d];//为p[i][j]进行动态内存分配，大小为dimension_d
				}
			}
		}
	}

	

	//声明一个值全部为value的矩阵
	MatrixDim4::MatrixDim4(int a, int b, int c, int d, float value) 
	{
		dimension_a = a;
		dimension_b = b;
		dimension_c = c;
		dimension_d = d;
		initialize();
		for (int i = 0; i < dimension_a; i++) {
			for (int j = 0; j < dimension_b; j++) {
				for (int k = 0; k < dimension_c; k++) {
					for (int l = 0; l < dimension_d; l++) {
						p[i][j][k][l] = value;
					}
				}
			}
		}
		_sumWeightMatrix = new MatrixDim2(3,30,0);
		//_sumWeightMatrix = matrix;
		
		for (int i = 0; i < dimension_a; i++) {
			for (int j = 0; j < dimension_b; j++) {
				for (int k = 0; k < dimension_c; k++) {
					for (int l = 0; l < dimension_d; l++) {
						_sumWeightMatrix->SetPoint(i, j, _sumWeightMatrix->Point(i, j)+p[i][j][k][l]);
					}			
				}
			}
		}

	}


	//析构函数
	MatrixDim4::~MatrixDim4() {
		for (int i = 0; i < dimension_a; ++i) {
			for (int j = 0; j < dimension_b; j++) {
				delete[] p[i][j];
				for (int k = 0; k < dimension_c; k++) {
					delete[] p[i][j][k];
				}
			}
			delete[] p[i];
		}
		delete[] p;
	}

	//矩阵显示
	void MatrixDim4::Show() const {
		//cout << rows_num <<" "<<cols_num<< endl;//显示矩阵的行数和列数
		for (int i = 0; i < dimension_a; i++) {
			for (int j = 0; j < dimension_b; j++) {
				for (int k = 0; k < dimension_c; k++) {
					for (int l = 0; l < dimension_d; l++) {
						cout << p[i][j][k][l] << " ";
					}
				}
				cout << endl;
			}
			cout << endl;
		}
		cout << endl;
	}

	//返回矩阵i j k l的数
	float MatrixDim4::Point(int i, int j, int k,int l ) const {
		return this->p[i][j][k][l];
	}

	//设置矩阵i,j,k,l的值
	void MatrixDim4::SetPoint(int i, int j, int k, int l ,float value){
		p[i][j][k][l] = value;
	}

	//读取矩阵尺寸
	void MatrixDim4::dimension_size() {
		cout << "dimension_a: " << this->dimension_a << endl;
		cout << "dimension_b: " << this->dimension_b << endl;
		cout << "dimension_c: " << this->dimension_c << endl;
		cout << "dimension_d: " << this->dimension_d << endl;
	}
	

	//聚集策略
	void MatrixDim4::Aggregate(int typeAgent, int shopID, int strength){
		//发优惠券只会影响相同类型的店铺，不会影响不同类型的店铺
		
		for (int i = 0; i < dimension_b; i++) {  //对于每一家起点店铺
			float temp = this->Point(typeAgent, i, shopID / 10, shopID % 10);//记录选中的那家的权重

			for (int j = 0; j < dimension_d; j++) {//改变同类型店铺的概率
				if (typeAgent == shopID / 10) {     //给agent发放他倾向的优惠券
					if (j == shopID % 10) this->SetPoint(typeAgent, i,shopID / 10, j, temp*(1+ 0.07 * strength * 9));//参数需要修改
					else this->SetPoint(typeAgent, i,shopID / 10, j, this->Point(typeAgent, i,shopID / 10, j) * (1 - 0.07 * strength));
				}
					
				else {  //发放不倾向的优惠券
					if (j == shopID % 10) this->SetPoint(typeAgent, i, shopID / 10, j, temp * (1 + 0.05 * strength * 9));//参数需要修改
					else this->SetPoint(typeAgent, i, shopID / 10, j, this->Point(typeAgent, i, shopID / 10, j) * (1 - 0.05 * strength));
				}
			}
		}
		
	}

}