//*******************************************************************
// Terms of Use and Disclaimer
// Copyright 2011-2040 Sungkil Lee
//
// This software is owned by Sungkil Lee.
// You may use and modify this software for non-commercial and commercial software,
// but re-distribution of this software is strictly prohibited.
//*******************************************************************

#ifndef __CGMATH_H__
#define __CGMATH_H__

#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif
// C standard
#include <float.h>
#include <inttypes.h>	// defines int64_t, uint64_t
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// STL
#include <algorithm>
#include <array>
#include <map>
#include <set>
#include <string>
#include <vector>
// C++11
#if (__cplusplus>199711L) || (_MSC_VER>=1600/*VS2010*/)
	#include <type_traits>
	#include <unordered_map>
	#include <unordered_set>
#endif
// windows/GCC
#if !defined(__GNUC__)&&(defined(_WIN32)||defined(_WIN64))
	#ifndef NOMINMAX
		#define NOMINMAX // suppress definition of min/max
	#endif
	#include <windows.h>
	#include <wchar.h>
	// disable excessive warnings
	#pragma warning( disable:4100 )	// unreferenced formal parameter
	#pragma warning( disable:4201 )	// nameless struct/union
	#pragma warning( disable:4459 )	// hiding global declarations
	#pragma warning( disable:4706 )	// assignment within conditional expression
	#pragma warning( disable:4710 )	// functions are not inlined

#elif defined(__GNUC__)&&!defined(__forceinline)
	#define __forceinline inline __attribute__((__always_inline__))
#endif
// common macros
#ifndef PI
	#define PI 3.141592653589793f
#endif
#if defined(__GNUC__)&&__cplusplus<201402L
	#error legacy C++11 is not used. Use -std=c++17 (or -std=c++14)
#endif

//*******************************************************************
// user types
using uint = unsigned int;
using ushort = unsigned short;
using uchar = unsigned char;

// template constants
template <class T> struct precision { static const T value(){ return std::numeric_limits<T>::epsilon()*10; } };	// need to be 10x for robust practical test

// template type_traits helpers
template <class T> using enable_signed_t = typename std::enable_if_t<std::is_signed<T>::value,T>;
template <class T> using enable_float_t	 = typename std::enable_if_t<std::is_floating_point<T>::value,T>;
#define signed_memfun(U) template <class X=T, typename U=enable_signed_t<X>>
#define float_memfun(U)	 template <class X=T, typename U=enable_float_t<X>>

//*******************************************************************
template <class T> struct tvec2
{
	union{ struct { T x, y; }; struct { T r, g; }; struct { T s, t; }; };

	// constructor/set
	tvec2(){ x=y=0; }
	tvec2( T a ){ x=y=a; }						inline void set( T a ){ x=y=a; }
	tvec2( T a, T b ){ x=a;y=b; }				inline void set( T a, T b ){ x=a;y=b; }
	tvec2( const tvec2& v ){ x=v.x;y=v.y; }		inline void set( const tvec2& v ){ x=v.x;y=v.y; }

	// assignment / compound assignment operators
	inline tvec2& operator=( T a ){ set(a); return *this; }
	inline tvec2& operator+=( const tvec2& v ){ x+=v.x; y+=v.y; return *this; }
	inline tvec2& operator-=( const tvec2& v ){ x-=v.x; y-=v.y; return *this; }
	inline tvec2& operator*=( const tvec2& v ){ x*=v.x; y*=v.y; return *this; }
	inline tvec2& operator/=( const tvec2& v ){ x/=v.x; y/=v.y; return *this; }
	inline tvec2& operator+=( T a ){ x+=a; y+=a; return *this; }
	inline tvec2& operator-=( T a ){ x-=a; y-=a; return *this; }
	inline tvec2& operator*=( T a ){ x*=a; y*=a; return *this; }
	inline tvec2& operator/=( T a ){ x/=a; y/=a; return *this; }

	// comparison operators
	inline bool operator==( const tvec2& v ) const { return std::abs(x-v.x)<=precision<T>::value()&&std::abs(y-v.y)<=precision<T>::value(); }
	inline bool operator!=( const tvec2& v ) const { return !operator==(v); }

	// casting operators
	inline operator T*(){ return &x; }
	inline operator const T*() const { return &x; }

	// array access operators
	inline T& operator[]( ptrdiff_t i ){ return (&r)[i]; }
	inline const T& operator[]( ptrdiff_t i ) const { return (&r)[i]; }
	inline T& at( ptrdiff_t i ){ return (&r)[i]; }
	inline const T& at( ptrdiff_t i ) const { return (&r)[i]; }

	// unary operators
	inline tvec2 operator+() const { return tvec2(r, g); }
	inline tvec2 operator-() const { return tvec2(-r, -g); }

	// binary operators
	inline tvec2 operator+( const tvec2& v ) const { return tvec2(r+v.r, g+v.g); }
	inline tvec2 operator-( const tvec2& v ) const { return tvec2(r-v.r, g-v.g); }
	inline tvec2 operator*( const tvec2& v ) const { return tvec2(r*v.r, g*v.g); }
	inline tvec2 operator/( const tvec2& v ) const { return tvec2(r/v.r, g/v.g);  }
	inline tvec2 operator+( T a ) const { return tvec2(r+a, g+a); }
	inline tvec2 operator-( T a ) const { return tvec2(r-a, g-a); }
	inline tvec2 operator*( T a ) const { return tvec2(r*a, g*a); }
	inline tvec2 operator/( T a ) const { return tvec2(r/a, g/a); }

	// length, normalize, dot product
	float_memfun(U) inline T length() const { return (T)(sqrt(r*r+g*g)); }
	float_memfun(U) inline T dot( const tvec2& v ) const { return (T)(r*v.r+g*v.g); }
	float_memfun(U) inline tvec2 normalize() const { return tvec2(r, g)/length(); }
	float_memfun(U) inline T length2() const { return (T)(r*r+g*g); }
};

