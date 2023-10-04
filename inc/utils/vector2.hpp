#pragma once

namespace engine::utils {
    /// <summary>
    /// Stores a bi-dimensional point of the specified type.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    struct Vector2 {
        Vector2()
            : x(0), y(0) {
        }

        Vector2(const Vector2<T>& rhs)
            : x(rhs.x), y(rhs.y) {
        }

        Vector2(T x, T y)
            : x(x), y(y) {
        }

        T x;
        T y;

        Vector2 abs() const {
            return { ::abs(x), ::abs(y) };
        }

        template <typename T>
        Vector2& operator+=(const Vector2<T>& r) {
            x += r.x;
            y += r.y;

            return *this;
        }

        template <typename T>
        Vector2& operator+=(const T& r) {
            x += r;
            y += r;

            return *this;
        }

        template <typename T>
        Vector2& operator-=(const Vector2<T>& r) {
            x -= r.x;
            y -= r.y;

            return *this;
        }

        template <typename T>
        Vector2& operator-=(const T& r) {
            x -= r;
            y -= r;

            return *this;
        }

        template <typename T>
        Vector2& operator/=(const Vector2<T>& r) {
            x /= r.x;
            y /= r.y;

            return *this;
        }

        template <typename T>
        Vector2& operator/=(const T& r) {
            x /= r;
            y /= r;

            return *this;
        }

        template <typename T>
        Vector2& operator*=(const Vector2<T>& r) {
            x *= r.x;
            y *= r.y;

            return *this;
        }

        template <typename T>
        Vector2& operator*=(const T& r) {
            x *= r;
            y *= r;

            return *this;
        }

        template <typename T>
        Vector2 operator+(const Vector2<T>& r) {
            x += r.x;
            y += r.y;

            return *this;
        }

        template <typename T>
        Vector2& operator+(const T& r) {
            x += r;
            y += r;

            return *this;
        }

        template <typename T>
        Vector2 operator-(const Vector2<T>& r) {
            x -= r.x;
            y -= r.y;

            return *this;
        }

        template <typename T>
        Vector2& operator-(const T& r) {
            x -= r;
            y -= r;

            return *this;
        }

        template <typename T>
        Vector2 operator/(const Vector2<T>& r) {
            x /= r.x;
            y /= r.y;

            return *this;
        }

        template <typename T>
        Vector2& operator/(const T& r) {
            x /= r;
            y /= r;

            return *this;
        }

        template <typename T>
        Vector2<T> operator*(const Vector2<T>& r) {
            x *= r.x;
            y *= r.y;

            return *this;
        }

        template <typename T>
        Vector2& operator*(const T& r) {
            x *= r;
            y *= r;

            return *this;
        }
    };

    using IVector2 = Vector2<int>;
    using FVector2 = Vector2<float>;
}