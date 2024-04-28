#pragma once

#include <cmath>
#include <glm/vec2.hpp>

namespace engine::utils {
#pragma pack(push, 1)

    /// <summary>
    /// Stores a bi-dimensional point of the specified type.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template<typename T>
    struct Vector2 {
        Vector2()
                : x(0), y(0) {
        }

        Vector2(const Vector2<T> &rhs)
                : x(rhs.x), y(rhs.y) {
        }

        Vector2(T x, T y)
                : x(x), y(y) {
        }

        T x;
        T y;

        static size_t get_singular_size() {
            return sizeof(T);
        }

        Vector2 abs() const {
            return {::abs(x), ::abs(y)};
        }

        template<typename U>
        Vector2 &operator+=(const Vector2<U> &r) {
            x += r.x;
            y += r.y;

            return *this;
        }

        template<typename U>
        Vector2 &operator+=(const U &r) {
            x += r;
            y += r;

            return *this;
        }

        template<typename U>
        Vector2 &operator-=(const Vector2<U> &r) {
            x -= r.x;
            y -= r.y;

            return *this;
        }

        template<typename U>
        Vector2 &operator-=(const U &r) {
            x -= r;
            y -= r;

            return *this;
        }

        template<typename U>
        Vector2 &operator/=(const Vector2<U> &r) {
            x /= r.x;
            y /= r.y;

            return *this;
        }

        template<typename U>
        Vector2 &operator/=(const U &r) {
            x /= r;
            y /= r;

            return *this;
        }

        template<typename U>
        Vector2 &operator*=(const Vector2<U> &r) {
            x *= r.x;
            y *= r.y;

            return *this;
        }

        template<typename U>
        Vector2 &operator*=(const U &r) {
            x *= r;
            y *= r;

            return *this;
        }

        template<typename U>
        Vector2 operator+(const Vector2<U> &r) {
            x += r.x;
            y += r.y;

            return *this;
        }

        template<typename U>
        Vector2 &operator+(const U &r) {
            x += r;
            y += r;

            return *this;
        }

        template<typename U>
        Vector2 operator-(const Vector2<U> &r) {
            x -= r.x;
            y -= r.y;

            return *this;
        }

        template<typename U>
        Vector2 &operator-(const U &r) {
            x -= r;
            y -= r;

            return *this;
        }

        template<typename U>
        Vector2 operator/(const Vector2<U> &r) {
            x /= r.x;
            y /= r.y;

            return *this;
        }

        template<typename U>
        Vector2 &operator/(const U &r) {
            x /= r;
            y /= r;

            return *this;
        }

        template<typename U>
        Vector2<U> operator*(const Vector2<U> &r) {
            x *= r.x;
            y *= r.y;

            return *this;
        }

        template<typename U>
        Vector2 &operator*(const U &r) {
            x *= r;
            y *= r;

            return *this;
        }

        bool operator==(const Vector2 &r) {
            return x == r.x && y == r.y;
        }

        bool operator!=(const Vector2 &r) {
            return x != r.x || y != r.y;
        }

        glm::vec2 to_glm_vec() {
            return {
                    x,
                    y
            };
        }
    };

#pragma pack(pop)

    using IVector2 = Vector2<int>;
    using FVector2 = Vector2<float>;
}