#include <sstream>;
#include <fstream>;
#include <vector>;
#include "Fraction.h"
using namespace std;

/*
	Алгоритм поиска приоритетов альтернатив и оптимальной альтернативы
	методом анализа иерархий
*/

// функция записи результата в файл
void output(vector<pair<int, float>> &opt) {

	ofstream out;
	out.open("output.txt");
	if (out.is_open()) {
		if (opt.empty()) out << "Оптимальная альтернатива отсутствует!" << std::endl;
		else {
			for (int i = 0; i < opt.size(); i++) {
				out << "Оптимальная альтернатива x" + to_string(opt[i].first) + " с приоритетом w" +
					to_string(opt[i].first) + " = " + to_string(round(opt[i].second * pow(10, 6)) / pow(10, 6)) << std::endl;
			}
		}
	}
}

// функция поиска оптимальной альтернативы
void findOptimalAlternative(vector<float> &pr_w, vector<pair<int, float>> &opt) {
	float max_value = pr_w[0];
	opt.push_back(make_pair(1, max_value));

	for (int i = 1; i < pr_w.size(); i++) {
		if (max_value < pr_w[i]) {
			max_value = pr_w[i];
			opt.clear();
			opt.push_back(make_pair(i + 1, max_value));
		}
		else if (max_value == pr_w[i]) {
			opt.push_back(make_pair(i + 1, max_value));
		}
	}
}

// функция поиска окончательных приоритетов альтернатив
void findPriorityAlternative(vector<float>& pr_e, vector<vector<float>>& pr_w_i, vector<float>& pr_w) {
	
	for (int i = 0; i < pr_w.size(); i++) {
		float sum = 0;
		for (int j = 0; j < pr_e.size(); j++) {
			sum += pr_e[j] * pr_w_i[j][i];
		}
		pr_w.push_back(sum);
	}
}

// функция поиска приоритета матрицы
void findPriority(Fraction** matrix, int size, vector<float>& pr) {

	float pr_sum = 0;
	for (int i = 0; i < size; i++) {

		Fraction value(1, 1);
		for (int j = 0; j < size; j++) {
			value = matrix[i][j].operator*(value);
		}
		pr.push_back(pow(value.getNumerator() / value.getDenominator(), 1.0 / size));
		pr_sum += pr[i];
	}

	for (int i = 0; i < size; i++) {
		pr[i] /= pr_sum;
	}
}

// функция реализации алгоритма анализа иерархий
void algorithmAnalysisHierarchy(int n, int m, int count_w, Fraction** E, 
	vector<Fraction**> W, vector<pair<int, float>> &opt) {

	vector<float> pr_e;
	vector<vector<float>> pr_w_i(count_w);

	// поиск приоритетов
	findPriority(E, n, std::ref(pr_e));
	for (int i = 0; i < pr_w_i.size(); i++) {
		findPriority(W[i], m, std::ref(pr_w_i[i]));
	}

	vector<float> pr_w(m);
	// окончательные приоритеты альтернатив
	findPriorityAlternative(pr_e, pr_w_i, std::ref(pr_w));

	// поиск оптимальной альтернативы
	findOptimalAlternative(pr_w, std::ref(opt));
}

// функция считывания матриц из консоли
void inputMatrix(int n, Fraction** matrix) {
	
	cin.ignore(cin.rdbuf()->in_avail());
	for (int i = 0; i < n; i++) {
		int j = 0;
		string row;
		getline(cin, row);
		istringstream ist(row);
		
		while (ist >> row) {
			matrix[i][j] = Fraction(row);
			j++;
		}
	}
}

// функция считывания матриц из файла
void importFromFile(int n, int m, Fraction** matrix_e, vector<Fraction**> matrix_w) {

	string str;
	ifstream in("input.txt");
	if (in.is_open()) {
		int i = 0,
			k = -1,
			count_w = matrix_w.size();
		while (getline(in, str)) {
			if (str.size() == 0) continue;

			string row;
			istringstream ist(str);
			int j = 0;
			if ((i - n) % m == 0 && i >= n) k++;

			while (ist >> row) {
				if (i < n) {
					matrix_e[i][j] = Fraction(row);
					j++;
					continue;
				}
				else if (i >= n && k < count_w) {
					if (k >= count_w) break;
					matrix_w[k][(i - n) % m][j] = Fraction(row);
					j++;
					continue;
				}
			}
			i++;
		}
	}
	in.close();
}

int main() {
	setlocale(LC_ALL, "ru");

	// считывание данных
	string ans;
	int n = 7, 
		m = 6,
		count_w = 7;
	while (true) {
		cout << "Использовать значения по умолчанию (2 вариант): (yes / no) ";
		cin >> ans;
		if (ans == "no" || ans == "yes") break;
	}

	if (ans == "no") {
		cout << "Введите размер матрицы Е:";
		cin >> n;
		cout << "Введите число матриц W:";
		cin >> count_w;
		cout << "Введите размер матриц W:";
		cin >> m;
		cout << "Введите матрицу Е:" << endl;
	}

	Fraction** E = new Fraction * [n];
	for (int i = 0; i < n; i++) {
		E[i] = new Fraction[n];
	}

	if (ans == "no") inputMatrix(n, std::ref(E));

	vector<Fraction**> W;
	for (int i = 0; i < count_w; i++) {
		W.push_back(new Fraction * [m]);
		for (int j = 0; j < m; j++) {
			W[i][j] = new Fraction[m];
		}
		if (ans == "no") {
			cout << "Введите матрицу W" + to_string(i + 1) + ":" << endl;
			inputMatrix(m, std::ref(W[i]));
		}
	}
	// считывание значений по умолчанию
	if (ans == "yes") importFromFile(n, m, std::ref(E), std::ref(W));

	vector<pair<int, float>> opt;
	// реализация алгоритма анализа иерархий
	algorithmAnalysisHierarchy(n, m, count_w, E, W, std::ref(opt));

	// запись результата в файл
	output(opt);
}