#pragma once
#undef ABS
#undef SIGN
#undef MIN
#undef MAX
#undef CLAMP

// Generic ABS function, for math uses please use Math::abs.
template <typename T>
constexpr T ABS(T m_v) {
	return m_v < 0 ? -m_v : m_v;
}

template <typename T>
constexpr const T SIGN(const T m_v) {
	return m_v == 0 ? 0.0f : (m_v < 0 ? -1.0f : +1.0f);
}

template <typename T, typename T2>
constexpr auto MIN(const T m_a, const T2 m_b) {
	return m_a < m_b ? m_a : m_b;
}

template <typename T, typename T2>
constexpr auto MAX(const T m_a, const T2 m_b) {
	return m_a > m_b ? m_a : m_b;
}

template <typename T, typename T2, typename T3>
constexpr auto CLAMP(const T m_a, const T2 m_min, const T3 m_max) {
	return m_a < m_min ? m_min : (m_a > m_max ? m_max : m_a);
}
template <typename T, typename T2, typename T3>
constexpr auto isEqualApprox(const T m_a, const T2 m_b, const T3 tolerance) {
	return ABS(m_a - m_b) < tolerance;
}

// Generic swap template.
#ifndef SWAP
#define SWAP(m_x, m_y) __swap_tmpl((m_x), (m_y))
template <class T>
inline void __swap_tmpl(T& x, T& y) {
	T aux = x;
	x = y;
	y = aux;
}
#endif // SWAP

template <size_t... Is>
struct indexSequence {};

template <size_t N, size_t... Is>
struct buildIndexSequence : buildIndexSequence<N - 1, N - 1, Is...> {};

template <size_t... Is>
struct buildIndexSequence<0, Is...> : indexSequence<Is...> {};

#define MAX_RECURSION 100