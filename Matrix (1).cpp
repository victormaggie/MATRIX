#include "Matrix.h"
#include "general.h"
#include <math.h>
#include "Vector.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
//------------------------------ Implemenation  -----------------------------


Matrix::Matrix() //default constructor
{
	rows=columns=0;
	buf=NULL;
}


Matrix::Matrix( int m,  int n)//declare an mxn matrix
{
	rows = m;
	columns = n;
	buf = new double[m*n];
}


Matrix::Matrix(const Matrix& A) //copy constructor
{
	if (!this->buf)	delete[]this->buf;   // delete
	this->rows = A.rows;
	this->columns = A.columns;          // copy rows and columns using this pointer
	this->buf = new double[A.rows*A.columns];  // allocate the hard dirve
	for (int i = 0; i < A.rows*A.columns; i++)
		this->buf[i] = A.buf[i];
}


Matrix::~Matrix()//destructor
{
	this->rows = 0;
	this->columns = 0;
	delete[] this->buf;    // deconstructor clean up.
}
//Assignment


Matrix& Matrix::operator = (const Matrix& A) //overloading =
{
	if (this == &A)
	return *this;
	if (this->buf)
	delete[] buf;
	this->columns = A.columns;
	this->rows = A.rows;
	this->buf = new double[A.columns*A.rows];
	int i;
	for (i=0; i< (A.columns*A.rows); i++)
	this->buf[i] = A.buf[i];
	return *this;
}


//operators
bool Matrix::operator == (const Matrix& A)//overloading ==
{
	if (!this->buf && !A.buf)    // two empty matrix
		return true;
	if (this->columns != A.columns || this->rows != A.rows)
		return false;
		int i;
		for (i = 0; i < columns*rows; i++)
			{
			if (fabs(this->buf[i]-A.buf[i])>1e-10)
			return false;
			}
	return true;
}


bool Matrix::operator != (const Matrix& A)//overloading !=
{
	return !(*this==A); //use ==
}


Matrix& Matrix::operator += (const Matrix& A) //overloading +=
{
	if (!A.buf) return *this;
	if ((this->rows != A.rows) || (this->columns != A.columns))
		throw logic_error("size mismatch in dimension");
	for (int i = 0; i < A.columns*A.rows; i++)
		this->buf[i] += A.buf[i];
	return *this;
}



Matrix& Matrix::operator -=(const Matrix& A) //overloading -=
{
	if (!A.buf)
	return *this;
	if ((this->rows != A.rows) || (this->columns != A.columns))
		throw logic_error("size mismatch in dimension");
	for (int i = 0; i < A.columns*A.rows; i++)
		this->buf[i] -= A.buf[i];
	return *this;
}


Matrix& Matrix::operator *=(const Matrix& A) //overloading *=
{
	if (!A.buf)
		throw logic_error("You are Multipling Empty Matrix");
	if (this->columns != A.rows)
		throw logic_error("You are Multipling Empty Matrix");
	if (A.columns == 0 || A.rows == 0 || this->columns == 0 || this->rows == 0)
		throw logic_error("the matrix is wrong!");
	else
	{
	Matrix tmp;
	tmp.rows = this->GetRows();
	tmp.columns = this->GetColumns();
	tmp.buf = new double [this->rows*A.columns];
	for (int i = 0; i < (this->rows); i++)
	{
		for (int j = 0; j < (this->columns); j++)
		{	double temp = 0;
			for (int k = 0; k < this->columns; k++)
			{
			temp = temp + buf[i*columns+k]*A.buf[k*A.columns+j];
			tmp.buf[i*A.columns+j] = temp;
			cout << " "<< tmp.buf[i*A.columns+j];
			}
		cout << endl;
		}
	}
		*this = tmp;
		return *this;
	}
}



Matrix& Matrix::operator *=(double a) //overloading *=
{
	if (!this->buf)
		throw logic_error("Please Check your empty matrix first");
	for (int i = 0; i <(this->columns*this->rows); i++)
	this -> buf[i] *= a;
	return *this;
}



