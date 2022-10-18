#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

const uint32_t BASE = 10;

class bigint {
private:
	std::vector<uint32_t> number;
public:
	bigint(std::string s);
	bigint(uint32_t n);
	bigint();

	bigint& operator++();
	bigint operator++(int);

	bigint& operator--();
	bigint operator--(int);

	friend bigint operator+(const bigint& n1, const bigint& n2);
	friend bigint operator+=(bigint& n1, const bigint& n2);

	friend bigint operator+(int n1, const bigint& n2);
	friend bigint operator+(const bigint& n1, int n2);
	friend bigint operator+=(bigint& n1, int n2);

	friend bigint operator-(const bigint& n1, const bigint& n2);
	friend bigint operator-=(bigint& n1, const bigint& n2);

	friend bigint operator-=(bigint& n1, int n2);

	friend bigint operator-(const bigint& n1, int n2);
	friend bigint operator-(int n1, const bigint& n2);

	friend bool operator==(const bigint& n1, const bigint& n2);
	friend bool operator!=(const bigint& n1, const bigint& n2);
	friend bool operator>(const bigint& n1, const bigint& n2);
	friend bool operator<(const bigint& n1, const bigint& n2);

	operator int() const;
	std::string to_string() const;

	friend bool operator==(int n1, const bigint& n2);
	friend bool operator==(const bigint& n1, int n2);
	friend bool operator!=(int n1, const bigint& n2);
	friend bool operator!=(const bigint& n1, int n2);

	friend bool operator>(int n1, const bigint& n2);
	friend bool operator>(const bigint& n1, int n2);
	friend bool operator<(int n1, const bigint& n2);
	friend bool operator<(const bigint& n1, int n2);

	friend std::ostream& operator<<(std::ostream& s, const bigint& b);
	friend std::istream& operator>>(std::istream& s, bigint& b);
};

inline bigint::bigint(std::string s) {
	// TODO: Сделать инициализацию вектора и заполнять с конца
	size_t len = s.size();
	for (size_t i = 0; i < len; i++)
		number.push_back(static_cast<uint32_t>(s[i] - '0'));
	std::reverse(number.begin(), number.end());
	while (len > 1 && !number[len - 1]) number.pop_back(), len--;
}

inline bigint::bigint(uint32_t n) : bigint(std::to_string(n)) { }

inline bigint::bigint() : bigint(0) { }

inline bigint& bigint::operator++()
{
	size_t len = this->number.size();
	size_t i;
	for (i = 0; i < len && this->number[i] == BASE - 1; i++)
		this->number[i] = 0;
	if (i == len)
		this->number.push_back(1);
	else
		this->number[i]++;
	return *this;
}

inline bigint bigint::operator++(int)
{
	bigint tmp;
	tmp = *this;
	++(*this);
	return tmp;
}

inline bigint& bigint::operator--()
{
	size_t len = this->number.size();
	size_t i;
	for (i = 0; i < len && this->number[i] == 0; i++)
		number[i] = BASE - 1;
	number[i]--;
	if (len > 1 && number[len - 1] == 0)
		number.pop_back();
	return *this;
}

inline bigint bigint::operator--(int)
{
	bigint tmp;
	tmp = *this;
	--(*this);
	return tmp;
}

inline bigint::operator int() const
{
	int value = 0;

	for (size_t i = 0; i < number.size(); i++)
		value += number[i] * static_cast<uint32_t>(pow(static_cast<double>(BASE), static_cast<double>(i)));

	return value;
}

inline std::string bigint::to_string() const
{
	std::string s;
	std::vector<uint32_t>::const_reverse_iterator it;
	for (it = number.rbegin(); it != number.rend(); it++)
		s += (char)('0' + *it);

	return s;
}

inline bigint operator+(const bigint& n1, const bigint& n2)
{
	bigint n3;
	uint32_t tmp = 0;
	size_t lenN1 = n1.number.size();
	size_t lenN2 = n2.number.size();
	size_t len = lenN1 > lenN2 ? lenN1 : lenN2;
	size_t minN = lenN1 < lenN2 ? lenN1 : lenN2;
	bigint maxNumb = (lenN1 > lenN2 ? n1 : n2);
	n3.number.assign(len, 0);

	for (size_t i = 0; i < len; i++)
	{
		uint32_t v = 0;
		if (i < minN)
			v = n1.number[i] + n2.number[i];
		else
			v = maxNumb.number[i];
		v += tmp;
		tmp = v / BASE;
		n3.number[i] = v % BASE;
	}

	if (tmp != 0)
		n3.number.push_back(tmp);

	return n3;
}

