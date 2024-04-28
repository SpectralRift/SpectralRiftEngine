#pragma once

#include <cmath>
#include <glm/vec3.hpp>

namespace engine::utils {
#pragma pack(push, 1)

    /// <summary>
    /// Stores a tri-dimensional point of the specified type.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template<typename T>
    struct Vector3 {
        Vector3()
                : x(0), y(0), z(0) {
        }

        Vector3(const Vector3<T> &rhs)
                : x(rhs.x), y(rhs.y), z(rhs.z) {
        }

        Vector3(T x, T y, T z)
                : x(x), y(y), z(z) {
        }

        T x;
        T y;
        T z;

        static size_t get_singular_size() {
            return sizeof(T);
        }

        Vector3 abs() const {
            return {::abs(x), ::abs(y), ::abs(z)};
        }

        template<typename U>
        Vector3 &operator+=(const Vector3<U> &r) {
            x += r.x;
            y += r.y;
            z += r.z;

            return *this;
        }

        template<typename U>
        Vector3 &operator+=(const U &r) {
            x += r;
            y += r;
            z += r;

            return *this;
        }

        template<typename U>
        Vector3 &operator-=(const Vector3<U> &r) {
            x -= r.x;
            y -= r.y;
            z -= r.z;

            return *this;
        }

        template<typename U>
        Vector3 &operator-=(const U &r) {
            x -= r;
            y -= r;
            z -= r;

            return *this;
        }

        template<typename U>
        Vector3 &operator/=(const Vector3<U> &r) {
            x /= r.x;
            y /= r.y;
            z /= r.z;

            return *this;
        }

        template<typename U>
        Vector3 &operator/=(const U &r) {
            x /= r;
            y /= r;
            z /= r;

            return *this;
        }

        template<typename U>
        Vector3 &operator*=(const Vector3<U> &r) {
            x *= r.x;
            y *= r.y;
            z *= r.z;

            return *this;
        }

        template<typename U>
        Vector3 &operator*=(const U &r) {
            x *= r;
            y *= r;
            z *= r;

            return *this;
        }

        template<typename U>
        Vector3 operator+(const Vector3<U> &r) {
            x += r.x;
            y += r.y;
            z += r.z;

            return *this;
        }

        template<typename U>
        Vector3 &operator+(const U &r) {
            x += r;
            y += r;
            z += r;

            return *this;
        }

        template<typename U>
        Vector3 operator-(const Vector3<U> &r) {
            x -= r.x;
            y -= r.y;
            z -= r.z;

            return *this;
        }

        template<typename U>
        Vector3 &operator-(const U &r) {
            x -= r;
            y -= r;
            z -= r;

            return *this;
        }

        template<typename U>
        Vector3 operator/(const Vector3<U> &r) {
            x /= r.x;
            y /= r.y;
            z /= r.z;

            return *this;
        }

        template<typename U>
        Vector3 &operator/(const U &r) {
            x /= r;
            y /= r;
            z /= r;

            return *this;
        }

        template<typename U>
        Vector3<U> operator*(const Vector3<U> &r) {
            x *= r.x;
            y *= r.y;
            z *= r.z;

            return *this;
        }

        template<typename U>
        Vector3 &operator*(const U &r) {
            x *= r;
            y *= r;
            z *= r;

            return *this;
        }

        glm::vec3 to_glm_vec() {
            return {
                x,
                y,
                z
            };
        }
    };

#pragma pack(pop)

    using IVector3 = Vector3<int>;
    using FVector3 = Vector3<float>;
}