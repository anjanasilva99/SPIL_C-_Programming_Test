#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>

// Structure to hold the point data
struct Point {
    double x;
    double y;
};

// Structure to hold the shape data
struct Shape {
    std::string name;
    std::vector<Point> points;
};

// Function to calculate the area of a shape
double calculateArea(const Shape& shape) {
    double area = 0.0;
    int n = shape.points.size();
    for (int i = 0; i < n; ++i) {
        double x1 = shape.points[i].x;
        double y1 = shape.points[i].y;
        double x2 = shape.points[(i + 1) % n].x;
        double y2 = shape.points[(i + 1) % n].y;
        area += (x1 * y2) - (y1 * x2);
    }
    return std::abs(area) / 2.0;
}

// Function to calculate the bounding box of a shape
void getBoundingBox(const Shape& shape, double& minX, double& minY, double& maxX, double& maxY) {
    minX = minY = std::numeric_limits<double>::max();
    maxX = maxY = std::numeric_limits<double>::lowest();

    for (const auto& point : shape.points) {
        if (point.x < minX) minX = point.x;
        if (point.y < minY) minY = point.y;
        if (point.x > maxX) maxX = point.x;
        if (point.y > maxY) maxY = point.y;
    }
}

// Function to determine if two shapes overlap using bounding boxes
bool doShapesOverlapBoundingBox(const Shape& shape1, const Shape& shape2) {
    double minX1, minY1, maxX1, maxY1;
    double minX2, minY2, maxX2, maxY2;

    getBoundingBox(shape1, minX1, minY1, maxX1, maxY1);
    getBoundingBox(shape2, minX2, minY2, maxX2, maxY2);

    return !(minX1 > maxX2 || maxX1 < minX2 || minY1 > maxY2 || maxY1 < minY2);
}

// Check if a point is inside a polygon
bool isPointInPolygon(const Point& point, const std::vector<Point>& polygon) {
    int n = polygon.size();
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if ((polygon[i].y < point.y && polygon[j].y >= point.y ||
             polygon[i].y >= point.y && polygon[j].y < point.y) &&
            (polygon[i].x + (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) * (polygon[j].x - polygon[i].x) < point.x)) {
            inside = !inside;
        }
    }
    return inside;
}

// Overlap detection using point-in-polygon check
bool doShapesOverlap(const Shape& shape1, const Shape& shape2) {
    // 1. Basic bounding box check (same as before)
    if (!doShapesOverlapBoundingBox(shape1, shape2)) {
        return false;
    }

    // 2. Check if any point of shape1 is inside shape2 (and vice versa)
    for (const auto& point : shape1.points) {
        if (isPointInPolygon(point, shape2.points)) {
            return true;
        }
    }
    for (const auto& point : shape2.points) {
        if (isPointInPolygon(point, shape1.points)) {
            return true;
        }
    }

    // No intersection found after both checks
    return false;
}

int main() {
    std::vector<Shape> shapes = {
        {"SHAPE1", {{80, 70}, {50, 200}, {190, 240}, {270, 120}}},
        {"SHAPE2", {{130, 40}, {130, 250}, {300, 250}, {310, 110}, {240, 40}}},
        {"SHAPE3", {{100, 330}, {50, 420}, {270, 590}, {350, 490}}},
        {"SHAPE4", {{210, 300}, {90, 490}, {250, 460}}},
        {"SHAPE5", {{490, 210}, {460, 330}, {560, 380}, {590, 310}, {610, 230}}},
        {"SHAPE6", {{750, 110}, {840, 270}, {920, 150}}},
        {"SHAPE7", {{740, 310}, {570, 350}, {670, 470}, {930, 500}, {1040, 310}}},
        {"SHAPE8", {{440, 530}, {590, 560}, {660, 480}, {610, 390}, {400, 350}, {370, 410}}},
        {"SHAPE9", {{440, 50}, {440, 120}, {690, 140}, {690, 60}}},
        {"SHAPE10", {{710, 100}, {690, 190}, {780, 210}, {810, 160}}}
    };

    std::cout << "1. Calculate the area of each shape and print on the console along with shape name" << std::endl;
    // Calculate and print the area of each shape
    for (const auto& shape : shapes) {
        double area = calculateArea(shape);
        std::cout << shape.name << " Area: " << area << std::endl;
    }

    std::cout << "2. Print overlapping shape names on the console" << std::endl;
    // Check for overlapping shapes and print the results
    for (size_t i = 0; i < shapes.size(); ++i) {
        for (size_t j = i + 1; j < shapes.size(); ++j) {
            if (doShapesOverlap(shapes[i], shapes[j])) {
                std::cout << shapes[i].name << " overlaps with " << shapes[j].name << std::endl;
            }
        }
    }

    return 0;
}
