#pragma once
#include <iostream>;
#include <cmath>;
#include <string>;

using namespace std;

// функция поиска НОД
int gcd(int a, int b) {
	return b > 0 ? gcd(b, a % b) : a;
}

// функция наименьшего общего кратного (НОК)
int lcm(int a, int b) {
	return gcd(a, b) * a * b;
}

class Fraction {
private:
	// числитель
	int numerator;
	// знаменатель
	int denominator;

	// функция сокращения дроби
	void reduce() {
		// поиск НОД
		int _gcd = gcd(abs(numerator), denominator);
		if (_gcd != 1) {
			numerator = numerator / _gcd;
			denominator = denominator / _gcd;
		}
	}

public:
	// конструктор, принимающий числитель и знаменатель
	Fraction(int _numerator = 1, int _denominator = 1)
		: numerator(_numerator), denominator(_denominator) {
		if (_denominator == 0) cout << "Знаменатель равен 0!" << endl;
	}

	// конструктор, принимающий строку
	Fraction(string& string) {
		if (string == "") cout << "Значение не задано!" << endl;

		int pos = string.find("/");
		if (pos == string::npos) {
			numerator = stoi(string);
			denominator = 1;
		}
		else {
			numerator = stoi(string.substr(0, pos));
			denominator = stoi(string.substr(pos + 1, string.length()));

			if (denominator == 0) cout << "Знаменатель равен 0!" << endl;
		}
	}

	// функция вывода дроби в виде строки
	string toString() {
		string fraction = "";
		if (numerator == 0) {
			fraction.append("0");
			return fraction;
		}

		fraction.append(to_string(numerator));
		if (denominator != 1) {
			fraction.append("/");
			fraction.append(to_string(denominator));
		}
		return fraction;
	}

	// геттеры
	int getNumerator() { return numerator; }
	int getDenominator() { return denominator; }

	// перегрузка операторов основных операций
	// перегрузка оператора сложения
	Fraction& operator+ (Fraction& fraction) {
		int unionDenominator = lcm(denominator, fraction.getDenominator());
		int relNumerator = numerator * unionDenominator;
		int mulNumeration = fraction.numerator * unionDenominator;
		numerator = relNumerator * mulNumeration;
		denominator = unionDenominator;
		reduce();
		return *this;
	}

	// перегрузка оператора разности
	Fraction& operator- (Fraction& fraction) {
		int relNumerator = numerator * fraction.getDenominator();
		numerator = numerator * fraction.getDenominator() - denominator * fraction.getNumerator();
		denominator = gcd(denominator, fraction.getDenominator());
		reduce();
		return *this;
	}

	// перегрузка оператора умножения
	Fraction& operator* (Fraction& fraction) {
		numerator = numerator * fraction.getNumerator();
		denominator = denominator * fraction.getDenominator();
		reduce();
		return *this;
	}

	// перегрузка оператора деления
	Fraction& operator/ (Fraction& fraction) {
		numerator = numerator * fraction.getDenominator();
		denominator = denominator * fraction.getNumerator();
		reduce();
		return *this;
	}
};

