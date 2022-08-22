#include <map>
#include <vector>
#include <string>

#include <iostream>
#include <chrono>
#include <algorithm>
#include <fstream>

// ������� ��������� ������������� �� 10'000 
// ���������
// ��������� ���������� ��������� map 
// �������� ����� ���������� �� ����� � ������������
// ������ � �������� ������ � ����������� map
// 
// ����� 10 ������������� � ����������� �������� � ������� ���������
// �������� � ���� ������ �������������(� ������ ��������)


class User {
public:
	int id;
	std::string name;
	bool dayShift;
	int salary;

	User() = default;

	User(int id,std::string name,bool dayShift, int salary)
		:id(id),
		name(name),
		dayShift(dayShift),
		salary(salary)
	{}


	bool operator==(const User& other)const {
		return id == other.id && name == other.name;
	}

	friend std::ostream &operator<<(std::ostream &out, const User& obj){
		out << obj.id << ' ' << obj.name << ' ' << obj.dayShift << ' ' <<
			obj.salary << ' ';
		return out;
	}

	friend std::istream& operator>>(std::istream &in, User& obj) {
		in >> obj.id >> obj.name >> obj.dayShift >> obj.salary;
		return in;
	}
};


void task(int count) {
	std::vector<User> database;
	database.reserve(count);
	auto begin = std::chrono::steady_clock::now();
	for (int i = 0; i < database.capacity(); i++)
	{
		database.emplace_back(
			i + 1,
			"user" + std::to_string(i + 1),
			static_cast<bool>(::rand() % 2),
			10000 + ::rand()
		);
	}
	auto end = std::chrono::steady_clock::now();
	auto elapsed_ms =
		std::chrono::duration_cast<std::chrono::nanoseconds>
		(end - begin);
	std::cout << "create vector: " << elapsed_ms.count()<< " ms\n";
	
	std::map<std::string, User*> searchTree;
	
	begin = std::chrono::steady_clock::now();
	for (auto& user : database) {
		searchTree.insert({ user.name, &user });
	}
	end = std::chrono::steady_clock::now();
	elapsed_ms =
		std::chrono::duration_cast<std::chrono::nanoseconds>
		(end - begin);
	std::cout << "create map: " << elapsed_ms.count() << " ns\n";
	
	begin = std::chrono::steady_clock::now();
	auto vec_tmp = std::find(
		database.begin(),
		database.end(),
		User{ count,"user"+std::to_string(count), true, 11111}
	);
	end = std::chrono::steady_clock::now();
	elapsed_ms =
		std::chrono::duration_cast<std::chrono::nanoseconds>
		(end - begin);
	std::cout << "Find in vector: " << 
		elapsed_ms.count() << " ns\n";

	begin = std::chrono::steady_clock::now();
	auto map_tmp = searchTree.find("user" + std::to_string(count));
	end = std::chrono::steady_clock::now();
	elapsed_ms =
		std::chrono::duration_cast<std::chrono::nanoseconds>
		(end - begin);
	std::cout << "Find in map: " <<
		elapsed_ms.count() << " ns\n";


	std::ofstream out("base.txt", std::ios::app);
	for (auto& user:database) {
		//std::cout << user << '\n';
		out << user << '\n';
	}
	out.close();
	//---------------------

	/*
		����� ������� ��������
		����� 10 ����� �� ������� ���������
		 � ��� ��� ���� ������ ��� ��� ������� ����� �������� ��� �� ����
		 ��� � �� �������

		 �������� ������ ����� � ����
		 ���������� ��������� ��������� ��� ����� � ��� ��������� ��������� ���� 
		 ������� �����������
	*/
}

/*
������� ��� �������� ����� ������������ multimap

	������� ���� � ����������(id ������ �����, � ��������� ����� �� �����)
	�� ���� ���� ������� ����������:
	������ ��� ������� ����� ������ ������������
	���� � ����� ������ �������� ������������ ������ ��� � ������� �������
	� ������� ����� 3 ����� ������� �����(������������ ������ ����� � ����� �������)
	� ������ ����� 3 ����� ������ �����(������������ ������ ����� � ����� �������)
	������ ����� � ������� � ������ ����� ����������� ������� � ������� ��������
	(������� ������ 5%)
*/

void task2() {
	std::vector<User> database;
	std::ifstream in("base.txt");
	while (!in.eof()) {
		User tmp;
		in >> tmp;
		tmp.id = database.size();
		database.push_back(tmp);
	}
	in.close();
	database.pop_back();
	for (auto& user : database) {
		std::cout << user << '\n';
	}
}

std::map<std::string, User> g_Users;

int main() {
	//for (size_t i = 0; i < 10; i++) {
	//	task(10'000);
	//}
	task2();


	/*g_Users.insert({ "user", {1,"user"} });
	User local;
	local.id = 2;
	local.name = "User2";

	g_Users.insert({ local.name, local });
	g_Users["User3"];
	g_Users["User3"].id = 3;
	g_Users["User3"].name = "User3";
	std::pair<const std::string, User> &tmp
		= *(g_Users.begin());

	auto where_ = g_Users.find("User3");
	where_->second.id;

	auto& [login, user] = *(g_Users.find("User3"));
	user.id;*/

	std::multimap<std::string, User> searchTree;

	//section - ���� ���������� ������ map 
	// �������� first & second - ������ � ����� ������� ��������� ��������������
	auto section = searchTree.equal_range("key");
	std::vector<User> names;
	
	//���� � ��������� �� ��� ��������� ������ �� ���������� � ������� ����������
	for (; section.first != section.second; section.first++) {
		names.push_back(section.first->second);
		//section.first - �������� ������������� �������� ������ � ���� ����
		// ����+�������� �� ��������� map 
		// �������������� section.first->second ��� ������������ �� map �������� �� 
		// ����� ���������� � vector
	}
	for (auto& el : names) {
		std::cout << el << '\n';
	}

}