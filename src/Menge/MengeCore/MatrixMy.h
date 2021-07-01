#pragma once
#ifndef __MATRIX_MY_H__
#define __MATRIX_MY_H__

#include <iostream>
#include <stdlib.h>

namespace Menge{
	
	class MatrixDim2 {
	private:
		int rows_num, cols_num;
		float** p;
		void initialize();//��ʼ������

	public:
		MatrixDim2(int, int, float);//��ά��ʼ��������ȫΪĳ��ֵ
		virtual ~MatrixDim2();//��������Ӧ�����麯�������Ǵ��಻��������
		void Show() const;//������ʾ2ά
		float Point(int i, int j) const;//��ʾ���ֵ
		void SetPoint(int i, int j, float value) ;//���õ��ֵ
		int row_size() const;//��������
		int col_size() const;//��������

		void Aggregate(int typeAgent, int shopID, int strength);
	};


	class MatrixDim3 {
	private:
		int dimension_a, dimension_b, dimension_c;
		float*** p;
		void initialize();//��ʼ������
	public:
		MatrixDim3(int, int, int, float);//��ά��ʼ��������ȫΪĳ��ֵ
		virtual ~MatrixDim3();//��������Ӧ�����麯�������Ǵ��಻��������
		void Show() const;//������ʾ3ά
		float Point(int i, int j, int k) const;
		void SetPoint(int i, int j, int k, float value) ;
		int dimensionA_size() const;//���ص�һά�ߴ�
		int dimensionB_size() const;//���صڶ�ά�ߴ�
		int dimensionC_size() const;//���ص���ά�ߴ�

		void Aggregate(int typeAgent, int shopID, int strength) ;
	};

	
	class MatrixDim4 {
	private:
		int dimension_a, dimension_b, dimension_c, dimension_d;
		float**** p;
		void initialize();//��ʼ������
		
	public:
		MatrixDim2* _sumWeightMatrix;
		MatrixDim4(int, int, int, int, float) ;//��ά��ʼ��������ȫΪĳ��ֵ
		virtual ~MatrixDim4();//��������Ӧ�����麯�������Ǵ��಻��������
		void Show() const;//������ʾ
		float Point(int i, int j, int k, int l) const;
		void SetPoint(int i, int j, int k, int l,float value) ;
		void dimension_size();//���سߴ�


		void Aggregate(int typeAgent, int shopID, int strength) ;

	};
}
#endif