//*******************************************************************
template <class T> struct tvec3
{
	union { struct { T x, y, z; }; struct { T r, g, b; }; struct { T s, t, p; }; };

	// constructor/set
	tvec3(){ x=y=z=0; }
	tvec3( T a ){ x=y=z=a; }								inline void set( T a ){ x=y=z=a; }
	tvec3( T a, T b, T c ){ x=a;y=b;z=c; }					inline void set( T a, T b, T c ){ x=a;y=b;z=c; }
	tvec3( const tvec3& v ){ x=v.x;y=v.y;z=v.z; }			inline void set( const tvec3& v ){ x=v.x;y=v.y;z=v.z; }
	tvec3( const tvec2<T>& v, T c ){ x=v.x;y=v.y;z=c; }		inline void set( const tvec2<T>& v, T c ){ x=v.x;y=v.y;z=c; }
	tvec3( T a, const tvec2<T>& v ){ x=a;y=v.x;z=v.y; }		inline void set( T a, const tvec2<T>& v ){ x=a;y=v.x;z=v.y; }

	// assignment / compound assignment operators
	inline tvec3& operator=( T a ){ set(a); return *this; }
	inline tvec3& operator+=( const tvec3& v ){ x+=v.x; y+=v.y; z+=v.z; return *this; }
	inline tvec3& operator-=( const tvec3& v ){ x-=v.x; y-=v.y; z-=v.z; return *this; }
	inline tvec3& operator*=( const tvec3& v ){ x*=v.x; y*=v.y; z*=v.z; return *this; }
	inline tvec3& operator/=( const tvec3& v ){ x/=v.x; y/=v.y; z/=v.z; return *this; }
	inline tvec3& operator+=( T a ){ x+=a; y+=a; z+=a; return *this; }
	inline tvec3& operator-=( T a ){ x-=a; y-=a; z-=a; return *this; }
	inline tvec3& operator*=( T a ){ x*=a; y*=a; z*=a; return *this; }
	inline tvec3& operator/=( T a ){ x/=a; y/=a; z/=a; return *this; }

	// comparison operators
	inline bool operator==( const tvec3& v ) const { return std::abs(x-v.x)<=precision<T>::value()&&std::abs(y-v.y)<=precision<T>::value()&&std::abs(z-v.z)<=precision<T>::value(); }
	inline bool operator!=( const tvec3& v ) const { return !operator==(v); }

	// casting operators
	inline operator T*(){ return &x; }
	inline operator const T*(){ return &x; }

	// array access operators
	inline T& operator[]( ptrdiff_t i ){ return (&r)[i]; }
	inline const T& operator[]( ptrdiff_t i ) const { return (&r)[i]; }
	inline T& at( ptrdiff_t i ){ return (&r)[i]; }
	inline const T& at( ptrdiff_t i ) const { return (&r)[i]; }

	// unary operators
	inline tvec3 operator+() const { return tvec3(r, g, b); }
	inline tvec3 operator-() const { return tvec3(-r, -g, -b); }

	// binary operators
	inline tvec3 operator+( const tvec3& v ) const { return tvec3(r+v.r, g+v.g, b+v.b); }
	inline tvec3 operator-( const tvec3& v ) const { return tvec3(r-v.r, g-v.g, b-v.b); }
	inline tvec3 operator*( const tvec3& v ) const { return tvec3(r*v.r, g*v.g, b*v.b); }
	inline tvec3 operator/( const tvec3& v ) const { return tvec3(r/v.r, g/v.g, b/v.b); }
	inline tvec3 operator+( T a ) const { return tvec3(r+a, g+a, b+a); }
	inline tvec3 operator-( T a ) const { return tvec3(r-a, g-a, b-a); }
	inline tvec3 operator*( T a ) const { return tvec3(r*a, g*a, b*a); }
	inline tvec3 operator/( T a ) const { return tvec3(r/a, g/a, b/a); }

	// length, normalize, dot product
	float_memfun(U) inline T length() const { return (T)(sqrt(r*r+g*g+b*b));}
	float_memfun(U) inline tvec3 normalize() const { return tvec3(r, g, b)/length(); }
	float_memfun(U) inline T dot( const tvec3& v ) const { return (T)(r*v.r+g*v.g+b*v.b); }
	float_memfun(U) inline T length2() const { return (T)(r*r+g*g+b*b);}

	// tvec3 only: cross product
	float_memfun(U) inline tvec3 cross( const tvec3& v ) const { return tvec3( g*v.b-b*v.g, b*v.r-r*v.b, r*v.g-g*v.r); }
};

