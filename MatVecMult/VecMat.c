#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<time.h>

int* get_vector(int n)
{
	int *a;
	a = (int*)malloc(n * sizeof(int));

	for(int i=0; i<n; i++)
		a[i] = rand()%10;

	return a;
}

int** get_matrix(int row, int col)
{
	int **mat = (int**)malloc(row * sizeof(int*));

	for(int i = 0; i<row; i++)
		mat[i] = (int*)malloc(col * sizeof(int));

	for(int i = 0; i < row; i++)
		for(int j = 0; j < col; j++)
			mat[i][j] = rand() % 10;

	return mat;
}

void disp_vec(int *a,int *b, int n)	//for displaying generated vectors
{
	printf("\n Elements of vector A:\n");
	for(int i=0;i<n;i++)
	{
		printf("\t %d",a[i]);
	}
	printf("\n Elements of vector B:\n");
	for(int i=0;i<n;i++)
	{
		printf("\t %d",b[i]);
	}
}//end

int* add_vec(int *a,int *b, int n)		// for displaying addn of generated vectors
{
	int *c = (int*)malloc(n * sizeof(int));
	printf("\n Addition of vector elements:\n");
	#pragma omp parallel for
	for(int i=0;i<n;i++)
		{
			c[i]=a[i]+b[i];
		}
	for(int i=0; i<n; i++)
		printf("\t %d", c[i]);
	return c;
}//end

void dispmat(int **a,int **b,int r1,int c1,int r2,int c2)	//displaying matrix1 and matrix2
{
	printf("\n Matrix A:\n");
	for(int i=0;i<r1;i++)
	{
		for(int j=0;j<c1;j++)
			printf("\t %d",a[i][j]);
		printf("\n");
	}
	printf("\n Matrix B:\n");
	for(int i=0;i<r2;i++)
	{
		for(int j=0;j<c2;j++)
			printf("\t %d",b[i][j]);
		printf("\n");
	}
}//end

void matrixmul(int **a,int **b,int **c,int r1,int c1,int c2)	//matrix multiplication
{

	#pragma omp parallel for
	for(int i=0;i<r1;i++)
	{
		#pragma omp parallel for
		for(int j=0;j<c2;j++)
		{
			c[i][j]=0;
			for(int k=0;k<c1;k++)
			{
				c[i][j]=c[i][j]+a[i][k]*b[k][j];
			}
		}
	}
}//end

void dispmatvec(int **a,int *b,int n,int r,int c)	//displaying original matrix and vector
{
	printf("\n Vector:\n");
	for(int i=0;i<n;i++)
	{
		printf("\t %d",b[i]);
	}

	printf("\n Matrix:\n");
	for(int i=0;i<r;i++)
	{
		for(int j=0;j<c;j++)
		{
			printf("\t %d",a[i][j]);
		}
		printf("\n");
	}
}//end

void matvecmul(int **a,int *b,int *vec,int r,int c,int n)	//matrix-vector multiplication
{
	#pragma omp parallel for
	for(int i=0;i<r;i++)
	{
		vec[i]=0;
		for(int j=0;j<c;j++)
		{
			vec[i]+=a[i][j]*b[j];
		}
	}

}//end

int main()
{
	int no_of_ele,*vec1,*vec2,*resvec,ch,mat,**mat1,**mat2,**resmat,row1,col1,row2,col2,row,col;
	clock_t start=clock();


	do
	{
		printf("\n---------------OPTIONS----------------");
		printf("\n1.Vector-Vector addition\n2.Matrix-Matrix Mul\n3.Matrix-Vector Mul\n4.Exit");
		printf("\n Enter your choice:");
		scanf("%d",&ch);

		switch(ch)
		{
			case 1: //vector-vector addn
				label:printf("\n Enter no. of elements:");
				      scanf("%d",&no_of_ele);
							printf("%d\n", no_of_ele);
				if(no_of_ele>100)
				{
					printf("\n Vector Size exceed");
					goto label;

				}
				else
				{
					vec1 = get_vector(no_of_ele);
					vec2 = get_vector(no_of_ele);
					disp_vec(vec1,vec2, no_of_ele);
					resvec = add_vec(vec1,vec2, no_of_ele);
				}
			break;

			case 2://matrix-matrix mul
				Errorslv:printf("\n Enter rows of first matrix:");
					scanf("%d",&row1);
					printf("\n Enter columns of first matrix:");
					scanf("%d",&col1);
					printf("\n Enter rows of second matrix:");
					scanf("%d",&row2);
					printf("\n Enter columns of second matrix:");
					scanf("%d",&col2);
				if(col1!=row2)
				{
					printf("\n Error: No. of columns of matrix1 should be equal to no. of rows of matrix2");
					goto Errorslv;
				}
				else
				{
					mat1 = get_matrix(row1, col1);
					mat2 = get_matrix(row2, col2);
					resmat = (int**)malloc(row1 * sizeof(int*));
					for(int i = 0; i<row1; i++)
						resmat[i] = (int*)malloc(col2 * sizeof(int));
					dispmat(mat1,mat2,row1,col1,row2,col2);
					printf("\n Resultant matrix:\n");
					matrixmul(mat1,mat2,resmat,row1,col1,col2);
					for(int i=0;i<row1;i++)
					{
						for(int j=0;j<col2;j++)
						{
							printf("\t %d",resmat[i][j]);
						}
						printf("\n");
					}
				}
			break;

			case 3://matrix-vector mul
				label1:printf("\n Enter no. of elements:");
					scanf("%d",&no_of_ele);
					printf("\n Enter no. of rows:");
					scanf("%d",&row);
					printf("\n Enter no. of columns:");
					scanf("%d",&col);
				if(no_of_ele!=col)
				{
					printf("\n Error");
					goto label1;
				}
				else
				{
					vec1 = get_vector(no_of_ele);
					mat1 = get_matrix(row, col);
					dispmatvec(mat1,vec1,no_of_ele,row,col);
					printf("\n Multiplication:\n");
					matvecmul(mat1,vec1,resvec,row,col,no_of_ele);
					for(int i=0;i<no_of_ele;i++)
					{
						 printf("\t %d",resvec[i]);
					}
					printf("\n");
				}
				break;

			case 4://exit
				break;

		}
	}while(ch!=4);
	clock_t stop=clock();
	double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
	printf("Time elapsed in ms: %f", elapsed);
return 0;
}
