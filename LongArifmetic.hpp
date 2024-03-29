// LongArifmetic.cpp : Defines the entry point for the console application.

#include <iostream>
#include <iomanip>

template<typename T>
T max(T a, T b) {
	if (a > b)
		return a;
	return b;
}


class Number {
public:
	static const int MAX_NUMBER_LENGTH = 256;
private:
	static const int base = 1000 * 1000 * 1000;

	int				mIntegerData[MAX_NUMBER_LENGTH];
	size_t			mLength;
	bool			mNegative;

	static bool		_compare(const Number& _big, const Number& _small) {
		if (_small.size() > _big.size())
			return false;
		else if (_small.size() < _big.size())
			return true;
		else {
			for (size_t i = 0; i < _big.size(); i++)
				if (_big[i] > _small[i])
					return true;
				else if (_big[i] < _small[i])
					return false;
		}
		return true;
	}

	static Number	_sum(const Number& _a, const Number& _b) {
		Number result;

		int carry = 0;
		int index = 0;
		for (size_t i = 0; i<max(_a.size(), _b.size()) || carry; ++i) {
			int arg = (i == _a.size()) ? 0 : _a[i];
			//std::cout << arg << " + " << (i < _b.size() ? _b[i] : 0) << std::endl;
			result[i] = arg + carry + (i < _b.size() ? _b[i] : 0);
			carry = result[i] >= base;
			if (carry)
				result[i] -= base;
			result.size(i + 1);
			//std::cout << "result[" << i << "] = " << result[i] << std::endl;
		}
		return result.normalize();
	}

	static Number	_subtraction(const Number& _a, const Number& _b) {
		Number result;

		int carry = 0;
		for (size_t i = 0; i<_b.size() || carry; ++i) {
			std::cout << _a[i] << " - " << (i < _b.size() ? _b[i] : 0) << std::endl;
			result[i] = _a[i] + carry - (i < _b.size() ? _b[i] : 0);
			carry = result[i] < 0;
			std::cout << "carry = " << carry << std::endl;
			if (carry)
				result[i] += base;
			result.size(i + 1);
			std::cout << "result[" << i << "] = " << result[i] << std::endl;
		}

		return result.normalize();
	}

	static Number	_multiple(const Number& _a, const Number& _b) {
		Number result;
		result.size(_a.size() + _b.size());

		for (size_t i = 0; i<_a.size(); ++i)
			for (int j = 0, carry = 0; j < (int)_b.size() || carry; ++j) {
				long long cur = /*result[i + j] + */_a[i] * 1ll * (j < (int)_b.size() ? _b[j] : 0) + carry;
				//std::cout << "result[" << i+j << "] = " << _a[i] << " * " << (j < (int)_b.size() ? _b[j] : 0) << " = " << cur << std::endl;
				result[i + j] = int(cur % base);
				carry = int(cur / base);
				result.size((i + j)+1);
			}
		return result.normalize();
	}
public:
	Number() : mNegative(false), mLength(0) {
	}
	Number(const Number& _copy, bool negativeInvert) : mNegative(false), mLength(0) {
		memcpy(mIntegerData, _copy.mIntegerData, MAX_NUMBER_LENGTH);
		mLength = _copy.mLength;
		mNegative = (negativeInvert) ? !_copy.mNegative : _copy.mNegative;
	}
	Number(const char* _str) : mNegative (false), mLength (0) {
		if (strlen(_str) > 0) {
			size_t size = strlen(_str);
			char* s = new char[size+1];
			strcpy_s(s, size+1, _str);
			s[strlen(_str)] = 0;

			for (int i = (int)size; i > 0; i -= 9) {
				s[i] = 0;
				mIntegerData[mLength++] = atoi(i >= 9 ? s + i - 9 : s);
			}

			if (mLength > 0)
				if (mIntegerData[mLength - 1] < 0) {
					mIntegerData[mLength - 1] *= -1;
					mNegative = true;
				}
			delete[]s;
		}
	}

	/** Removing zeros */
	Number&	normalize() {
		if (valid())
			for (size_t i = mLength - 1; i >= 0; i++) {
				if (mIntegerData[i] != 0) {
					mLength = i + 1;
					break;
				}

			}
		return *this;
	}

	bool	valid() const {
		return (mLength > 0);
	}

	void	setNegative(bool _value) {
		mNegative = _value;
	}
	bool	isNegative() const {
		return mNegative;
	}

	/* Memory leak, nothing not free result[]*/
	const char* c_str() const {
		size_t size = 9 * mLength + 1;
		char* result = new char[size];
		memset(result, 0, size);

		char* buffer = result;
		sprintf_s(buffer++, size--, "-");

		if (mLength > 0) {
			sprintf_s(buffer, size,"%d", mIntegerData[mLength - 1]);
			size -= strlen(buffer);
			buffer += strlen(buffer);

			for (int i = mLength - 2; i >= 0; i--) {
				sprintf_s(buffer, size, "%09d", mIntegerData[i]);
				size -= 9;
				buffer += 9;
			}
		}
		else
			sprintf_s(buffer, size, "INVALID");

		return result;
	}
	void	print() const {
		if (mNegative)
			printf("-");

		if (mLength > 0) {
			printf("%d", mIntegerData[mLength - 1]);

			for (int i = mLength - 2; i >= 0; i--)
				printf("%09d", mIntegerData[i]);
		} else
			printf("INVALID");

		printf("\n");
	}

