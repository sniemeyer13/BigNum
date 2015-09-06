//Sarah Niemeyer
//BigNum Part 2
//100027519
//Matt Sainz


// FILENAME: BigNum.cxx
// This is the implementation file of the BigNum class


#ifndef HW3_BIGNUM_CXX
#define HW3_BIGNUM_CXX
#include <algorithm>   // Provides copy function
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include "BigNum.h"
using namespace std;

namespace HW3
{
	BigNum::BigNum()
	{
		capacity = DEFAULT_CAPACITY;
		digits = new unsigned int[capacity]; 
		digits[0] = 0;
		positive = true;      
		used = 1;
	}    

	BigNum::BigNum(int num)
	{
		digits = nullptr;

		if (num == 0) 
		{
			BigNum zero;
			*this = zero;
		}
    
		else 
		{
			if (num > 0) 
			{
				positive = true;
			}
			else 
			{
				positive = false;
				num = -num;
			}
      
			unsigned int i = 0, temp = num;
      
			// count the digits
			while (temp > 0) 
			{
				temp = temp/10; 
				i++;
			}

			capacity = i; 
		
			digits = new unsigned int[capacity];

			temp = num;
		
			for (used = 0; used < i; ++used) 
			{
				digits[used] = temp % 10;
				temp /= 10;
			}
		}
	}

	// Constructor that receives a string; leading 0's will be ignored
	BigNum::BigNum(const string& strin)
	{
		digits = nullptr;
		
		int len = strin.length();
		
		if (len == 0)
		{
			BigNum zero;
			*this = zero;
			return;
		}

		used = len;
		positive = true;
		
		int i = 0;

		if(strin[i] == '-')  
		{
			positive = false;
			i = 1;
			used--;
		}
		else if(strin[i] == '+')  
		{
			i = 1;
			used--;
		}
		
		capacity = used;
	
		digits = new unsigned int[capacity];

		for(unsigned int k = 0; k < used; ++k) 
		{
			digits[used - k - 1] = strin[i++] - '0';
		}
		
		if (used == 1 && digits[0] == 0)
			positive = true;
			
		trim();
	}
 
    BigNum::BigNum(const BigNum& anotherBigNum)
    {
		digits = nullptr;
		
		// makes operator = do the work; use that function if you use this
		*this = anotherBigNum;
    }

    BigNum::~BigNum()
    {
		delete [] digits;
    }
        
    // assume doubling is done before passing in n
	void BigNum::resize(unsigned int n)
	{
		unsigned int *largerArray;

		if (n < used) return; // Can't allocate less than we are using

		capacity = n;
		largerArray = new unsigned int[capacity];
		
		copy(digits, digits + used, largerArray);
		
		delete [] digits;
		digits = largerArray;
	}

	BigNum& BigNum::operator=(const BigNum& anotherBigNum)
	{
		if (this == &anotherBigNum)  return *this;
		
		if (digits != nullptr)
			delete [] digits;
			
		capacity = anotherBigNum.capacity;
		digits = new unsigned int[capacity];
		
		positive = anotherBigNum.positive;
		used = anotherBigNum.used;
		copy(anotherBigNum.digits, anotherBigNum.digits + used, digits);
  
		return *this;
	}

  
	BigNum& BigNum::operator+=(const BigNum& addend)  
	{
		BigNum newBigNum = *this + addend;
		*this = newBigNum;
		return *this;
	}

	BigNum& BigNum::operator-=(const BigNum& subtractand)
	{
		BigNum newBigNum = *this - subtractand;
		*this = newBigNum;
		return *this;
	}

	BigNum& BigNum::operator*=(const BigNum& multiplicand)
	{
		BigNum newBigNum = *this * multiplicand;
		*this = newBigNum;
		return *this;
	}

	BigNum& BigNum::operator/=(const BigNum& divisor)
	{
		BigNum newBigNum = BigNum(divisor);
		*this = *this/newBigNum;
		return *this;
	}