//*******************************************************************
template <class T> struct tvec4
{
	union { struct { T x, y, z, w; }; struct { T r, g, b, a; }; struct { T s, t, p, q; }; };

	// constructor/set
	tvec4(){ x=y=z=w=0; }
	tvec4( T a ){ x=y=z=w=a; }										inline void set( T a ){ x=y=z=w=a; }
	tvec4( T a, T b, T c, T d ){ x=a;y=b;z=c;w=d; }					inline void set( T a, T b, T c, T d ){ x=a;y=b;z=c;w=d; }
	tvec4( const tvec4& v ){ x=v.x;y=v.y;z=v.z;w=v.w; }				inline void set( const tvec4& v ){ x=v.x;y=v.y;z=v.z;w=v.w; }
	tvec4( const tvec2<T>& v, T c, T d ){ x=v.x;y=v.y;z=c;w=d; }	inline void set( const tvec2<T>& v, T c, T d ){ x=v.x;y=v.y;z=c;w=d; }
	tvec4( T a, T b, const tvec2<T>& v ){ x=a;y=b;z=v.x;w=v.y; }	inline void set( T a, T b, const tvec2<T>& v ){ x=a;y=b;z=v.x;w=v.y; }	
	tvec4( const tvec3<T>& v, T d ){ x=v.x;y=v.y;z=v.z;w=d; }		inline void set( const tvec3<T>& v, T d ){ x=v.x;y=v.y;z=v.z;w=d; }
	tvec4( T a, const tvec3<T>& v ){ x=a;y=v.x;z=v.y;w=v.z; }		inline void set( T a, const tvec3<T>& v ){ x=a;y=v.x;z=v.y;w=v.z; }
	tvec4( const tvec2<T>& v1, const tvec2<T>& v2 ){ x=v1.x;y=v1.y;z=v2.x;w=v2.y; }
	inline void set( const tvec2<T>& v1, const tvec2<T>& v2 ){ x=v1.x;y=v1.y;z=v2.x;w=v2.y; }

	// assignment / compound assignment operators
	inline tvec4& operator=( T a ){ set(a); return *this; }
	inline tvec4& operator+=( const tvec4& v ){ x+=v.x; y+=v.y; z+=v.z; w+=v.w; return *this; }
	inline tvec4& operator-=( const tvec4& v ){ x-=v.x; y-=v.y; z-=v.z; w-=v.w; return *this; }
	inline tvec4& operator*=( const tvec4& v ){ x*=v.x; y*=v.y; z*=v.z; w*=v.w; return *this; }
	inline tvec4& operator/=( const tvec4& v ){ x/=v.x; y/=v.y; z/=v.z; w/=v.w; return *this; }
	inline tvec4& operator+=( T a ){ x+=a; y+=a; z+=a; w+=a; return *this; }
	inline tvec4& operator-=( T a ){ x-=a; y-=a; z-=a; w-=a; return *this; }
	inline tvec4& operator*=( T a ){ x*=a; y*=a; z*=a; w*=a; return *this; }
	inline tvec4& operator/=( T a ){ x/=a; y/=a; z/=a; w/=a; return *this; }

	// comparison operators
	inline bool operator==( const tvec4& v ) const { return std::abs(x-v.x)<=precision<T>::value()&&std::abs(y-v.y)<=precision<T>::value()&&std::abs(z-v.z)<=precision<T>::value()&&std::abs(w-v.w)<=precision<T>::value(); }
	inline bool operator!=( const tvec4& v ) const { return !operator==(v); }

	// casting operators
	inline operator T*(){ return &x; }
	inline operator const T*(){ return &x; }

	// array access operators
	inline T& operator[]( ptrdiff_t i ){ return (&r)[i]; }
	inline const T& operator[]( ptrdiff_t i ) const { return (&r)[i]; }
	inline T& at( ptrdiff_t i ){ return (&r)[i]; }
	inline const T& at( ptrdiff_t i ) const { return (&r)[i]; }

	// unary operators
	inline tvec4 operator+() const { return tvec4(r, g, b, a); }
	inline tvec4 operator-() const { return tvec4(-r, -g, -b, -a); }

	// binary operators
	inline tvec4 operator+( const tvec4& v ) const { return tvec4(r+v.r, g+v.g, b+v.b, a+v.a); }
	inline tvec4 operator-( const tvec4& v ) const { return tvec4(r-v.r, g-v.g, b-v.b, a-v.a); }
	inline tvec4 operator*( const tvec4& v ) const { return tvec4(r*v.r, g*v.g, b*v.b, a*v.a); }
	inline tvec4 operator/( const tvec4& v ) const { return tvec4(r/v.r, g/v.g, b/v.b, a/v.a); }
	inline tvec4 operator+( T v ) const { return tvec4(r+v, g+v, b+v, this->a+v); }
	inline tvec4 operator-( T v ) const { return tvec4(r-v, g-v, b-v, this->a-v); }
	inline tvec4 operator*( T v ) const { return tvec4(r*v, g*v, b*v, this->a*v); }
	inline tvec4 operator/( T v ) const { return tvec4(r/v, g/v, b/v, this->a/v); }

	// length, normalize, dot product
	float_memfun(U) inline T length() const { return (T)(sqrt(r*r+g*g+b*b+a*a)); }
	float_memfun(U) inline tvec4 normalize() const { return tvec4(r, g, b, a)/length(); } 
	float_memfun(U) inline T dot( const tvec4& v ) const { return (T)(r*v.r+g*v.g+b*v.b+a*v.a); }
	float_memfun(U) inline T length2() const { return (T)(r*r+g*g+b*b+a*a); }
};

