/*
Copyright (c) 2010, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _PEAKENGINE_SUPPORT_MATRIX4_HPP_
#define _PEAKENGINE_SUPPORT_MATRIX4_HPP_

#include "Vector3.hpp"

namespace peak
{
	class Matrix4
	{
		public:
			Matrix4()
			{
				m[0][0] = 1;
				m[0][1] = 0;
				m[0][2] = 0;
				m[0][3] = 0;

				m[1][0] = 0;
				m[1][1] = 1;
				m[1][2] = 0;
				m[1][3] = 0;

				m[2][0] = 0;
				m[2][1] = 0;
				m[2][2] = 1;
				m[2][3] = 0;

				m[3][0] = 0;
				m[3][1] = 0;
				m[3][2] = 0;
				m[3][3] = 1;
			}
			Matrix4(const Matrix4 &other)
			{
				for (unsigned int i = 0; i < 16; i++)
				{
					x[i] = other.x[i];
				}
			}
			Matrix4(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33)
			{
				m[0][0] = m00;
				m[0][1] = m01;
				m[0][2] = m02;
				m[0][3] = m03;

				m[1][0] = m10;
				m[1][1] = m11;
				m[1][2] = m12;
				m[1][3] = m13;

				m[2][0] = m20;
				m[2][1] = m21;
				m[2][2] = m22;
				m[2][3] = m23;

				m[3][0] = m30;
				m[3][1] = m31;
				m[3][2] = m32;
				m[3][3] = m33;
			}
			static Matrix4 TransMat(const Vector3F &translation)
			{
				float x = translation.x;
				float y = translation.y;
				float z = translation.z;
				return Matrix4(1, 0, 0, 0,
				               0, 1, 0, 0,
				               0, 0, 1, 0,
				               x, y, z, 1);
			}
			static Matrix4 ScaleMat(const Vector3F &scale)
			{
				float x = scale.x;
				float y = scale.y;
				float z = scale.z;
				return Matrix4(x, 0, 0, 0,
				               0, y, 0, 0,
				               0, 0, z, 0,
				               0, 0, 0, 1);
			}

			Matrix4 operator*(const Matrix4 &o)
			{
				return Matrix4(m[0][0] * o.m[0][0] + m[1][0] * o.m[0][1] + m[2][0] * o.m[0][2] + m[3][0] * o.m[0][3],
				               m[0][1] * o.m[0][0] + m[1][1] * o.m[0][1] + m[2][1] * o.m[0][2] + m[3][1] * o.m[0][3],
				               m[0][2] * o.m[0][0] + m[1][2] * o.m[0][1] + m[2][2] * o.m[0][2] + m[3][2] * o.m[0][3],
				               m[0][3] * o.m[0][0] + m[1][3] * o.m[0][1] + m[2][3] * o.m[0][2] + m[3][3] * o.m[0][3],

				               m[0][0] * o.m[1][0] + m[1][0] * o.m[1][1] + m[2][0] * o.m[1][2] + m[3][0] * o.m[1][3],
				               m[0][1] * o.m[1][0] + m[1][1] * o.m[1][1] + m[2][1] * o.m[1][2] + m[3][1] * o.m[1][3],
				               m[0][2] * o.m[1][0] + m[1][2] * o.m[1][1] + m[2][2] * o.m[1][2] + m[3][2] * o.m[1][3],
				               m[0][3] * o.m[1][0] + m[1][3] * o.m[1][1] + m[2][3] * o.m[1][2] + m[3][3] * o.m[1][3],

				               m[0][0] * o.m[2][0] + m[1][0] * o.m[2][1] + m[2][0] * o.m[2][2] + m[3][0] * o.m[2][3],
				               m[0][1] * o.m[2][0] + m[1][1] * o.m[2][1] + m[2][1] * o.m[2][2] + m[3][1] * o.m[2][3],
				               m[0][2] * o.m[2][0] + m[1][2] * o.m[2][1] + m[2][2] * o.m[2][2] + m[3][2] * o.m[2][3],
				               m[0][3] * o.m[2][0] + m[1][3] * o.m[2][1] + m[2][3] * o.m[2][2] + m[3][3] * o.m[2][3],

				               m[0][0] * o.m[3][0] + m[1][0] * o.m[3][1] + m[2][0] * o.m[3][2] + m[3][0] * o.m[3][3],
				               m[0][1] * o.m[3][0] + m[1][1] * o.m[3][1] + m[2][1] * o.m[3][2] + m[3][1] * o.m[3][3],
				               m[0][2] * o.m[3][0] + m[1][2] * o.m[3][1] + m[2][2] * o.m[3][2] + m[3][2] * o.m[3][3],
				               m[0][3] * o.m[3][0] + m[1][3] * o.m[3][1] + m[2][3] * o.m[3][2] + m[3][3] * o.m[3][3]);
			}

			union
			{
				float m[4][4];
				float x[16];
			};
	};
}

#endif
