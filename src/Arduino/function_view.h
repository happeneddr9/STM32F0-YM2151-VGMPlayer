/*
 * function_view.h
 *
 *  Created on: 2020年3月11日
 *      Author: happe
 *
 * 	@brief:	taken from Sergey Ryazanov & Yakk - Adam Nevraumont's function_view,
 * 			using this replace std::function
 */

#ifndef ARDUINO_FUNCTION_VIEW_H_
#define ARDUINO_FUNCTION_VIEW_H_

#include <cmath>
#include <map>
#include <vector>
#include <stack>
#include <string>
#include <atomic>
#include <wchar.h>
#include <chrono>
#include <utility>
#include <tuple>
#include <string>
#include <functional>




template<class Sig>
struct function_view;

template<class R, class ...Args>
struct function_view<R(Args...)> {
		template <typename _Func, typename _Res2 = typename std::result_of<_Func&(Args...)>::type>
		struct _Callable : std::__check_func_return_type<_Res2, R> {};

		// Used so the return type convertibility checks aren't done when
		// performing overload resolution for copy construction/assignment.
		template<typename _Tp>
		struct _Callable<function_view, _Tp> : std::false_type {};

		template<typename _Cond, typename _Tp>
		using _Requires = typename std::enable_if<_Cond::value, _Tp>::type;


		void* ptr = nullptr;
		R (*pf)(void*, Args...) = nullptr;

		template<class F>
		using pF = decltype(std::addressof(std::declval<F&>()));

		template<class F>
		void bind_to(F& f) noexcept {
			ptr = (void*) std::addressof(f);
			pf = [](void* ptr, Args... args)->R {
				return (*(pF<F>)ptr)(std::forward<Args>(args)...);
			};
		}
		// when binding to a function pointer
		// even a not identical one, check for
		// null.  In addition, we can remove a
		// layer of indirection and store the function
		// pointer directly in the `void*`.
		template<class R_in, class ...Args_in>
		void bind_to(R_in (*f)(Args_in...)) noexcept {
			using Fun = decltype(f);
			if (!f)
				return bind_to(nullptr);
			ptr = (void*) f;
			pf = [](void* ptr, Args... args)->R {
				return (Fun(ptr))(std::forward<Args>(args)...);
			};
		}
		// binding to nothing:
		void bind_to(std::nullptr_t) noexcept {
			ptr = nullptr;
			pf = nullptr;
		}
		explicit operator bool() const noexcept {
			return pf;
		}

		function_view() noexcept {
			bind_to(nullptr);
		}

		function_view(const function_view& __x) noexcept = default;

		/**
		 *  @brief %Function move constructor.
		 *  @param __x A %function object rvalue with identical call signature.
		 *
		 *  The newly-created %function contains the target of @a __x
		 *  (if it has one).
		 */
		function_view(function_view&& __x) noexcept {
			__x.swap(*this);
		}

		template<class F, std::enable_if_t<
				!std::is_same<function_view, std::decay_t<F>> { }, int> = 0,
				std::enable_if_t<
						std::is_convertible<std::result_of_t<F & (Args...)>, R> { },
						int> = 0>
		function_view(F&& f) noexcept {
			bind_to(f); // not forward
		}

		// when binding to a function pointer
		// even a not identical one, check for
		// null.  In addition, we can remove a
		// layer of indirection and store the function
		// pointer directly in the `void*`.
		template<class R_in, class ...Args_in>
		function_view(R_in (*f)(Args_in...)) noexcept {
			bind_to(f); // not forward
		}

		function_view(std::nullptr_t) noexcept {
			bind_to(nullptr);
		}

		function_view& operator=(const function_view&) = default;

		/*function_view& operator=(const function_view& __x) noexcept {
			function_view(__x).swap(*this);
			return *this;
		}*/

		function_view& operator=(function_view&& __x) noexcept {
			function_view(std::move(__x)).swap(*this);
			return *this;
		}

		function_view& operator=(nullptr_t) noexcept {
			if (pf) {
				ptr = nullptr;
				pf = nullptr;
			}
			return *this;
		}

		/**
		 *  @brief %Function assignment to a new target.
		 *  @param __f A %function object that is callable with parameters of
		 *  type @c T1, @c T2, ..., @c TN and returns a value convertible
		 *  to @c Res.
		 *  @return @c *this
		 *
		 *  This  %function object wrapper will target a copy of @a
		 *  __f. If @a __f is @c reference_wrapper<F>, then this function
		 *  object will contain a reference to the function object @c
		 *  __f.get(). If @a __f is a NULL function pointer or NULL
		 *  pointer-to-member, @c this object will be empty.
		 *
		 *  If @a __f is a non-NULL function pointer or an object of type @c
		 *  reference_wrapper<F>, this function will not throw.
		 */
		template <typename _Functor>
		_Requires<_Callable<typename std::decay<_Functor>::type>, function_view&>
		operator=(_Functor&& __f) {
			function_view(std::forward<_Functor>(__f)).swap(*this);
			return *this;
		}

		 /// @overload
		template <typename _Functor>
		function_view&
		operator=(std::reference_wrapper<_Functor> __f) noexcept
		{
			function_view(__f).swap(*this);
			return *this;
		}



		void swap(function_view& __x) noexcept {
			std::swap(ptr, __x.ptr);
			std::swap(pf, __x.pf);
		}



		R operator()(Args ...args) const noexcept {
			return pf(ptr, std::forward<Args>(args)...);
		}
};

// [20.7.15.2.6] null pointer comparisons

/**
 *  @brief Compares a polymorphic function object wrapper against 0
 *  (the NULL pointer).
 *  @returns @c true if the wrapper has no target, @c false otherwise
 *
 *  This function will not throw an %exception.
 */
template<typename _Res, typename ... _Args>
inline bool operator==(const function_view<_Res(_Args...)>& __f, nullptr_t) noexcept {
	return !static_cast<bool>(__f);

}

/// @overload
template<typename _Res, typename ... _Args>
inline bool operator==(nullptr_t, const function_view<_Res(_Args...)>& __f) noexcept {
	return !static_cast<bool>(__f);

}

/**
 *  @brief Compares a polymorphic function object wrapper against 0
 *  (the NULL pointer).
 *  @returns @c false if the wrapper has no target, @c true otherwise
 *
 *  This function will not throw an %exception.
 */
template<typename _Res, typename ... _Args>
inline bool operator!=(const function_view<_Res(_Args...)>& __f, nullptr_t) noexcept {
	return static_cast<bool>(__f);

}

/// @overload
template<typename _Res, typename ... _Args>
inline bool operator!=(nullptr_t, const function_view<_Res(_Args...)>& __f) noexcept {
	return static_cast<bool>(__f);

}

// [20.7.15.2.7] specialized algorithms

/**
 *  @brief Swap the targets of two polymorphic function object wrappers.
 *
 *  This function will not throw an %exception.
 */
// _GLIBCXX_RESOLVE_LIB_DEFECTS
// 2062. Effect contradictions w/o no-throw guarantee of std::function swaps
template <typename _Res, typename ... _Args>
inline void swap(function_view<_Res(_Args...)>& __x, function_view<_Res(_Args...)>& __y) noexcept {
	__x.swap(__y);
}



#endif /* ARDUINO_FUNCTION_VIEW_H_ */