Matrix& Matrix::operator *=(const Vector& b) //overloading *=
{
	if (!this->buf)
		throw logic_error("Empty matrix man");
	if (this->columns == b.Size())
	{
		int c = this->columns;
		int r = this->rows;
		Matrix tmp (r, 1);
		for (int i = 0; i < r; i++)
			for (int j = 0; j < c; j++)
		{
			tmp.buf[i]= tmp.buf[i]+this->buf[i*c+j]*b[j];
		}
	*this = tmp;
	}
	else
		cout<<"Dimension mismatch"<<endl;
	return *this;
}


Matrix Matrix::operator + () //unary +
{
	return *this; //good enough.
}


Matrix Matrix::operator - () //unary -
{
	Matrix tmp(*this);
	for (int i = 0; i < (this->rows*this->columns);i++)
	tmp.buf[i]*=(-1);
	return tmp;
}


double& Matrix::operator ()( int i,  int j)// access (i,j)
{
	if(i > rows||j > columns||i < 0||j < 0)
	{
		cout<<"Out of range"<<endl;
		return buf[0];
	}
	else
	return buf[i*columns+j]; // is this correct? Unsafe
}

double& Matrix::operator()( int i,  int j) const //read only
{
        if(i > rows||j > columns||i < 0||j < 0)
        {
                cout<<"Out of range"<<endl;
                return buf[0];
        }
        else
	return buf[i*columns+j]; // is this correct? Unsafe
}


ostream& operator << (ostream& output, const Matrix& A)
{
	//should we output the dimension first?
	cout<<"rows="<<A.GetRows()<<endl;
	cout<<"columns="<<A.GetColumns()<<endl;
	for ( int i = 0; i < A.GetRows(); i++)
	{
		for ( int j = 0; j < A.GetColumns(); j++)
			output << A(i,j) << "\t   ";
		output << endl;
	}
	return output;
}

istream& operator >> (istream& input, Matrix& A)
{
	//use BypassComment to skip comments.
	cout<<"Reading"<<endl;
	BypassComment(input);
	input>>A.rows;
	BypassComment(input);
	input>>A.columns;
        if (A.buf) delete[] A.buf;
	A.buf=new double[A.rows*A.columns];
        BypassComment(input);
	for(int i=0;i<A.rows*A.columns;i++)
	{
		input>>A.buf[i];
	        BypassComment(input);
	}
	return input;
}

//------------Member Functions------------------------------


Matrix Matrix::Adjugate() //Adjoint/Adjugate
{
	if(this->rows!=this->columns||this->rows==0||this->rows==1)
	{
		cout<<"Wrong size"<<endl;
		return *this;
	}
	Matrix tmp;
	tmp=this->Cofactor();
	tmp=tmp.Transpose();
	return tmp;
}



double Matrix::Cofactor(int i, int j) //cofactor Cij
{
        if(this->rows!=this->columns||this->rows==0||this->rows==1)
        {
                cout<<"Wrong matrix size"<<endl;
                return 0.0;
        }
	if(i>=this->rows||j>=this->columns||i<0||j<0)
        {
                cout<<"Wrong location"<<endl;
                return 0.0;
        }
	double tmp;
	tmp=pow(-1,(i+j))*this->Minor(i,j);
	return tmp;
}


Matrix Matrix::Cofactor()//matrix of cofactors
{
        if(this->rows!=this->columns||this->rows==0||this->rows==1)
        {
                cout<<"Wrong matrix size"<<endl;
                return *this;
        }
	Matrix tmp;
	tmp.rows=this->rows;
	tmp.columns=this->columns;
	tmp.buf=new double[this->rows*this->columns];
	for(int i=0;i< this->rows;i++)
	{
		for(int j=0;j< this->columns;j++)
		{
			tmp.buf[i*this->columns+j]=this->Cofactor(i,j);
		}
	}
	return tmp;
}



