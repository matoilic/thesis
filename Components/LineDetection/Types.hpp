#ifndef TYPES_HPP
#define TYPES_HPP

#include <complex>
#include <vector>
#include <opencv2/core/core.hpp>


typedef ARD_EXPORT struct Point
{
    int x;
    int y;

    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}

    bool equals(Point &point)
    {
        return point.x == x && point.y == y;
    }

    double calculateDistanceTo(const Point &other) const
    {
        int dx = x - other.x;
        int dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    cv::Vec2i toVector() const
    {
        return cv::Vec2i(x, y);
    }

} Point;


typedef ARD_EXPORT struct LineEquation
{
    cv::Vec2i point;
    cv::Vec2i direction;

    LineEquation(cv::Vec2i point, cv::Vec2i direction) :  point(point), direction(direction) {}

} LineEquation;


typedef ARD_EXPORT struct LineSegment
{
    Point start;
    Point end;
    std::vector<LineSegment*> *concatenation;

    LineSegment(Point from, Point to) : start(from), end(to), concatenation(nullptr) {}

    bool equals(LineSegment &segment)
    {
        return start.equals(segment.start) && end.equals(segment.end);
    }

    double getLength() const
    {
        return start.calculateDistanceTo(end);
    }

    cv::Vec2i toVector() const
    {
        return end.toVector() - start.toVector();
    }

    LineEquation getLineEquation() const
    {
        return LineEquation(start.toVector(), toVector());
    }

    double getAngle(const LineSegment &other) const
    {
        cv::Vec2i a = toVector();
        cv::Vec2i b = other.toVector();
        double absA = std::sqrt(a(0) * a(0) + a(1) * a(1));
        double absB = std::sqrt(b(0) * b(0) + b(1) * b(1));
        return a.dot(b) / (absA * absB);
    }

} LineSegment;


typedef ARD_EXPORT struct Rectangle
{
    int x;
    int y;
    int width;
    int height;

    Rectangle() : x(0), y(0), width(0), height(0) {}
    Rectangle(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

    bool contains(Point &point)
    {
        return (point.x >= x)
            && (point.y >= y)
            && (point.x <= x + width)
            && (point.y <= y + height);
    }

    bool intersects(Rectangle &rectangle)
    {
        return (x + width > rectangle.x)
            && (rectangle.x + rectangle.width > x)
            && (y + height > rectangle.y)
            && (rectangle.y + rectangle.height > y);
    }
} Rectangle;


typedef ARD_EXPORT std::vector<LineSegment> LineSegmentList;


#endif // TYPES_HPP
