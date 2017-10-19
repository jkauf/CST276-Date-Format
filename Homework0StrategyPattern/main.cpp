// Assignment #0
// Class: CST 276
// Professor: Mitch Besser
// Author: Julian Kaufman
// Date Due: 2017-10-02

#include <cassert>
#include <limits>
#include <string>
#include <ios>
#include <iostream>
#include <sstream>

using namespace std;


class IDateFormat
{
public:
	virtual bool parseDate(const string& date) = 0;
	bool isValid(const unsigned y, const unsigned m, const unsigned d) const;
	void skipDelim(istream& is) const;
	void setDelim(char delim) { delim_ = delim; }

	char delim_;
};

bool IDateFormat::isValid(const unsigned y, const unsigned m, const unsigned d) const
{

	auto result = y >= 1901 && y < 2100 && m >= 0 && m < 12 && d >= 0 && d < 31;

	if (result)
	{
		static const unsigned maxDays[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

		result = d < maxDays[m];

		constexpr auto FEB = 1U;

		if (result && m == FEB)
		{
			if (y % 4 != 0)
			{
				result = d < 28;
			}
		}
	}

	return result;
}

void IDateFormat::skipDelim(istream& is) const
{
	auto c = std::istream::char_type(0);

	is >> c;

	assert(c != ' ');

	if (c != delim_)
	{
		is.setstate(std::ios::failbit);
	}
}


/********************************************************************
Order: Year/Month/Day
********************************************************************/

class YMD : public IDateFormat
{
public:
	bool parseDate(const string& date) override;
};

bool YMD::parseDate(const string& date)
{
	auto dateStrStrm = std::istringstream(date);

	auto y = 0U;
	auto m = 0U;
	auto d = 0U;

	dateStrStrm >> y;
	skipDelim(dateStrStrm);
	dateStrStrm >> m;
	skipDelim(dateStrStrm);
	dateStrStrm >> d;

	auto c = std::istringstream::char_type(0);
	dateStrStrm >> c;

	auto result = dateStrStrm.eof() && c == 0 && isValid(y, m - 1, d - 1);

	return result;
}


/********************************************************************
Order: Day/Month/Year
********************************************************************/
class DMY : public IDateFormat
{
public: 
	bool parseDate(const string& date) override;
};

bool DMY::parseDate(const string& date) 
{
	auto dateStrStrm = std::istringstream(date);

	auto y = 0U;
	auto m = 0U;
	auto d = 0U;
	
	dateStrStrm >> d;
	skipDelim(dateStrStrm);
	dateStrStrm >> m;
	skipDelim(dateStrStrm);
	dateStrStrm >> y;

	auto c = std::istringstream::char_type(0);
	dateStrStrm >> c;

	auto result = dateStrStrm.eof() && c == 0 && isValid(y, m - 1, d - 1);

	return result;
}


/********************************************************************
Order: Month/Day/Year
********************************************************************/
class MDY : public IDateFormat
{
public:
	bool parseDate(const string& date) override;
};

bool MDY::parseDate(const string& date)
{
	auto dateStrStrm = std::istringstream(date);

	auto y = 0U;
	auto m = 0U;
	auto d = 0U;

	dateStrStrm >> m;
	skipDelim(dateStrStrm);
	dateStrStrm >> d;
	skipDelim(dateStrStrm);
	dateStrStrm >> y;

	auto c = std::istringstream::char_type(0);
	dateStrStrm >> c;

	auto result = dateStrStrm.eof() && c == 0 && isValid(y, m - 1, d - 1);

	return result;
}


/********************************************************************
Order: Year/Day/Month
********************************************************************/
class YDM : public IDateFormat
{
public:
	bool parseDate(const string& date) override;
}; 

bool YDM::parseDate(const string& date)
{
	auto dateStrStrm = std::istringstream(date);

	auto y = 0U;
	auto m = 0U;
	auto d = 0U;

	dateStrStrm >> y;
	skipDelim(dateStrStrm);
	dateStrStrm >> d;
	skipDelim(dateStrStrm);
	dateStrStrm >> m;

	auto c = std::istringstream::char_type(0);
	dateStrStrm >> c;

	auto result = dateStrStrm.eof() && c == 0 && isValid(y, m - 1, d - 1);

	return result;
}


class DateChecker
{
public: 
	void setOrder(IDateFormat* dateFormat) { dateFormat_ = dateFormat; }
	void setParameters(string date, char delim); 
	bool checkDate() {return dateFormat_->parseDate(date_); }
 
private:
	IDateFormat* dateFormat_;
	string date_;
	
};

void DateChecker::setParameters (string date, char delim)
{
	date_ = date; 
	dateFormat_->setDelim(delim);
}


int main()
{
	YMD* ymd = new YMD;
	DMY* dmy = new DMY;
	MDY* mdy = new MDY;
	YDM* ydm = new YDM;
	DateChecker aDate;
	bool valid;

	aDate.setOrder(ymd);
	aDate.setParameters("2003/02/13", '/');
	//aDate.setParameters("02/03/2005", '/');	//fails due to incorrect format
	valid = aDate.checkDate();
	assert(valid);

	aDate.setOrder(dmy);
	aDate.setParameters("13/02/2003", '/');
	//aDate.setParameters("02/03/1998", ',');	//fails due to mismatched delimiter
	valid = aDate.checkDate();
	assert(valid);

	aDate.setOrder(mdy);
	aDate.setParameters("02/13/2003", '/');
	//aDate.setParameters("13/15/2010", '/');	//fails due to month out of range
	valid = aDate.checkDate();
	assert(valid);

	aDate.setOrder(ydm);
	aDate.setParameters("2003/15/06", '/');
	//aDate.setParameters("2003/45/06", '/');	//fails due to day out of range
	valid = aDate.checkDate();
	assert(valid);

	return 0;
}