	BigNum& BigNum::operator%=(const BigNum& divisor)
	{
		BigNum newBigNum = BigNum(divisor);
		*this = *this%newBigNum;
		return *this;
	}

	BigNum& BigNum::operator++()
	{
		BigNum newBigNum = BigNum(1);
		*this = *this + newBigNum;
		return *this;
	}

	BigNum& BigNum::operator--()
	{
		BigNum newBigNum = BigNum(1);
		*this = *this - newBigNum;
		return *this;
	}

	BigNum& BigNum::diff(const BigNum& a, const BigNum& b)
	{
		unsigned int borrow = 0;
		unsigned int c = 0;
		used = 0;
		BigNum zero = 0;
		BigNum temp;
		temp.resize(a.used);
		*this = temp;
		
		
		
		if(a == zero)
		{
			*this = zero;
			return *this;
		}
		if(b == zero)
		{
			*this = a;
			return *this;
		}
		
		
		
		while(c < b.used)
		{
			if(a.digits[c] < b.digits[c] + borrow)
			{
				digits[c] = a.digits[c]+ 10 - b.digits[c] -borrow;
				borrow = 1;
			}
			else
			{
				digits[c] = a.digits[c] - b.digits[c] - borrow;
				borrow = 0;
			}
			used++;
			c++;
		}
		used--;
		
		
		while(c < a.used)
		{
			if(a.digits[c] < borrow)
			{
				digits[c] = a.digits[c] + 10 - borrow;
				borrow = 1;
			}
			else
			{
				digits[c] = a.digits[c] -borrow;
				borrow = 0;
			}
			used++;
			c++;
		}
		
		
		trim();
		return *this;
	}  

	BigNum& BigNum::mult(const BigNum& a, const BigNum& b)
       {
			BigNum zero;
			*this = zero;
			resize(a.used + b.used);
			unsigned int product = 0;
			unsigned int carry;
			unsigned int i, x, y;
			if(a == 1)
			{
				*this = b;
				return *this;
			}
			else if(b == 1)
			{
				*this = a;
				return *this;
			}


			for (i = 0; i < a.used; ++i)
			{
				BigNum int_result;
				if (a.digits[i] > 0)
				{
					int_result.resize(a.used + b.used);
					int_result.used = 0;

					for (y = 0; y < i; ++y)
					{
						int_result.used = int_result.used + 1;
						int_result.digits[y] = 0;
					}

					carry = 0;
					for (x = 0; x < b.used; ++x)
					{
						product = a.digits[i] * b.digits[x];
						product += carry;
						int_result.used = int_result.used + 1;
						int_result.digits[y + x] = product % 10;
						carry = product / 10;
					}
					while (carry > 0)
					{
						int_result.used = int_result.used + 1;
						int_result.digits[y + x] = carry % 10;
						carry = carry / 10;
						++y;
					}
					int_result.trim();
				}
				*this += int_result;
			}
			trim();
			return *this;
        }	
	BigNum& BigNum::sum(const BigNum& a, const BigNum& b) 
	{

		unsigned int sum = 0;
		unsigned int carry = 0;
		unsigned int number = 0;
		unsigned int x, y;
		used = 0;
		if(a.used == b.used && a.used ==1 &&a.digits[0] == 0)
		{
			used = 1;
			digits[0] = 0;
			return *this;
		}
		if(a.used > b.used)
		{
			y = b.used;
			x = a.used;
		}
		else
		{
			y = a.used;
			x = b.used;
		}
		resize(x+1);
		for(unsigned int i = 0; i < x; i++)
		{
			sum = 0;
			number = 0;
			if(y> i)
				number = a.digits[i]+b.digits[i];
			else
				if(a.used>b.used)
					number = a.digits[i];
				else
					number = b.digits[i];
			number = number + carry;
			sum = (sum + number)%10;
			carry = number/10;
			digits[i] = sum;
			used++;
		}
		if(carry > 0)
		{
			digits[x] = carry;
			used++;
		}
		trim();
		return *this;
	}


