#pragma once

#include <iostream>
#include <list>
#include <utility>
#include <vector>

namespace gsml
{

typedef std::pair<double, double> Point2D; // Erstatter Vector2D klasse
// Template klasse, laget for ID og T
// ID er ikke implementert ennå
// Bruker Point2D - se typedef ovenfor - i stedet for Vector2D
template <class ID, class T>
class QuadTree
{
public:
    QuadTree();
    QuadTree(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D & v4);
    void init(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D & v4);
    void subDivide(int n);
    void print() const;
    void print_all() const;
    QuadTree* insert(const Point2D& point, const ID& id, const T& object);
    QuadTree* find(const Point2D& p);
    const QuadTree* find(const Point2D& p) const;
    T& find(const ID& id);
    T& operator[] (ID id);

    // Hvert quad lagrer sine egne objekter
    // (T kan være objekter, pekere eller referanser)
    // Legger opp til mulig søk på id
    std::list<std::pair<ID,T>> m_objects;

    // Hjelpevariabel for å lagre alle T-ene
    // for senere iterering
    std::vector<T> m_all_objects; // skulle vært static

    // Jukser litt med disse iteratorene. Det er ikke ekte iteratorfunksjoner
    // for QuadTre klassen, men for en container av T-er
    typename std::vector<T>::iterator begin()
    {
        traverse_all(m_all_objects);
        return m_all_objects.begin();
    }
    typename std::vector<T>::iterator end()
    {
        return m_all_objects.end();
    }

private:
    // Punkter som definerer hjørnene til objektet
    // i samme rekkefølge som pekerne til subtrær
    Point2D m_a;
    Point2D m_b;
    Point2D m_c;
    Point2D m_d;
    // Pekere til subtrær er nødvendig i en dynamisk
    // quadtre implementering. Vi kunne ha lagt det
    // i array også
    QuadTree* m_sw = nullptr;
    QuadTree* m_se = nullptr;
    QuadTree* m_ne = nullptr;
    QuadTree* m_nw = nullptr;

    bool isLeaf() const;

    // Hjelpefunksjon - preorder traversering av subtre
    void traverse_all(std::vector<T>& all_objects);

};

template<class ID, class T>
bool QuadTree<ID, T>::isLeaf() const
{
    return m_sw == nullptr && m_se == nullptr && m_ne == nullptr && m_nw == nullptr;
}

template<class ID, class T>
void QuadTree<ID, T>::traverse_all(std::vector<T> &all_objects)
{

}

template<class ID, class T>
QuadTree<ID, T>::QuadTree()
{

}

template<class ID, class T>
QuadTree<ID, T>::QuadTree(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4)
    : m_a{v1}, m_b{v2}, m_c{v3}, m_d{v4}, m_sw{nullptr}, m_se{nullptr}, m_nw{nullptr}, m_ne{nullptr}
{
    //
}

template<class ID, class T>
void QuadTree<ID, T>::init(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4)
{
    m_a = v1; m_b = v2; m_c = v3; m_d = v4;
}

template<class ID, class T>
void QuadTree<ID, T>::subDivide(int n)
{
    if (n>0)
    {
        Point2D v1 = Point2D{(m_a.first + m_b.first) / 2, (m_a.second + m_b.second) / 2};
        Point2D v2 = Point2D{(m_b.first + m_c.first) / 2, (m_b.second + m_c.second) / 2};
        Point2D v3 = Point2D{(m_c.first + m_d.first) / 2, (m_c.second + m_d.second) / 2};
        Point2D v4 = Point2D{(m_d.first + m_a.first) / 2, (m_d.second + m_a.second) / 2};
        Point2D m = Point2D{(m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2};

        m_sw = new QuadTree(m_a, v1, m, v4);
        m_sw->subDivide(n - 1);
        m_se = new QuadTree(m_a, v1, m, v4);
        m_se->subDivide(n - 1);
        m_ne = new QuadTree(m_a, v1, m, v4);
        m_ne->subDivide(n - 1);
        m_nw = new QuadTree(m_a, v1, m, v4);
        m_nw->subDivide(n - 1);
    }
}

template<class ID, class T>
void QuadTree<ID, T>::print() const
{
    Point2D c = Point2D{(m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2};

    std::cout << "senter = (" << c.first << ", " << c.second << ")" << std::endl;
    for (auto& object : m_objects)
    {
        std::cout << object.first << ", " << object.second->m_navn << std::endl;
    }

}

template<class ID, class T>
void QuadTree<ID, T>::print_all() const
{
    print();
    if (m_ne) m_ne->print_all();
    if (m_nw) m_nw->print_all();
    if (m_sw) m_sw->print_all();
    if (m_se) m_se->print_all();
}

template<class ID, class T>
QuadTree<ID, T> *QuadTree<ID, T>::insert(const Point2D &point, const ID &id, const T &object)
{
    if (isLeaf())
    {
        m_objects.push_back(std::pair<ID, T>{id, object});
        return this;
    }
    else
    {
        Point2D m = Point2D{(m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2};

        if (point.second < m.second)
        {
            if (point.first < m.first)
                m_sw->insert(point, id, object);
            else
                m_se->insert(point, id, object);
        }
        else
        {
            if (point.first < m.first)
                m_nw->insert(point, id, object);
            else
                m_ne->insert(point, id, object);
        }
    }
}

template<class ID, class T>
QuadTree<ID, T> *QuadTree<ID, T>::find(const Point2D &p)
{
    if (isLeaf())
        return this;

    else
    {
        Point2D m = Point2D{(m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2};

        if (p.second < m.second)
        {
            if (p.first < m.first)
                m_sw->find(p);
            else
                m_se->find(p);
        }
        else
        {
            if (p.first < m.first)
                m_nw->find(p);
            else
                m_ne->find(p);
        }
    }
}

template<class ID, class T>
const QuadTree<ID, T> *QuadTree<ID, T>::find(const Point2D &p) const
{
    if (isLeaf())
        return this;

    else
    {
        Point2D m = Point2D{(m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2};

        if (p.second < m.second)
        {
            if (p.first < m.first)
                m_sw->find(p);
            else
                m_se->find(p);
        }
        else
        {
            if (p.first < m.first)
                m_nw->find(p);
            else
                m_ne->find(p);
        }
    }
}

template<class ID, class T>
T &QuadTree<ID, T>::find(const ID &id)
{

}

} // namespace gsml