	size_t	size() const {
		return mLength;
	}

	void 	size(size_t _size) {
		mLength = _size;
	}

	int& operator[] (const int _index) const {
		return ((int*)mIntegerData)[_index];
	};

	bool operator==(const Number& _right) const {

		const Number& a = *this;
		const Number& b = _right;

		if (a.size() == b.size()) {
			if (a.isNegative() == b.isNegative()) {
				for (size_t i = 0; i < a.size(); i++)
					if (a[i] != b[i])
						return false;
				return true;
			}
		}

		return false;
	}

	bool operator>(const Number& _right) const {

		const Number& a = *this;
		const Number& b = _right;

		// (-a) > (-b)
		if (mNegative && _right.mNegative) {
			return _compare(b, a);
		}
		// (+a) > (+b)
		else if (!mNegative && !_right.mNegative) {
			return _compare(a, b);
		}
		// (+a) > (-b)
		else if (!mNegative && _right.mNegative) {
			return true;
		}
		// (-a) > (+b)
		else if (mNegative && !_right.mNegative) {
			return false;
		}

		return false;
	}

	bool operator<(const Number& _right) const {

		const Number& a = *this;
		const Number& b = _right;

		// (-a) < (-b)
		if (mNegative && _right.mNegative) {
			return _compare(a, b);
		}
		// (+a) < (+b)
		else if (!mNegative && !_right.mNegative) {
			return _compare(b, a);
		}
		// (+a) < (-b)
		else if (!mNegative && _right.mNegative) {
			return false;
		}
		// (-a) < (+b)
		else if (mNegative && !_right.mNegative) {
			return true;
		}

		return false;
	}

	Number operator+(const Number& _right) const {

		Number result;

		if (valid() && _right.valid()) {

			const Number& a = *this;
			const Number& b = _right;

			// (-a) + (-b) = -(a+b)
			if (mNegative && _right.mNegative) {
				result = _sum(a, b);
				result.setNegative(true);
			}
			// (+a) + (+b) = a+b
			else if (!mNegative && !_right.mNegative) {
				return _sum(a, b);
			}
			// (+a) + (-b) = a - b
			else if (!mNegative && _right.mNegative) {
				return (a - Number(b, true));
			}
			// (-a) + (+b) = b - a
			else if (mNegative && !_right.mNegative) {
				return (b - Number(a, true));
			}
		}

		return result;
	}

	Number operator-(const Number& _right) const {
		Number result;

		if (valid() && _right.valid()) {
			const Number& a = *this;
			const Number& b = _right;

			// (-a) - (-b) = b - a
			if (mNegative && _right.mNegative) {
				if (a == b)
					return Number("0");
				else if (Number(b, true) > Number(a, true)) {
					return _subtraction(Number(b, true), Number(a, true));
				}
				else { // a > b
					result = _subtraction(Number(a, true), Number(b, true));
					result.setNegative(true);
				}

			}
			// (+a) - (+b) = a - b
			else if (!mNegative && !_right.mNegative) {
				if (a == b)
					return Number("0");
				if (a == b)
					return Number("0");
				else if (a > b) {
					return _subtraction(a, b);
				}
				else { // a > b
					result = _subtraction(b, a);
					result.setNegative(true);
				}
			}
			// (+a) - (-b) = a + b
			else if (!mNegative && _right.mNegative) {
				return _sum(a, Number(b, true));
			}
			// (-a) - (+b) = (-a) + (-b) = -(a+b)
			else if (mNegative && !_right.mNegative) {
				result = _sum(Number(a, true), Number(b, true));
				result.setNegative(true);
			}
		}

		return result;
	}

	Number operator*(const Number& _right) const {

		Number result;

		if (valid() && _right.valid()) {

			const Number& a = *this;
			const Number& b = _right;

			result = _multiple(a, b);

			// (-a) * (-b) = -(a+b)
			if (mNegative && _right.mNegative) {
			}
			// (+a) * (+b) = a+b
			else if (!mNegative && !_right.mNegative) {
			}
			// (+a) * (-b) = a - b
			else if (!mNegative && _right.mNegative) {
				result.setNegative(true);
			}
			// (-a) * (+b) = b - a
			else if (mNegative && !_right.mNegative) {
				result.setNegative(true);
			}
		}

		return result;
	}

	friend std::ostream& operator<<(std::ostream& stream, const Number& obj) {

		if (obj.mNegative)
			stream << "-";

		if (obj.mLength > 0) {
			stream << obj.mIntegerData[obj.mLength - 1];

			for (int i = obj.mLength - 2; i >= 0; i--)
				stream << std::setfill('0') << std::setw(9) << obj.mIntegerData[i] << std::setw(0);
		}
		else
			stream << "INVALID";

		return stream;
	}
};