double Matrix::Minor(int i, int j)//Minor Mij
{
        if(this->rows!=this->columns||this->rows==0||this->rows==1)
        {
                cout<<"Wrong matrix size"<<endl;
                return 0.0;
        }
        if(i>=this->rows||j>=this->columns||i<0||j<0)
        {
                cout<<"Wrong location"<<endl;
                return 0.0;
        }
	double tmp;
	Matrix m;
	m.rows=this->rows-1;
	m.columns=this->columns-1;
	m.buf=new double[m.rows*m.columns];
	for(int a=0;a<m.rows;a++)
	{
		for(int b=0;b<m.columns;b++)
		{
			if(a<i&&b<j) m.buf[a*m.columns+b]=this->buf[a*this->columns+b];
			if(a<i&& b>=j) m.buf[a*m.columns+b]=this->buf[a*this->columns+(b+1)];
			if(a>=i&&b<j) m.buf[a*m.columns+b]=this->buf[(a+1)*this->columns+b];
			if(a>=i&&b>=j) m.buf[a*m.columns+b]=this->buf[(a+1)*this->columns+(b+1)];
		}
	}
	cout<<m<<endl;
	tmp=m.det();
	return tmp;
}



bool Matrix::IsSingular()
{
	return (fabs(this->det()-0)<1e-10);  //may not work, because of double precision. you fix it.
}



bool Matrix::IsSymmetric()
{
	return ((*this)==(this->Transpose()));
}



const int Matrix::GetRows() const
{
	return rows;
};

const int Matrix::GetColumns() const
{
	return columns;
};


Matrix Matrix::Transpose()  //transpose
{
	Matrix tmp;
	tmp.rows=this->columns;
	tmp.columns=this->rows;
	tmp.buf=new double[tmp.rows*tmp.columns];
	for(int i=0;i<tmp.rows;i++)
	{
		for(int j=0;j<tmp.columns;j++)
		tmp.buf[i*columns+j]=this->buf[j*rows+i];
	}
	return tmp;
}


Matrix Matrix::Inverse()//Inverse Matrix
{
	Matrix tmp;
	if (this->GetRows() != this->GetColumns())
		throw logic_error("Matrix should be match");
	if ((fabs(this->det())- 0) < 0.00000001)
		throw logic_error("determinant equal to zero, can not inverse");;
	tmp = this->Adjugate();
	tmp  *= (1 /this->det());
	return tmp;
}


Matrix Matrix::Null(int n) //make a "zero" Matrix, with a new dimension, change "this"
{
	if (n <= 0)
	{
		throw logic_error("can not do Null");
	}
	this->rows = n;
	this->columns = n;
	this->buf = new double[n*n];
	for (int a = 0; a < n; a++)
	{
		for (int b = 0; b < n; b++)
			this->buf[a*this->columns + b] = 0;
	}
	return *this;
}


Matrix Matrix::Null()//make a "zero" Matrix, does not change the dimension, change "this"
{
	if (this->GetRows() == 0 || this->GetColumns() == 0)
		throw logic_error("WRONG!!!");
		for (int a = 0; a < this->GetRows(); a++)
		{
			for (int b = 0; b < this->GetColumns(); b++)
			{
				this->buf[a*this->GetColumns() + b] = 0;
			}
		}
	return *this;
}


Matrix Matrix::Identity( int n)//make a nxn identity matrix,change "this"
{
	if (n == 0 || n < 0)
		throw logic_error("Matix is not available");
	if (n > 0)
	{
		this->columns = n;
		this->rows = n;
		this->buf = new double [n*n];
		for (int a = 0; a < n; a++)
		{
			for (int b = 0; b < n; b++)
				{
					if (a == b)
					this->buf[a*n + b] = 1;
					else
					this->buf[a*n + b] = 0;
				}
		}
	}
	return *this;
}



Matrix Matrix::Identity()//make a identity matrix, does not change the dimentsion, change "this"
{
	if (this->GetColumns() != this->GetRows() || this->GetColumns() == 0)
		throw logic_error("Check your matrix, Matrix need to be equal");
	for (int a = 0; a < this->GetColumns(); a++)
	{
		for (int b = 0; b < this->GetColumns(); b++)
		{
			if (a == b)	this->buf[a*this->GetColumns() + b] = 1;
			else this->buf[a*this->GetColumns() + b] = 0;
		}
	}
	return *this;
}



