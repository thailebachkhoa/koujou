#include <iostream>
#include <vector>
using namespace std;

// Lớp cơ sở ảo
class Shape {
public:
    virtual void draw() const = 0; // Hàm thuần ảo
    virtual ~Shape() {}            // Hàm hủy ảo
};

// Lớp con
class Circle : public Shape {
public:
    void draw() const override {
        cout << "Drawing Circle\n";
    }
};

class Rectangle : public Shape {
public:
    void draw() const override {
        cout << "Drawing Rectangle\n";
    }
};

int main() {
    vector<Shape*> shapes; // Vector chứa con trỏ đến lớp cơ sở ảo

    shapes.push_back(new Circle());
    shapes.push_back(new Rectangle());

    // Gọi hàm ảo qua con trỏ
    for (auto s : shapes) {
        s->draw(); // Gọi đúng hàm của lớp con (đa hình)
    }

    // Giải phóng bộ nhớ
    for (auto s : shapes) {
        delete s;
    }

    return 0;
}
