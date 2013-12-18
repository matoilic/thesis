#ifndef POINTQUADTREE_H
#define POINTQUADTREE_H

#include <vector>
#include "Types.hpp"

template<class T>
class ARD_EXPORT PointQuadTree
{
private:
    Rectangle bounds;
    Point *splitPoint;

    std::vector<T> contents;

    PointQuadTree<T> *northWest;
    PointQuadTree<T> *northEast;
    PointQuadTree<T> *southWest;
    PointQuadTree<T> *southEast;

    void init(Rectangle bounds)
    {
        this->bounds = bounds;
        splitPoint = nullptr;
        northWest = nullptr;
        northEast = nullptr;
        southWest = nullptr;
        southEast = nullptr;
    }

    void split()
    {
        northWest = new PointQuadTree<T>(bounds.x, bounds.y, splitPoint->x - bounds.x, splitPoint->y - bounds.y);
        northEast = new PointQuadTree<T>(splitPoint->x + 1, bounds.y, bounds.x + bounds.width - splitPoint->x - 1, splitPoint->y - bounds.y);
        southWest = new PointQuadTree<T>(bounds.x, splitPoint->y + 1, splitPoint->x - bounds.x, bounds.y + bounds.height - splitPoint->y - 1);
        southEast = new PointQuadTree<T>(splitPoint->x + 1, splitPoint->y + 1, bounds.x + bounds.width - splitPoint->x - 1, bounds.y + bounds.height - splitPoint->y - 1);
    }

public:
    PointQuadTree(Rectangle bounds) { init(bounds); }
    PointQuadTree(int x, int y, int width, int height) { init(Rectangle(x, y, width, height)); }
    PointQuadTree(int width, int height) { init(Rectangle(0, 0, width, height)); }

    ~PointQuadTree()
    {
        if (splitPoint != nullptr) {
            delete splitPoint;
        }

        if (northWest != nullptr) {
            delete northEast;
            delete northWest;
            delete southEast;
            delete southWest;
        }
    }

    bool insert(int x, int y, T item)
    {
        return insert(Point(x, y), item);
    }

    bool insert(Point point, T item)
    {
        // Ignore objects which do not belong in this quad tree
        if (!bounds.contains(point))
            return false;

        if (splitPoint == nullptr) {
            splitPoint = new Point(point.x, point.y);
        }

        // If there is space in this quad tree, add the object here
        if (splitPoint->equals(point)) {
            contents.push_back(item);
            return true;
        }

        // Otherwise, we need to subdivide then add the point to whichever node will accept it
        if (northWest == nullptr)
            split();

        if (northWest->insert(point, item)) return true;
        if (northEast->insert(point, item)) return true;
        if (southWest->insert(point, item)) return true;
        if (southEast->insert(point, item)) return true;

        // Otherwise, the point cannot be inserted for some unknown reason (which should never happen)
        return false;
    }

    std::vector<T> query(int x, int y, int width, int height)
    {
        return query(Rectangle(x, y, width, height));
    }

    std::vector<T> query(Rectangle &area)
    {
        // create a list of the items that are found
        std::vector<T> results;

        // Automatically abort if the range does not collide with this quad
        if (!bounds.intersects(area) || splitPoint == nullptr)
          return results; // empty list

        if (area.contains(*splitPoint)) {
            results.insert(results.end(), contents.begin(), contents.end());
        }

        // Terminate here, if there are no children
        if (northWest == nullptr)
            return results;

        // get children results
        std::vector<T> northWestResults = northWest->query(area);
        std::vector<T> northEastResults = northEast->query(area);
        std::vector<T> southWestResults = southWest->query(area);
        std::vector<T> southEastResults = southEast->query(area);

        results.insert(results.end(), northWestResults.begin(), northWestResults.end());
        results.insert(results.end(), northEastResults.begin(), northEastResults.end());
        results.insert(results.end(), southWestResults.begin(), southWestResults.end());
        results.insert(results.end(), southEastResults.begin(), southEastResults.end());

        return results;
    }

};

#endif // POINTQUADTREE_H