bool Matrix::LU(Matrix& L, Matrix& U)//LU decomposition. return true if successful
{
Matrix tmp(*this);
L.rows = this->rows;
L.columns = this->columns;
U.rows = this->rows;
U.columns = this->columns;
L.buf = new double [rows*columns];
U.buf = new double [rows*columns];
for (int i = 0; i < L.rows; i++)
{
	for (int j =0; j < L.columns; j++)
	{
	if (i == j)
		L(i,j) = 1.0;
	else
		L(i,j) = 0.0;
	}
}

for (int col = 0; col < columns; col++)
{
	for (int row = (col+1); row < rows; row++)
	{
	if (tmp(col, col)!=0)
		L(row, col) = tmp(row, col)/tmp(col,col);
	for (int i = (col+1); i < columns; i++)
		tmp(row, i) =tmp(row, i)-L(row,col)*tmp(col,i);
	}
	for (int ind=col; ind<rows; ind++)
	U(col,ind) = tmp(col,ind);

}
cout << "L matrix is "<< endl<< L <<endl;
cout << "U matrix is "<< endl<< U <<endl;
return true;

}


bool Matrix::QR(Matrix& Q, Matrix& R)
{
	if (this->rows!=this->columns)
	{
		cout<<"Wrong size"<<endl;
		return false;
	}
	else
	{
		bool sing;   // sing =true this matrix is singular;
		double s, sum, sigma, t;
		double f, precision;
		precision=1e-15;
		R.Null(this->rows);
		Q.Null(this->rows);
		int n = this->rows;
		Matrix tmp(this->rows, this->columns);
		tmp = *this;
		Matrix d(1,n);
		int * Remove_Column= new int [tmp.columns];
		int c = 0;
		for (int j = 0; j < n; j++)
		{
		    for (int k = 0; k < j; k++)
		    {    s = 0.0;
			for (int i = 0; i < tmp.rows; i++)
			    s+= tmp (i,j) * tmp (i,k);
			for (int i = 0; i < tmp.rows; i++)
			    tmp(i,j)= tmp (i,j)- s* tmp(i,k);
		    }
		    sum = 0.0;
		    for (int i = 0; i< tmp.rows; i++)
			sum+= tmp(i,j) * tmp (i,j);
		    if (sum < precision)
		    {
			Remove_Column[c]=j;
			c++;
			for (int i=0; i< tmp.rows; i++)
			    tmp(i,j)=0.0;
			continue;
		    }
		    for (int i=0; i<tmp.rows; i++)
			tmp(i,j)= tmp(i,j)/sqrt(sum);
		}

		if (Q.buf) delete [] Q.buf;
		Q.rows = tmp.rows;
		Q.buf = new double [tmp.rows* (tmp.columns-c)];
		int st = 0;
		int col = 0;

		for (int j=0; j< tmp.columns; j++)
		{
			bool next=false;
			for (int k=st; k< c; k++)
			{
				if (j==c)
				{
					st++;
					next=true;
					break;
				}
			}
			if (next) continue;
			for (int i=0; i< tmp.rows; i++)
			Q(i,col)=tmp(i,j);
			col++;
		}
		R=Q.Transpose()* (*this);
	}
	return true; 
}


double Matrix::det()//determinant(Matrix)
{
	double tmp=0.0;
	if (buf == NULL)
	{
		cout<<"The Matrix does not contain any value"<<endl;
	}
	else
	{
	tmp = 1.0;
	Matrix L;
	Matrix U;
	this -> LU(L,U);
	for (int i=0; i<columns; i++)
		tmp *= U.buf[i*columns+i];
	}
	return tmp;
}