//*******************************************************************
// type definitions
using vec2	= tvec2<float>;		using vec3	= tvec3<float>;		using vec4	= tvec4<float>;
using ivec2 = tvec2<int>;		using ivec3 = tvec3<int>;		using ivec4 = tvec4<int>;
using uvec2 = tvec2<uint>;		using uvec3 = tvec3<uint>;		using uvec4 = tvec4<uint>;
using dvec2 = tvec2<double>;	using dvec3 = tvec3<double>;	using dvec4 = tvec4<double>;

//*******************************************************************
// matrix 3x3: uses a standard row-major notation
struct mat3
{
	union { float a[9]; struct {float _11,_12,_13,_21,_22,_23,_31,_32,_33;}; };

	inline mat3(){ _12=_13=_21=_23=_31=_32=0.0f;_11=_22=_33=1.0f; }
	inline mat3( float f11, float f12, float f13, float f21, float f22, float f23, float f31, float f32, float f33 ){_11=f11;_12=f12;_13=f13;_21=f21;_22=f22;_23=f23;_31=f31;_32=f32;_33=f33;}

	// comparison operators
	inline bool operator==( const mat3& m ) const { for( size_t k=0; k<std::extent<decltype(a)>::value; k++ ) if(std::abs(a[k]-m[k])>precision<float>::value()) return false; return true; }
	inline bool operator!=( const mat3& m ) const { return !operator==(m); }

	// casting operators
	inline operator float*(){ return a; }
	inline operator const float*() const { return a; }

	// array access operators
	inline float& operator[]( ptrdiff_t i ){ return a[i]; }
	inline const float& operator[]( ptrdiff_t i ) const { return a[i]; }
	inline float& at( ptrdiff_t i ){ return a[i]; }
	inline const float& at( ptrdiff_t i ) const { return a[i]; }

	// row vectors
	inline vec3& rvec3( int row ){ return reinterpret_cast<vec3&>(a[row*3]); }
	inline const vec3& rvec3( int row ) const { return reinterpret_cast<const vec3&>(a[row*3]); }

	// identity and transpose
	inline static mat3 identity(){ return mat3(); }
	inline mat3& set_identity(){ _12=_13=_21=_23=_31=_32=0.0f;_11=_22=_33=1.0f; return *this; }
	inline mat3 transpose() const { return mat3(_11,_21,_31,_12,_22,_32,_13,_23,_33); }

	// addition/subtraction operators
	inline mat3 operator+( const mat3& m ) const { mat3 r; for( size_t k=0; k < std::extent<decltype(a)>::value; k++ ) r[k]=a[k]+m[k]; return r; }
	inline mat3 operator-( const mat3& m ) const { mat3 r; for( size_t k=0; k < std::extent<decltype(a)>::value; k++ ) r[k]=a[k]-m[k]; return r; }
	inline mat3& operator+=( const mat3& m ){ return *this=operator+(m); }
	inline mat3& operator-=( const mat3& m ){ return *this=operator-(m); }

	// multiplication operators
	inline mat3 operator*( float f ) const { mat3 r; for( size_t k=0; k < std::extent<decltype(a)>::value; k++ ) r[k]=a[k]*f; return r; }
	inline vec3 operator*( const vec3& v ) const { return vec3(rvec3(0).dot(v), rvec3(1).dot(v), rvec3(2).dot(v)); }
	inline mat3 operator*( const mat3& m ) const { mat3 t=m.transpose(), r; for(int k=0;k<3;k++) r.rvec3(k)=t.operator*(rvec3(k)); return r; } // a bit tricky implementation
	inline mat3& operator*=( const mat3& m ){ return *this=operator*(m); }

	// determinant
	inline float det() const { return _11*(_22*_33-_23*_32) + _12*(_23*_31-_21*_33) + _13*(_21*_32-_22*_31); }