	BigNum operator+(const BigNum& a, const BigNum& b)
	{
		BigNum result;
		
		
		if(a.positive ==true && b.positive==true)
		{
			result.sum(a, b);
			result.positive = a.positive;
			return result;
		}
		
		
		else if(a.positive == false && b.positive == false)
		{
			BigNum temp1 = a;
			BigNum temp2 = b;
			temp1.positive = true;
			temp2.positive = true;
			result.sum(temp1, temp2);
			result.positive = false;
			return result;
		}
		
		
		else if(a.positive == true && b.positive == false)
		{
			BigNum temp1 = b;
			temp1.positive = true;
			if(a > temp1)
			{
				result.diff(a, temp1);
				result.positive = true;
				return result;
			}
			else if(a < temp1)
			{
				result.diff(temp1, a);
				result.positive = false;
				return result;
			}
			
		}
	
		
		
		else if(a.positive == false && b.positive == true)
		{
			BigNum temp1 = a;
			temp1.positive = true;
			if(temp1 > b)
			{
				result.diff(temp1, b);
				result.positive = false;
				return result;
			}
			else if (temp1 < b)
			{
				result.diff(b, temp1);
				result.positive = true;
				return result;
			}
			
		
		
	}
	return result;
} 


	BigNum operator-(const BigNum& a, const BigNum& b)
	{
		BigNum result;
		//see comments for operator +
		if(a.positive == true && b.positive == true)
		{
			if(a.used > b.used)
			{
				result.diff(a, b);
				result.positive = true;
				return result;
			}
			else if(a.used < b.used)
			{
				result.diff(b, a);
				result.positive = false;
				return result;
			}
			else
			{
				if(a>=b)
				{
					result.diff(a, b);
					result.positive = true;
					return result;
				}
				else
				{
					result.diff(b,a);
					result.positive = false;
					return result;
				}
			}
		}
		
		
		else if(a.positive == true && b.positive == false)
		{
			BigNum temp1 = b;
			temp1.positive = true;
			result.sum(a, temp1);
			result.positive = true;
		}
		
		
		else if(a.positive == false && b.positive == false)
		{
			if(a.used < b.used)
			{
				BigNum temp1 = a;
				BigNum temp2 = b;
				temp1.positive = true;
				temp2.positive = true;
				result.diff(temp2, temp1);
				result.positive = true;
			}
			else if(a.used > b.used)
			{
				BigNum temp1 = a;
				BigNum temp2 = b;
				temp1.positive = true;
				temp2.positive = true;
				result.diff(temp1, temp2);
				result.positive = false;
			}
			else
			{
				BigNum temp1 = a;
				BigNum temp2 = b;
				temp1.positive = true;
				temp2.positive = true;
				if(a > b)
				{
					result.diff(temp1, temp2);
					result.positive = false;
				}
				else
				{
					result.diff(temp2, temp1);
					result.positive = true;
				}
			}
		}
			
		
		else if(a.positive == false && b.positive == true)
		{
			BigNum temp1 = a;
			temp1.positive = true;
			result.sum(temp1, b);
			result.positive = false;
		}
		
		return result;
	}      


	BigNum operator*(const BigNum& a, const BigNum& b)
	{
		BigNum result;
		result.mult(a, b);
		
		//if they both have the same sign, the result is positive;
		//if they have different signs, the result is negative
		if(a.positive == b.positive)
		{
			result.positive = true;
		}
		else
		{
			result.positive = false;
		}
		return result;
	}


