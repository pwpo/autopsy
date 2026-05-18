#pragma once

#include <vector>
#include <string>
#include <complex>

namespace sdk
{
    struct vector2 final {
        float x{ 0.0f }, y{ 0.0f };

        vector2 operator+(const vector2& rhs) const {
            return { x + rhs.x, y + rhs.y };
        }
        vector2 operator-(const vector2& rhs) const {
            return { x - rhs.x, y - rhs.y };
        }
        vector2 operator*(float scalar) const {
            return { x * scalar, y * scalar };
        }
        vector2& operator+=(const vector2& rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }
        vector2& operator-=(const vector2& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        float distance(const vector2& other) const {
            float dx = x - other.x;
            float dy = y - other.y;
            return std::sqrt(dx * dx + dy * dy);
        }
    };

    struct vector3 final {

        float x{ 0.0f }, y{ 0.0f }, z{ 0.0f };

        vector3 operator+(const vector3& rhs) const {
            return { x + rhs.x, y + rhs.y, z + rhs.z };
        }

        vector3 operator-(const vector3& rhs) const {
            return { x - rhs.x, y - rhs.y, z - rhs.z };
        }

        vector3 operator*(float scalar) const {
            return { x * scalar, y * scalar, z * scalar };
        }

        vector3 operator*(const vector3& other) const {
            return { x * other.x, y * other.y, z * other.z };
        }

        vector3 operator/(const vector3& rhs) const {
            return { x / rhs.x, y / rhs.y, z / rhs.z };
        }

        vector3& operator+=(const vector3& rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        vector3& operator-=(const vector3& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        bool operator==(const vector3& other) const {
            return (x == other.x && y == other.y && z == other.z);
        }

        vector3 operator/(float scalar) const {
            return { x / scalar, y / scalar, z / scalar };
        }

        vector3 operator-() const {
            return { -x, -y, -z };
        }

        const float magnitude() const {
            return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
        }

        const float distance(vector3 vector) const {
            return (*this - vector).magnitude();
        }

        const vector3 normalize() const {
            vector3 ret;
            float mag = magnitude();
            ret.x = x / mag;
            ret.y = y / mag;
            ret.z = z / mag;
            return ret;
        }

        auto cross(vector3 vec) const {
            vector3 ret;
            ret.x = y * vec.z - z * vec.y;
            ret.y = -(x * vec.z - z * vec.x);
            ret.z = x * vec.y - y * vec.x;
            return ret;
        }
    };

    struct matrix4 final { float data[16]; };

    struct matrix3 final {
        float data[9];

        vector3 operator*(const vector3& v) const {
            return {
                data[0] * v.x + data[1] * v.y + data[2] * v.z,
                data[3] * v.x + data[4] * v.y + data[5] * v.z,
                data[6] * v.x + data[7] * v.y + data[8] * v.z
            };
        }
    };

    struct vector4 final { float x, y, z, w; };
}