	// inverse
	inline mat3 inverse() const
	{
		float d=det(), s=1.0f/d; if(d==0) printf( "mat3::inverse() might be singular.\n" );
		return mat3( (_22*_33-_32*_23)*s, (_13*_32-_12*_33)*s, (_12*_23-_13*_22)*s, (_23*_31-_21*_33)*s, (_11*_33-_13*_31)*s, (_21*_13-_11*_23)*s, (_21*_32-_31*_22)*s, 	(_31*_12-_11*_32)*s,	 (_11*_22-_21*_12)*s );
	}
};

//*******************************************************************
// matrix 4x4: uses a standard row-major notation
struct mat4
{
	union { float a[16]; struct {float _11,_12,_13,_14,_21,_22,_23,_24,_31,_32,_33,_34,_41,_42,_43,_44;}; };

	mat4(){ _12=_13=_14=_21=_23=_24=_31=_32=_34=_41=_42=_43=0.0f;_11=_22=_33=_44=1.0f; }
	mat4( float f11, float f12, float f13, float f14, float f21, float f22, float f23, float f24, float f31, float f32, float f33, float f34, float f41, float f42, float f43, float f44 ){_11=f11;_12=f12;_13=f13;_14=f14;_21=f21;_22=f22;_23=f23;_24=f24;_31=f31;_32=f32;_33=f33;_34=f34;_41=f41;_42=f42;_43=f43;_44=f44;}

	// comparison operators
	inline bool operator==( const mat4& m ) const { for( size_t k=0; k<std::extent<decltype(a)>::value; k++ ) if(std::abs(a[k]-m[k])>precision<float>::value()) return false; return true; }
	inline bool operator!=( const mat4& m ) const { return !operator==(m); }

	// casting operators
	inline operator float*(){ return a; }
	inline operator const float*() const { return a; }
	inline operator mat3() const {return mat3(_11, _12, _13, _21, _22, _23, _31, _32, _33 ); }

	// array access operators
	inline float& operator[]( ptrdiff_t i ){ return a[i]; }
	inline const float& operator[]( ptrdiff_t i ) const { return a[i]; }
	inline float& at( ptrdiff_t i ){ return a[i]; }
	inline const float& at( ptrdiff_t i ) const { return a[i]; }

	// row vectors
	inline vec4& rvec4( int row ){ return reinterpret_cast<vec4&>(a[row*4]); }
	inline vec3& rvec3( int row ){ return reinterpret_cast<vec3&>(a[row*4]); }
	inline const vec4& rvec4( int row ) const { return reinterpret_cast<const vec4&>(a[row*4]); }
	inline const vec3& rvec3( int row ) const { return reinterpret_cast<const vec3&>(a[row*4]); }

	// identity and transpose
	static mat4 identity(){ return mat4(); }
	inline mat4& set_identity(){ _12=_13=_14=_21=_23=_24=_31=_32=_34=_41=_42=_43=0.0f;_11=_22=_33=_44=1.0f; return *this; }
	inline mat4 transpose() const { return mat4(_11, _21, _31, _41, _12, _22, _32, _42, _13, _23, _33, _43, _14, _24, _34, _44); }

	// addition/subtraction operators
	inline mat4 operator+( const mat4& m ) const { mat4 r; for( size_t k=0; k < std::extent<decltype(a)>::value; k++ ) r[k]=a[k]+m[k]; return r; }
	inline mat4 operator-( const mat4& m ) const { mat4 r; for( size_t k=0; k < std::extent<decltype(a)>::value; k++ ) r[k]=a[k]-m[k]; return r; }
	inline mat4& operator+=( const mat4& m ){ return *this=operator+(m); }
	inline mat4& operator-=( const mat4& m ){ return *this=operator-(m); }

	// multiplication operators
	inline mat4 operator*( float f ) const { mat4 r; for( size_t k=0; k < std::extent<decltype(a)>::value; k++ ) r[k]=a[k]*f; return r; }
	inline vec4 operator*( const vec4& v ) const { return vec4(rvec4(0).dot(v), rvec4(1).dot(v), rvec4(2).dot(v), rvec4(3).dot(v)); }
	inline mat4 operator*( const mat4& m ) const { mat4 t=m.transpose(), r; for(uint k=0;k<4;k++) r.rvec4(k)=t.operator*(rvec4(k)); return r; } // a bit tricky implementation
	inline mat4& operator*=( const mat4& m ){ return *this=operator*(m); }

	// determinant and inverse: see below for implementations
	inline float det() const;
	inline mat4 inverse() const;

	// static row-major transformations
	static mat4 translate( const vec3& v ){ return mat4().set_translate(v); }
	static mat4 translate( float x, float y, float z ){ return mat4().set_translate(x,y,z); }
	static mat4 scale( const vec3& v ){ return mat4().set_scale(v); }
	static mat4 scale( float x, float y, float z ){ return mat4().set_scale(x,y,z); }
	static mat4 rotate( const vec3& axis, float angle ){ return mat4().set_rotate(axis,angle); }
	static mat4 look_at( const vec3& eye, const vec3& at, const vec3& up ){ return mat4().set_look_at(eye, at, up); }
	static mat4 perspective( float fovy, float aspect, float dnear, float dfar ){ return mat4().set_perspective(fovy, aspect, dnear, dfar); }

