#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

class Vector2D {
private:
    std::string label;
public:
    double x, y;

    // Default constructor
    Vector2D() : label(""), x(0.0), y(0.0) {}

    // Custom constructor
    Vector2D(const std::string& label, double x, double y) : label(label), x(x), y(y) {}

    void set_label(const std::string& new_label) {
        label = new_label;
    }

    void set(double new_x, double new_y) {
        x = new_x;
        y = new_y;
    }

    void print() const {
        std::cout << "Label: " << label << ", x: " << x << ", y: " << y << std::endl;
    }

    std::string to_csv_header() const {
        return "label,x,y";
    }

    std::string to_csv_row() const {
        // Escape double quotes in label if present
        std::string escaped_label = label;
        size_t pos = 0;
        while ((pos = escaped_label.find('"', pos)) != std::string::npos) {
            escaped_label.insert(pos, "\"");
            pos += 2;
        }
        return '"' + escaped_label + '"' + "," + std::to_string(x) + "," + std::to_string(y);
    }

    double norm() const {
        return std::hypot(x, y);
    }

    void scale(double factor) {
        x *= factor;
        y *= factor;
    }

    void add(const Vector2D& other) {
        x += other.x;
        y += other.y;
    }
};

int main() {
    std::vector<Vector2D> vecs = {
        Vector2D("A", 1.0, 2.0),
        Vector2D("B", -3.5, 4.2),
        Vector2D("C", 0.0, -1.0)
    };

    // Print CSV header
    std::cout << vecs[0].to_csv_header() << std::endl;
    for (const auto& v : vecs) {
        std::cout << v.to_csv_row() << std::endl;
    }

    // Scale all elements by 2 (reference-based loop)
    for (auto& v : vecs) {
        v.scale(2.0);
    }

    std::cout << "\nAfter scaling by 2:" << std::endl;
    std::cout << vecs[0].to_csv_header() << std::endl;
    for (const auto& v : vecs) {
        std::cout << v.to_csv_row() << std::endl;
    }

    // Test: copy-based loop (no effect on original)
    for (auto v : vecs) {
        v.scale(0.5); // This does not affect the original vector
    }

    std::cout << "\nAfter copy-based scaling (should be unchanged):" << std::endl;
    std::cout << vecs[0].to_csv_header() << std::endl;
    for (const auto& v : vecs) {
        std::cout << v.to_csv_row() << std::endl;
    }

    return 0;
}