	BigNum operator / (const BigNum& a, const BigNum& b)
	{	
		BigNum result;
		BigNum anew = BigNum(a);
		BigNum bnew = BigNum(b);
		
		//if they're both the same sign, the answer is positive; if they
		//are different signs, the answer is negative
		if(anew.positive == false)
		{
			anew.positive = true;
		}
		if(bnew.positive == false)
		{
			bnew.positive = true;
		}
		
		//if the divisor is 0, you can't divide it!
		if(result == b)
		{
			cout<<"THE LIMIT DOES NOT EXIST!"<<endl;
		}
		//if the divisor is 1 then the answer is the dividend 
		else if(b == 1)
		{
			result = a;
			return result;
		}
		else if(a == 0)
		{
			result.used = 1;
			result.digits[0] = 0;
			return result;
		}
		//if the divisor is bigger than the dividend, the answer is zero
		else if(anew < bnew)
		{
			return result;
		}
		//if the dividend is bigger than the divisor, then keep adding one
		//to the result until it times the divisor is equal to or just less
		//than the dividend BUT you have to decriment result once you're done
		//with this because we have to do ++result because that is how you 
		//can impliment the ++ function with a BigNum... you may also
		//be able to just say while anew>result*bnew rather than >=?
		else if(anew > bnew)
		{
			while(anew >= result*bnew)
			{
				++result;
			}
			--result;
		}
		
		//same as before.. if they're both the same sign then the answer
		//is positive and if they're different signs the answer is -
		if(a.positive == b.positive)
		{
			result.positive = true;
		}
		else
		{
			result.positive = false;
		}
		
		
		return result;
	}


	BigNum operator%(const BigNum& a, const BigNum& b)
	{
		BigNum result;
		result = (a - (a/b) * b);
		return result;
	}

	bool operator>(const BigNum& a, const BigNum& b)
	{
		if (a.positive == true && b.positive == false) return true;
		
		else if (a.positive == false && b.positive == true) return false;
		
		else 
		{
			if (a.used > b.used) 
			{
				if (a.positive == true) return true;
				else return false;
			}
			
			else if (a.used < b.used) 
			{
				if (a.positive == true) return false;
				else return true;
			}
	
			else 
			{
				for (unsigned int i = 0; i < a.used; ++i) 
				{
					if (a.digits[a.used - 1 - i] < b.digits[b.used - 1 - i])  
					{
						if(a.positive == true) return false;
						else return true;
					}
					if (a.digits[a.used - 1 - i] > b.digits[b.used - 1 - i])  
					{
						if(a.positive == true) return true;
						else return false;
					}
				}
			}          
		}
		return false;
	}


	bool operator>=(const BigNum& a, const BigNum& b)
	{
		return ((a > b) || (a == b));
	}


	bool operator<(const BigNum& a, const BigNum& b)
	{
		return !(a >= b);
	}


	bool operator<=(const BigNum& a, const BigNum& b)
	{
		return !(a > b);
	}


	bool operator==(const BigNum& a, const BigNum& b)
	{
		if ((a.positive != b.positive) || (a.used != b.used))
			return false;

		for (unsigned int i = 0; i < a.used; i++) 
		{
			if (a.digits[a.used - 1 - i] != b.digits[b.used - 1 - i]) 
				return false;
		}

		return true;
	}

	
	
	int get_int_len(int value)
	{
		int l = 1;
		while(value> 9)
		{
			l++;
			value/=10;
		}
		return l;
	}

	
	
	bool operator!=(const BigNum& a, const BigNum& b)
	{
		return !(a == b);
	}

	// trim leading zeros
	void BigNum::trim()
	{
		while (used > 1 && digits[used-1] == 0)
			used--;
	}
	
	std::ostream& operator<<(std::ostream &os, const BigNum& bignum)
	{
		unsigned int i = 0;
		unsigned int j = 0;

		if (bignum.positive == false)  os << '-';

		for (i=0; i<bignum.used; ++i) 
		{
			os << bignum.digits[bignum.used - i - 1];
			if (j < 60) ++j; 
			else 
			{
				os << endl;
				j = 0;
			}
		}
    
		return os;
	}	 

	std::istream& operator>>(std::istream &is, BigNum& bignum)
	{
		string str;
		is >> str;

		BigNum temp = str;
		bignum = temp;
		return is;
	}
	
	BigNum factorial(const BigNum& a)
	{
		BigNum result;
		if(a<=1)
			return 1;
		else
		{
			result = a * factorial(a-1);
			return result;			
		}
	}
  }


#endif