	// row-major transformations
	inline mat4& set_translate( const vec3& v ){ set_identity(); _14=v.x; _24=v.y; _34=v.z; return *this; }
	inline mat4& set_translate( float x,float y,float z ){ set_identity(); _14=x; _24=y; _34=z; return *this; }
	inline mat4& set_scale( const vec3& v ){ set_identity(); _11=v.x; _22=v.y; _33=v.z; return *this; }
	inline mat4& set_scale( float x, float y, float z ){ set_identity(); _11=x; _22=y; _33=z; return *this; }
	inline mat4& set_rotate( const vec3& axis, float angle )
	{
		float c=cos(angle), s=sin(angle), x=axis.x, y=axis.y, z=axis.z;
		a[0] = x*x*(1-c)+c;		a[1] = x*y*(1-c)-z*s;		a[2] = x*z*(1-c)+y*s;	a[3] = 0.0f;
		a[4] = x*y*(1-c)+z*s;	a[5] = y*y*(1-c)+c;			a[6] = y*z*(1-c)-x*s;	a[7] = 0.0f;
		a[8] = x*z*(1-c)-y*s;	a[9] = y*z*(1-c)+x*s;		a[10] = z*z*(1-c)+c;	a[11] = 0.0f;
		a[12] = 0;				a[13] = 0;					a[14] = 0;				a[15] = 1.0f;
		return *this;
	}

	mat4& set_look_at( const vec3& eye, const vec3& at, const vec3& up )
	{
		set_identity();

		// define camera frame
		vec3 n = (eye-at).normalize();
		vec3 u = up.cross(n).normalize();
		vec3 v = n.cross(u).normalize();

		// calculate lookAt matrix
		_11 = u.x;  _12 = u.y;  _13 = u.z;  _14 = -u.dot(eye);
		_21 = v.x;  _22 = v.y;  _23 = v.z;  _24 = -v.dot(eye);
		_31 = n.x;  _32 = n.y;  _33 = n.z;  _34 = -n.dot(eye);

		return *this;
	};

	mat4& set_perspective( float fovy, float aspect, float dnear, float dfar )
	{
		set_identity();
		_22 = 1 / tan(fovy / 2.0f);
		_11 = _22 / aspect;
		_33 = (dnear + dfar) / (dnear - dfar);
		_34 = (2 * dnear * dfar) / (dnear - dfar);
		_43 = -1;
		_44 = 0;

		return *this;
	}
};

inline float mat4::det() const
{
	return
	_41 * _32 * _23 * _14 - _31 * _42 * _23 * _14 - _41 * _22 * _33 * _14 + _21 * _42 * _33 * _14 +
	_31 * _22 * _43 * _14 - _21 * _32 * _43 * _14 - _41 * _32 * _13 * _24 + _31 * _42 * _13 * _24 +
	_41 * _12 * _33 * _24 - _11 * _42 * _33 * _24 - _31 * _12 * _43 * _24 + _11 * _32 * _43 * _24 +
	_41 * _22 * _13 * _34 - _21 * _42 * _13 * _34 - _41 * _12 * _23 * _34 + _11 * _42 * _23 * _34 +
	_21 * _12 * _43 * _34 - _11 * _22 * _43 * _34 - _31 * _22 * _13 * _44 + _21 * _32 * _13 * _44 +
	_31 * _12 * _23 * _44 - _11 * _32 * _23 * _44 - _21 * _12 * _33 * _44 + _11 * _22 * _33 * _44 ;
}

