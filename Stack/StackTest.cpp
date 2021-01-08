#include <iostream>
#include <string>
#include "Stack"

int main() {
	Stack<char> s;
	std::string inp;
	std::cin >> inp;

	std::cout << "Reverse of String Is : ";
	for (const auto& ch : inp)
		s.push(ch);

	while (!s.empty()) {
		std::cout << s.top();
		s.pop();
	}
	std::cout << "\n";
	return 0;
}