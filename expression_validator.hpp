#ifndef EXPRESION_VALIDATOR_HPP
#define EXPRESION_VALIDATOR_HPP

#include <string>
#include <cctype> 
#include <set>

class simbol
{
public:
	enum type {
		digit_type,
		alpha_type,
		operator_type,
		open_bracket_type,
		close_bracket_type,
		unhandled
	};
public:
	explicit simbol(char c)
		: m_simbol(c)
	{
	}
	
	simbol()
		: m_simbol(0)
	{
	}

	bool is_digit() const
	{
		return std::isdigit(m_simbol); // return  m_simol >= '0' && <= '9'
	}

	bool is_alpha() const
	{
		return std::isalpha(m_simbol); // return  m_simol >= 'a' && <= 'z' ||
					       // return  m_simol >= 'A' && <= 'Z'
	}

	bool is_operator() const
	{
		static std::set<char> operators;
		if ( operators.empty() ) {
			operators.insert('-');
			operators.insert('+');
			operators.insert('/');
			operators.insert('*');
			operators.insert('=');
		}
		return operators.end() != operators.find(m_simbol);
	}

	bool is_opening_bracket() const
	{
		return '(' == m_simbol;
	}

	bool is_closing_bracket() const
	{
		return ')' == m_simbol;
	}

	bool need_to_skip() const
	{
		return '/0' == m_simbol || ' ' == m_simbol;
	}

	type get_type() const
	{
		if ( is_digit() ) {
			return digit_type;
		}
		if ( is_alpha() ) {
			return alpha_type;
		}
		if ( is_operator() ) {
			return operator_type;
		}
		if ( is_opening_bracket() ) {
			return open_bracket_type;
		}
		if ( is_closing_bracket() ) {
			return close_bracket_type;
		}
		return unhandled;
	}
private:
	char m_simbol;
};

class exp_validator
{
private:
	exp_validator()
		: m_simbol()
		, m_direction(0)
	{
	}
public:
	static bool is_valid_expresion(const std::string& exp)
	{
		return exp_validator()(exp);
	}
private:
	bool operator()(const std::string& exp)
	{
		if ( exp.empty() ) {
			return true;
		}
		std::string::const_iterator i = exp.begin();
		while ( i != exp.end() && simbol(*i).need_to_skip() ) {
			++i;
		}
		set(simbol(*i));
		while( ++i != exp.end() ) {
			if (!consider_current_simbol(simbol(*i))) {
				return false;
			}
		}
		return are_valid_brackets();
	}
private:
	bool consider_current_simbol(const simbol& current)
	{
		if ( current.need_to_skip() ) {
			return true;
		}
		if ( !is_valid(current) ) {
			return false;
		}
		set(current);
		return true;
	}

	bool is_valid(const simbol& s) const
	{
		switch (m_simbol.get_type()) {
		case simbol::digit_type:
		case simbol::close_bracket_type:
			return s.is_operator() || s.is_closing_bracket();
		case simbol::alpha_type:
			return s.is_operator() || s.is_digit() || s.is_closing_bracket();
		case simbol::open_bracket_type:
		case simbol::operator_type:
			return s.is_alpha() || s.is_digit() || s.is_opening_bracket();
		}
		return false;
	}

	bool are_valid_brackets() const
	{
		return 0 == m_direction;
	}

	void set(const simbol& s)
	{
		m_simbol = s;
		if (m_simbol.is_opening_bracket()) {
			++m_direction;
		}
		else if (m_simbol.is_closing_bracket()) {
			--m_direction;
		}
	}
private:
	simbol m_simbol;
	int m_direction;
};

#endif /// EXPRESION_VALIDATOR_HPP