inline mat4 mat4::inverse() const
{
	float d=det(), s=1.0f/d; if(d==0) printf( "mat4::inverse() might be singular.\n" );
	return mat4((_32*_43*_24 - _42*_33*_24 + _42*_23*_34 - _22*_43*_34 - _32*_23*_44 + _22*_33*_44)*s,
				(_42*_33*_14 - _32*_43*_14 - _42*_13*_34 + _12*_43*_34 + _32*_13*_44 - _12*_33*_44)*s,
				(_22*_43*_14 - _42*_23*_14 + _42*_13*_24 - _12*_43*_24 - _22*_13*_44 + _12*_23*_44)*s,
				(_32*_23*_14 - _22*_33*_14 - _32*_13*_24 + _12*_33*_24 + _22*_13*_34 - _12*_23*_34)*s,

				(_41*_33*_24 - _31*_43*_24 - _41*_23*_34 + _21*_43*_34 + _31*_23*_44 - _21*_33*_44)*s,
				(_31*_43*_14 - _41*_33*_14 + _41*_13*_34 - _11*_43*_34 - _31*_13*_44 + _11*_33*_44)*s,
				(_41*_23*_14 - _21*_43*_14 - _41*_13*_24 + _11*_43*_24 + _21*_13*_44 - _11*_23*_44)*s,
				(_21*_33*_14 - _31*_23*_14 + _31*_13*_24 - _11*_33*_24 - _21*_13*_34 + _11*_23*_34)*s,

				(_31*_42*_24 - _41*_32*_24 + _41*_22*_34 - _21*_42*_34 - _31*_22*_44 + _21*_32*_44)*s,
				(_41*_32*_14 - _31*_42*_14 - _41*_12*_34 + _11*_42*_34 + _31*_12*_44 - _11*_32*_44)*s,
				(_21*_42*_14 - _41*_22*_14 + _41*_12*_24 - _11*_42*_24 - _21*_12*_44 + _11*_22*_44)*s,
				(_31*_22*_14 - _21*_32*_14 - _31*_12*_24 + _11*_32*_24 + _21*_12*_34 - _11*_22*_34)*s,

				(_41*_32*_23 - _31*_42*_23 - _41*_22*_33 + _21*_42*_33 + _31*_22*_43 - _21*_32*_43)*s,
				(_31*_42*_13 - _41*_32*_13 + _41*_12*_33 - _11*_42*_33 - _31*_12*_43 + _11*_32*_43)*s,
				(_41*_22*_13 - _21*_42*_13 - _41*_12*_23 + _11*_42*_23 + _21*_12*_43 - _11*_22*_43)*s,
				(_21*_32*_13 - _31*_22*_13 + _31*_12*_23 - _11*_32*_23 - _21*_12*_33 + _11*_22*_33)*s );
}

//*******************************************************************
// scalar-vector operators
inline vec2 operator+( float f, const vec2& v ){ return v+f; }
inline vec3 operator+( float f, const vec3& v ){ return v+f; }
inline vec4 operator+( float f, const vec4& v ){ return v+f; }
inline vec2 operator-( float f, const vec2& v ){ return -v+f; }
inline vec3 operator-( float f, const vec3& v ){ return -v+f; }
inline vec4 operator-( float f, const vec4& v ){ return -v+f; }
inline vec2 operator*( float f, const vec2& v ){ return v*f; }
inline vec3 operator*( float f, const vec3& v ){ return v*f; }
inline vec4 operator*( float f, const vec4& v ){ return v*f; }

//*******************************************************************
// vertor-matrix multiplications
inline vec3 mul( const vec3& v, const mat3& m ){ return m.transpose()*v; }
inline vec4 mul( const vec4& v, const mat4& m ){ return m.transpose()*v; }
inline vec3 mul( const mat3& m, const vec3& v ){ return m*v; }
inline vec4 mul( const mat4& m, const vec4& v ){ return m*v; }
inline vec3 operator*( const vec3& v, const mat3& m ){ return m.transpose()*v; }
inline vec4 operator*( const vec4& v, const mat4& m ){ return m.transpose()*v; }
inline float dot( const vec2& v1, const vec2& v2){ return v1.dot(v2); }
inline float dot( const vec3& v1, const vec3& v2){ return v1.dot(v2); }
inline float dot( const vec4& v1, const vec4& v2){ return v1.dot(v2); }
inline vec3 cross( const vec3& v1, const vec3& v2){ return v1.cross(v2); }

//*******************************************************************
// utility math functions
inline uint miplevels( uint width, uint height=1 ){ uint l=0; uint s=width>height?width:height; while(s){s=s>>1;l++;} return l; }

