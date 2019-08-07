// LongArifmetic.cpp : Defines the entry point for the console application.

#include <iostream>

#include <stdio.h>
#include <malloc.h>

template<typename T>
T max(const T& a, const T& b) {
	if (a > b)
		return a;
	return b;
}

template<typename T>
class Array {
private:
	T*		mData;
	size_t	mSize;
public:
	Array() : mData(nullptr), mSize(0) {}
	Array(const Array& _source) {
		mSize = _source.mSize;
		if (mSize > 0) {
			mData = new T[mSize];
			memcpy(mData, _source.mData, mSize * sizeof(T));
		}
		else
			mData = nullptr;
	}
	Array(size_t _size) : mSize(_size) {
		mData = new T[mSize];
	}
	~Array() {
		if (mData)
			delete mData;
	}

	size_t getSize() const { 
		return mSize;
	};

	bool	valid() const {
		return (mData && mSize > 0);
	}

	T& operator[] (int _index) const {
		return mData[_index];
	};
};


template<typename T>
Array<T>	long_arifmetic_sum(const Array<T>& _a, const Array<T>& _b) {
	size_t size = max(_a.getSize(), _b.getSize()) + 1;

	Array<T> result(size);

	if (result.valid()) {
		int		i = _a.getSize() - 1;
		int		j = _b.getSize() - 1;
		int		carry = 0;
		int		k = 0;

		for (; i >= 0 && j >= 0; i--, j--, k++) {
			result[k] = (_a[i] + _b[j] + carry) % 10;
			carry = (_a[i] + _b[j] + carry) / 10;
		}

		if (_a.getSize() > _b.getSize())
		{
			while (i >= 0)	{
				result[k++] = (_a[i] + carry) % 10;
				carry = (_a[i--] + carry) / 10;
			}
		}
		else if (_a.getSize() < _b.getSize()) {
			while (j >= 0)	{
				result[k++] = (_b[j] + carry) % 10;
				carry = (_b[j--] + carry) / 10;
			}
		}
		else {
			if (carry > 0)
				result[k++] = carry;
		}

	}

	return result;
}

class LongNumber
{
private:
	Array<int>		mIntegerData;
	bool			mNegative;
private:
	

public:
	LongNumber() {}
	LongNumber(const Array<int>& _data, bool _negative) : mIntegerData(_data), mNegative(_negative) {}

	LongNumber(const char* _str) {
		std::cout << "digit : " << _str << std::endl;

		mNegative = (strstr(_str, "-") == _str);

		if (strstr(_str, "+") == _str || strstr(_str, "-") == _str) {
			_str++;
		}
		
		size_t size = strlen(_str);
		if (size > 0) {
			mIntegerData = Array<int>(size);
			for (size_t i = 0; i < size; i++) {
				char digit[2] = { _str++[0], 0 };
				std::cout << "\tfound : " << digit[0] << std::endl;
				mIntegerData[i] = atoi(digit);
			}
		}
	}
	

	~LongNumber() {
	}

	bool	valid() const {
		return mIntegerData.valid();
	}

	LongNumber operator+(const LongNumber& _right) {
		if (valid() && _right.valid()) {
			const LongNumber& a = *this;
			const LongNumber& b = _right;

			return LongNumber(long_arifmetic_sum(a.mIntegerData, b.mIntegerData), false);
			/*
			int		carry	= 0;
			int		k		= 0;

			int		i		= a.mIntegerLenght - 1;
			int		j		= b.mIntegerLenght - 1;
			
			int		size	= max(i, j);
			int*	data	= new int[size];

			if (data) {
				for (; i >= 0 && j >= 0; i--, j--, k++)	{
					data[k] = (a.mIntegerData[i] + b.mIntegerData[j] + carry) % 10;
					carry = (a.mIntegerData[i] + b.mIntegerData[j] + carry) / 10;
				}

				if (a.mIntegerLenght > b.mIntegerLenght)
				{
					while (i >= 0)
					{
						data[k++] = (a.mIntegerData[i] + carry) % 10;
						carry = (a.mIntegerData[i--] + carry) / 10;
					}
				}
				else if (mIntegerLenght < _right.mIntegerLenght)
				{
					while (j >= 0)
					{
						data[k++] = (_right.mIntegerData[j] + carry) % 10;
						carry = (_right.mIntegerData[j--] + carry) / 10;
					}
				}
				else
				{
					if (carry > 0)
						data[k++] = carry;
				}
			}
			*/
		}

		return LongNumber();
		
	}

	friend std::ostream& operator<<(std::ostream& stream, const LongNumber& obj) {
		if (obj.valid()) {
			if (obj.mNegative)
				stream << "-";
			for (size_t i = 0; i < obj.mIntegerData.getSize(); i++) {
				//std::cout << "print " << obj.mIntegerData[i] << std::endl;
				stream << obj.mIntegerData[i];
			}
		}
		else {
			stream << "invalid";
		}
		return stream;
	}
};



int main(char* argv[], int argc)
{
	LongNumber a("+3855468");
	LongNumber b("789656669878787");
	LongNumber c("-89667");

	std::cout << "a = " << a << std::endl;
	std::cout << "b = " << b << std::endl;
	std::cout << "c = " << c << std::endl;

	//std::cout << "a + b = " << a + b << std::endl;
	//std::cout << "a + c = " << a + c << std::endl;

	std::cout << getchar();

    return 0;
}

