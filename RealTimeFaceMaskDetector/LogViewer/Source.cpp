template<typename T>
class Vector;

template<typename T>
class Point {
public:
    Point(T x_, T y_) : x{ x_ }, y{ y_ } {}
    Vector<T> operator-(Point tmp) {
        T x_v = x - tmp.x;
        T y_v = y - tmp.y;
        return Vector(x_v, y_v);
    }
private:
    T x;
    T y;
};


template<typename T>
class Vector {
public:
    Vector(T x_, T y_) : x{ x_ }, y{ y_ } {}
private:
    T x;
    T y;
};