inline bigint operator+=(bigint& n1, const bigint& n2)
{
	bigint n3 = n1 + n2;
	n1 = n3;
	return n3;
}

inline bigint operator+(int n1, const bigint& n2)
{
	return bigint(static_cast<uint32_t>(n1)) + n2;
}

inline bigint operator+(const bigint& n1, int n2)
{
	return n1 + bigint(static_cast<uint32_t>(n2));
}

inline bigint operator+=(bigint& n1, int n2)
{
	return n1 += bigint(static_cast<uint32_t>(n2));
}

inline bigint operator-(const bigint& n1, const bigint& n2)
{
	bigint n3;
	int tmp = 0;
	size_t lenN1 = n1.number.size();
	size_t lenN2 = n2.number.size();
	size_t len = lenN1 > lenN2 ? lenN1 : lenN2;
	size_t minN = lenN1 < lenN2 ? lenN1 : lenN2;
	bigint maxNumb = (lenN1 > lenN2 ? n1 : n2);
	n3.number.assign(len, 0);

	for (size_t i = 0; i < len; i++)
	{
		int v;
		if (i < minN)
			v = static_cast<int>(n1.number[i] - n2.number[i]);
		else
			v = maxNumb.number[i];
		v += tmp;
		if (v < 0)
		{
			v += BASE;
			tmp = -1;
		}
		else
			tmp = 0;

		n3.number[i] = static_cast<uint32_t>(v);
	}

	while (len > 1 && !n3.number[len - 1]) n3.number.pop_back(), len--;
	return n3;
}

inline bigint operator-=(bigint& n1, const bigint& n2)
{
	bigint n3 = n1 - n2;
	n1 = n3;
	return n1;
}


inline bigint operator-=(bigint& n1, int n2)
{
	return n1 -= bigint(static_cast<uint32_t>(n2));
}

inline bigint operator-(const bigint& n1, int n2)
{
	return n1 - bigint(static_cast<uint32_t>(n2));
}

inline bigint operator-(int n1, const bigint& n2)
{
	return bigint(static_cast<uint32_t>(n1)) - n2;
}

inline bool operator==(const bigint& n1, const bigint& n2)
{
	return n1.number == n2.number;
}

inline bool operator!=(const bigint& n1, const bigint& n2)
{
	return !(n1 == n2);
}

inline bool operator>(const bigint& n1, const bigint& n2)
{
	size_t lenN1 = n1.number.size();
	size_t lenN2 = n2.number.size();
	size_t len = lenN1 > lenN2 ? lenN1 : lenN2;
	if (lenN1 != lenN2)
		return lenN1 > lenN2;
	for (int i = len - 1; i >= 0; i--)
		if (n1.number[i] != n2.number[i])
			return n1.number[i] > n2.number[i];

	return false;
}

inline bool operator<(const bigint& n1, const bigint& n2)
{
	return n2 > n1;
}

inline bool operator==(int n1, const bigint& n2)
{
	return static_cast<int>(n1) == static_cast<int>(n2);
}

inline bool operator==(const bigint& n1, int n2)
{
	return static_cast<int>(n1) == static_cast<int>(n2);
}

inline bool operator!=(int n1, const bigint& n2)
{
	return static_cast<int>(n1) != static_cast<int>(n2);
}

inline bool operator!=(const bigint& n1, int n2)
{
	return static_cast<int>(n1) != static_cast<int>(n2);
}

inline bool operator>(int n1, const bigint& n2)
{
	return static_cast<int>(n1) > static_cast<int>(n2);
}

inline bool operator>(const bigint& n1, int n2)
{
	return static_cast<int>(n1) > static_cast<int>(n2);
}

inline bool operator<(int n1, const bigint& n2)
{
	return static_cast<int>(n1) < static_cast<int>(n2);
}

inline bool operator<(const bigint& n1, int n2)
{
	return static_cast<int>(n1) < static_cast<int>(n2);
}

inline std::ostream& operator<<(std::ostream& s, const bigint& b)
{
	// TODO: Переделать без итераторов
	for (std::vector<unsigned int>::const_reverse_iterator it = b.number.rbegin(); it != b.number.rend(); it++)
		s << (char)('0' + *it);
	return s;
}

inline std::istream& operator>>(std::istream& s, bigint& b)
{
	b = bigint();
	std::string str;
	s >> str;
	for (size_t i = 0; i < str.size(); i++)
		b.number.push_back(static_cast<uint32_t>(str[i] - '0'));
	std::reverse(b.number.begin(), b.number.end());

	size_t len = b.number.size();
	while (!b.number[len - 1] && len > 1) b.number.pop_back(), len--;

	return s;
}