Vector Matrix::Eigenvalues()//find the eigen values and store them in a vector 
{
	Matrix tmp;
	Matrix R;
	Matrix Q;
	Matrix tmp1;
	Matrix tmp2;
	tmp1 = tmp2 = *this;
	double Standard=0.000000001;
	tmp1.QR(Q,R);
	tmp1=R*Q;
	bool judge = true;
	int m = 0;
	do {
	tmp1.QR(Q,R);
	tmp1 = R*Q;

	for (int i=0; i<rows; i++)
	{

	if (tmp1.buf[i*columns+i] >-1&&tmp1.buf[i*columns+i] < 1)
	{
	    if (fabs(tmp1.buf[i*columns+i]-tmp2.buf[i*columns+i])>Standard)
	    {
		cout<<fabs(tmp1.buf[i*columns+i]-tmp2.buf[i*columns+i])<<" > "<<Standard<<endl;
		judge=false;

	    }
	    else
	    continue;
	}
	else
	{
	    if (fabs(1-tmp2.buf[i*columns+i]/tmp1.buf[i*columns+i])>Standard)
	    {
		judge=false;

	    }
	    else
	    continue;
	}
	}

	for (int j=0; j<columns; j++)
	{
	    for (int i=j+1; i<rows;i++)
	    {
		if (tmp1.buf[i*columns+j]<Standard) continue;
		else
		{
		    m = 0;
		    break;
		}
	    }
	}
	m++;
	judge = false;
	tmp2 = tmp1;
	}
	while (m <= 1);
	tmp.rows = rows;
	tmp.columns = 1;
	tmp.buf = new double[rows];
	for (int i=0; i<tmp1.rows; i++)
	{
	tmp.buf[i] = tmp1.buf[i*columns+i];
	cout << tmp.buf[i] << endl;
	}
	Vector vector_tmp(rows);
	for (int i = 0; i < rows; i++)
	vector_tmp[i] = tmp.buf[i];

	return vector_tmp; 
}


Vector Matrix::Root(const Vector& b)//solving linear system of equations. b is actually a vector (mx1 Matrix) 
{
  	Vector tmp;
	if (!columns == b.Size())          // what if its not the square matrix
	{
		cout<<"Wrong size"<<endl;    //only square Matrix has only answer
		return b;
	}
	else
	{  
		Matrix B;
		B= this->Inverse();
		tmp = B*b;
	}
	return tmp; 
}

//------------------------------------------------------------------------------------------------
//operators, + - * can be overloaded as global operators

Matrix operator + (const Matrix& A, const Matrix& B) //Matrix A+B, using += .....
{
	Matrix tmp=A;
	tmp+=B;//use "+="
	return tmp;//
}

Matrix operator - (const Matrix& A, const Matrix& B) //Matrix A+B, using -= .....
{
	Matrix tmp=A;
	tmp-=B;//use "-="
	return tmp;//
}



Matrix operator * (const Matrix& A, const Matrix& B) //Matrix A+B, using *= .....
{
	Matrix tmp=A;
	tmp*=B;//use "*="
	return tmp;//
}



Matrix operator * (double a, const Matrix& A) //Matrix a*A, using *= .....
{
	Matrix tmp=A;
	//do a*A
	tmp*=a;
	return tmp;
};



Matrix operator * (const Matrix& A, double a ) //Matrix A*a, using *= .....
{
	Matrix tmp=A;
	//do A*a
	tmp*=a;
	return tmp;
};



Vector operator * (const Vector& b, const Matrix& A ) //Matrix b*A, using *= .....
{
	//do b*A
        Vector tmp(A.GetColumns());
        for(int i=0;i<A.GetColumns();i++)
        tmp[i]=0.0;
	//check size
	if(A.GetRows()!=b.Size())
	{
		cout<<"Wrong size"<<endl;
		return 	tmp;
	}
        for(int i = 0;i < A.GetColumns();i++)
        {
                for(int j = 0;j < A.GetRows();j++)
                tmp[i]=tmp[i] + b[i]*A(j,i);
        }
	return tmp;
};



Vector operator * (const Matrix& A, const Vector& b) //Matrix A*b,
{
	//do A*b
        Vector tmp(A.GetRows());
        for(int i=0;i<A.GetRows();i++)
        tmp[i]=0.0;
	if(A.GetColumns()!=b.Size())
	{
		cout<<"Wrong size"<<endl;
		return 	tmp;
	}
        for(int i=0;i<A.GetRows();i++)
        {
                for(int j=0;j<A.GetColumns();j++)
                tmp[i]=tmp[i]+b[i]*A(i,j);
        }
	return tmp;
}
