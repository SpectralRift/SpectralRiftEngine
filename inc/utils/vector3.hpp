#pragma once

namespace engine::utils {
	/// <summary>
	/// Stores a tri-dimensional point of the specified type.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	struct Vector3 {
		Vector3()
			: x(0), y(0), z(0) {
		}

		Vector3(const Vector3<T>& rhs)
			: x(rhs.x), y(rhs.y), z(rhs.z) {
		}

		Vector3(T x, T y, T z)
			: x(x), y(y), z(z) {
		}

		T x;
		T y;
		T z;

		Vector3 abs() const {
			return { ::abs(x), ::abs(y), ::abs(z) };
		}

		template <typename _T>
		Vector3& operator+=(const Vector3<_T>& r) {
			x += r.x;
			y += r.y;
			z += r.z;

			return *this;
		}

		template <typename _T>
		Vector3& operator+=(const _T& r) {
			x += r;
			y += r;
			z += r;

			return *this;
		}

		template <typename _T>
		Vector3& operator-=(const Vector3<_T>& r) {
			x -= r.x;
			y -= r.y;
			z -= r.z;

			return *this;
		}

		template <typename _T>
		Vector3& operator-=(const _T& r) {
			x -= r;
			y -= r;
			z -= r;

			return *this;
		}

		template <typename _T>
		Vector3& operator/=(const Vector3<_T>& r) {
			x /= r.x;
			y /= r.y;
			z /= r.z;

			return *this;
		}

		template <typename _T>
		Vector3& operator/=(const _T& r) {
			x /= r;
			y /= r;
			z /= r;

			return *this;
		}

		template <typename _T>
		Vector3& operator*=(const Vector3<_T>& r) {
			x *= r.x;
			y *= r.y;
			z *= r.z;

			return *this;
		}

		template <typename _T>
		Vector3& operator*=(const _T& r) {
			x *= r;
			y *= r;
			z *= r;

			return *this;
		}

		template <typename _T>
		Vector3 operator+(const Vector3<_T>& r) {
			x += r.x;
			y += r.y;
			z += r.z;

			return *this;
		}

		template <typename _T>
		Vector3& operator+(const _T& r) {
			x += r;
			y += r;
			z += r;

			return *this;
		}

		template <typename _T>
		Vector3 operator-(const Vector3<_T>& r) {
			x -= r.x;
			y -= r.y;
			z -= r.z;

			return *this;
		}

		template <typename _T>
		Vector3& operator-(const _T& r) {
			x -= r;
			y -= r;
			z -= r;

			return *this;
		}

		template <typename _T>
		Vector3 operator/(const Vector3<_T>& r) {
			x /= r.x;
			y /= r.y;
			z /= r.z;

			return *this;
		}

		template <typename _T>
		Vector3& operator/(const _T& r) {
			x /= r;
			y /= r;
			z /= r;

			return *this;
		}

		template <typename _T>
		Vector3<T> operator*(const Vector3<_T>& r) {
			x *= r.x;
			y *= r.y;
			z *= r.z;

			return *this;
		}

		template <typename _T>
		Vector3& operator*(const _T& r) {
			x *= r;
			y *= r;
			z *= r;

			return *this;
		}
	};

	using IVector3 = Vector3<int>;
	using FVector3 = Vector3<float>;
}