//*******************************************************************
// {GLSL|HLSL}-like shader intrinsic functions
inline vec2 abs( const vec2& v ){ return vec2(fabs(v.x),fabs(v.y)); }
inline vec3 abs( const vec3& v ){ return vec3(fabs(v.x),fabs(v.y),fabs(v.z)); }
inline vec4 abs( const vec4& v ){ return vec4(fabs(v.x),fabs(v.y),fabs(v.z),fabs(v.w)); }
template <class T,class N,class X> T clamp( T v, N vmin, X vmax ){ return v<T(vmin)?T(vmin):v>T(vmax)?T(vmax):v; }
inline float degrees( float f ){ return float(f*float(180.0)/PI); }
inline float distance( const vec2& a, const vec2& b ){ return (a-b).length(); }
inline float distance( const vec3& a, const vec3& b ){ return (a-b).length(); }
inline float distance( const vec4& a, const vec4& b ){ return (a-b).length(); }
inline float fract( float f ){ return float(f-floor(f)); }
inline vec2 fract( const vec2& v ){ return vec2(fract(v.x),fract(v.y)); }
inline vec3 fract( const vec3& v ){ return vec3(fract(v.x),fract(v.y),fract(v.z)); }
inline vec4 fract( const vec4& v ){ return vec4(fract(v.x),fract(v.y),fract(v.z),fract(v.w)); }
inline vec2 fma( vec2 a, vec2 b, vec2 c ){ return vec2(fma(a.x,b.x,c.x),fma(a.y,b.y,c.y)); }
inline vec3 fma( vec3 a, vec3 b, vec3 c ){ return vec3(fma(a.x,b.x,c.x),fma(a.y,b.y,c.y),fma(a.z,b.z,c.z)); }
inline vec4 fma( vec4 a, vec4 b, vec4 c ){ return vec4(fma(a.x,b.x,c.x),fma(a.y,b.y,c.y),fma(a.z,b.z,c.z),fma(a.w,b.w,c.w)); }
inline vec2 fabs( const vec2& v ){ return vec2(fabs(v.x),fabs(v.y)); }
inline vec3 fabs( const vec3& v ){ return vec3(fabs(v.x),fabs(v.y),fabs(v.z)); }
inline vec4 fabs( const vec4& v ){ return vec4(fabs(v.x),fabs(v.y),fabs(v.z),fabs(v.w)); }
inline float length( const vec2& v ){ return v.length(); }
inline float length( const vec3& v ){ return v.length(); }
inline float length( const vec4& v ){ return v.length(); }
inline float length2( const vec2& v ){ return v.length2(); }
inline float length2( const vec3& v ){ return v.length2(); }
inline float length2( const vec4& v ){ return v.length2(); }
inline float lerp( float y1, float y2, float t ){ return y1*(-t+1.0f)+y2*t; }
inline vec2 lerp( const vec2& y1, const vec2& y2, const vec2& t ){ return y1*(-t+1.0f)+y2*t; }
inline vec3 lerp( const vec3& y1, const vec3& y2, const vec3& t ){ return y1*(-t+1.0f)+y2*t; }
inline vec4 lerp( const vec4& y1, const vec4& y2, const vec4& t ){ return y1*(-t+1.0f)+y2*t; }
inline float mix( float v1, float v2, float t ){ return lerp(v1,v2,t); }
inline vec2 mix( vec2 v1, vec2 v2, vec2 t ){ return lerp(v1,v2,t); }
inline vec3 mix( vec3 v1, vec3 v2, vec3 t ){ return lerp(v1,v2,t); }
inline vec4 mix( vec4 v1, vec4 v2, vec4 t ){ return lerp(v1,v2,t); }
inline vec2 normalize( const vec2& v ){ return v.normalize(); }
inline vec3 normalize( const vec3& v ){ return v.normalize(); }
inline vec4 normalize( const vec4& v ){ return v.normalize(); }
inline float radians( float f ){ return float(f*PI/float(180.0)); }
inline vec3 reflect( const vec3& I, const vec3& N ){ return I-N*dot(I,N)*2.0f; }	// I: incident vector, N: normal
inline vec3 refract( const vec3& I, const vec3& N, float eta /* = n0/n1 */ ){ float d = I.dot(N); float k = 1.0f-eta*eta*(1.0f-d*d); return k<0.0f?0.0f:(I*eta-N*(eta*d+sqrtf(k))); } // I: incident vector, N: normal
inline float saturate( float value ){ return value<0.0f?0.0f:value>1.0f?1.0f:value; }
inline vec2 saturate( const vec2& value ){ return vec2(saturate(value.x),saturate(value.y)); }
inline vec3 saturate( const vec3& value ){ return vec3(saturate(value.x),saturate(value.y),saturate(value.z)); }
inline vec4 saturate(const  vec4& value ){ return vec4(saturate(value.x),saturate(value.y),saturate(value.z),saturate(value.w)); }
inline float sign( float f ){ return f>0.0f?1.0f:f<0.0f?-1.0f:0; }
inline vec2 sign( const vec2& v ){ return vec2(sign(v.x),sign(v.y)); }
inline vec3 sign( const vec3& v ){ return vec3(sign(v.x),sign(v.y),sign(v.z)); }
inline vec4 sign( const vec4& v ){ return vec4(sign(v.x),sign(v.y),sign(v.z),sign(v.w)); }
inline float smoothstep( float t ){ t=saturate(t); return t*t*(3-2*t); }							// C1-continuity
inline vec2 smoothstep( const vec2& t ){ return vec2(smoothstep(t.x),smoothstep(t.y)); }
inline vec3 smoothstep( const vec3& t ){ return vec3(smoothstep(t.x),smoothstep(t.y),smoothstep(t.z)); }
inline vec4 smoothstep( const vec4& t ){ return vec4(smoothstep(t.x),smoothstep(t.y),smoothstep(t.z),smoothstep(t.w)); }
inline float smootherstep( float t ){ t=saturate(t); return t*t*t*(6.0f*t*t-15.0f*t+10.0f); }		// C2-continuity (by Ken Perlin)
inline vec2 smootherstep( const vec2& t ){ return vec2(smootherstep(t.x),smootherstep(t.y)); }
inline vec3 smootherstep( const vec3& t ){ return vec3(smootherstep(t.x),smootherstep(t.y),smootherstep(t.z)); }
inline vec4 smootherstep( const vec4& t ){ return vec4(smootherstep(t.x),smootherstep(t.y),smootherstep(t.z),smootherstep(t.w)); }

#endif // __CGMATH_H__
