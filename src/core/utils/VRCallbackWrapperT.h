#ifndef VRCALLBACKWRAPPERT_H_INCLUDED
#define VRCALLBACKWRAPPERT_H_INCLUDED

#include "VRCallbackWrapper.h"

#define CW_CHECK_SIZE(N) \
int Np = params.size(); \
int Ndp = defaultParams.size(); \
if (Np > N) { \
    VRCallbackWrapperBase::err = "Function takes at most "+toString(N)+" arguments ("+toString( Np )+" given)"; \
    return false; \
} \
if (Np < N-Ndp ) { \
    VRCallbackWrapperBase::err = "Function takes at least "+toString(N-Ndp)+" arguments ("+toString( Np )+" given)"; \
    return false; \
}

#define CW_GET_VALUE(i,T,t) \
T t; \
if (i < params.size()) { \
    if (! toValue(params[i], t)) { \
        VRCallbackWrapperBase::err = "Function argument "+toString(i)+" expects a "+typeName(t)+" ("+typeName( params[i] )+" given)"; \
        return false; \
    } \
} else { \
    int j = i - params.size(); \
    if (! toValue(defaultParams[j], t)) { \
        VRCallbackWrapperBase::err = "Internal error, function argument "+toString(i)+" expects a "+typeName(t)+", but got default argument "+defaultParams[j]; \
        return false; \
    } \
}

#define MACRO_GET_1(str, i) \
    (sizeof(str) > (i) ? str[(i)] : 0)

#define MACRO_GET_4(str, i) \
    MACRO_GET_1(str, i+0),  \
    MACRO_GET_1(str, i+1),  \
    MACRO_GET_1(str, i+2),  \
    MACRO_GET_1(str, i+3)

#define MACRO_GET_16(str, i) \
    MACRO_GET_4(str, i+0),   \
    MACRO_GET_4(str, i+4),   \
    MACRO_GET_4(str, i+8),   \
    MACRO_GET_4(str, i+12)

#define MACRO_GET_STR(str) MACRO_GET_16(str, 0), 0 //guard for longer strings

template<char... C>
struct VRCallbackWrapperParams {
    static int size() {
        const int n = sizeof...(C);
        return n;
    }
    static string str() {
        return string({C...});
    }
};

template <typename P, typename U, typename T> struct VRCallbackWrapperT;
template <typename P, typename U, typename T, typename R, typename ...Args>
struct VRCallbackWrapperT<P, U, R (T::*)(Args...)> : public VRCallbackWrapper<P> {
    typedef R (T::*Callback)(Args...);
    Callback callback;

    static std::shared_ptr<VRCallbackWrapperT> create() {
        return std::shared_ptr<VRCallbackWrapperT>( new VRCallbackWrapperT() );
    }

     // TODO: generic solution instead one for each number of parameter??
     // template class O is not really needed, only a trick to allow to call without parameters

    template<class O>
    bool call(T* obj, const vector<P>& params, R& r, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(0);
        r = (obj->*callback)(); return true;
    }

    template<class O, class A>
    bool call(T* obj, const vector<P>& params, R& r, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(1);
        CW_GET_VALUE(0,A,a);
        r = (obj->*callback)( a ); return true;
    }

    template<class O, class A, class B>
    bool call(T* obj, const vector<P>& params, R& r, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(2);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        r = (obj->*callback)( a, b ); return true;
    }

    template<class O, class A, class B, class C>
    bool call(T* obj, const vector<P>& params, R& r, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(3);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        CW_GET_VALUE(2,C,c);
        r = (obj->*callback)( a, b, c ); return true;
    }

    template<class O, class A, class B, class C, class D>
    bool call(T* obj, const vector<P>& params, R& r, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(4);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        CW_GET_VALUE(2,C,c);
        CW_GET_VALUE(3,D,d);
        r = (obj->*callback)( a, b, c, d ); return true;
    }

    template<class O, class A, class B, class C, class D, class E>
    bool call(T* obj, const vector<P>& params, R& r, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(5);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        CW_GET_VALUE(2,C,c);
        CW_GET_VALUE(3,D,d);
        CW_GET_VALUE(4,E,e);
        r = (obj->*callback)( a, b, c, d, e ); return true;
    }

    template<class O, class A, class B, class C, class D, class E, class F>
    bool call(T* obj, const vector<P>& params, R& r, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(6);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        CW_GET_VALUE(2,C,c);
        CW_GET_VALUE(3,D,d);
        CW_GET_VALUE(4,E,e);
        CW_GET_VALUE(5,F,f);
        r = (obj->*callback)( a, b, c, d, e, f ); return true;
    }

    template<class O, class A, class B, class C, class D, class E, class F, class G>
    bool call(T* obj, const vector<P>& params, R& r, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(7);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        CW_GET_VALUE(2,C,c);
        CW_GET_VALUE(3,D,d);
        CW_GET_VALUE(4,E,e);
        CW_GET_VALUE(5,F,f);
        CW_GET_VALUE(6,G,g);
        r = (obj->*callback)( a, b, c, d, e, f, g ); return true;
    }

    template<class O, class A, class B, class C, class D, class E, class F, class G, class H>
    bool call(T* obj, const vector<P>& params, R& r, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(8);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        CW_GET_VALUE(2,C,c);
        CW_GET_VALUE(3,D,d);
        CW_GET_VALUE(4,E,e);
        CW_GET_VALUE(5,F,f);
        CW_GET_VALUE(6,G,g);
        CW_GET_VALUE(7,H,h);
        r = (obj->*callback)( a, b, c, d, e, f, g, h ); return true;
    }

    bool execute(void* o, const vector<P>& params, P& result) {
        if (!callback) return false;
        R res;
        vector<string> defaultParams = splitString(U::str(),'|');
        if (!call<R, Args...>((T*)o, params, res, defaultParams)) return false;
        result = this->convert( res );
        return true;
    }
};

// --- void specialisation ---

template <typename P, typename U, typename T, typename ...Args>
struct VRCallbackWrapperT<P, U, void (T::*)(Args...)> : public VRCallbackWrapper<P> {
    typedef void (T::*Callback)(Args...);
    Callback callback;

    static std::shared_ptr<VRCallbackWrapperT> create() {
        return std::shared_ptr<VRCallbackWrapperT>( new VRCallbackWrapperT() );
    }


    template<class O>
    bool call(T* obj, const vector<P>& params, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(0);
        (obj->*callback)(); return true;
    }

    template<class O, class A>
    bool call(T* obj, const vector<P>& params, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(1);
        CW_GET_VALUE(0,A,a);
        (obj->*callback)(a); return true;
    }

    template<class O, class A, class B>
    bool call(T* obj, const vector<P>& params, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(2);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        (obj->*callback)( a, b ); return true;
    }

    template<class O, class A, class B, class C>
    bool call(T* obj, const vector<P>& params, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(3);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        CW_GET_VALUE(2,C,c);
        (obj->*callback)( a, b, c ); return true;
    }

    template<class O, class A, class B, class C, class D>
    bool call(T* obj, const vector<P>& params, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(4);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        CW_GET_VALUE(2,C,c);
        CW_GET_VALUE(3,D,d);
        (obj->*callback)( a, b, c, d ); return true;
    }

    template<class O, class A, class B, class C, class D, class E>
    bool call(T* obj, const vector<P>& params, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(5);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        CW_GET_VALUE(2,C,c);
        CW_GET_VALUE(3,D,d);
        CW_GET_VALUE(4,E,e);
        (obj->*callback)( a, b, c, d, e ); return true;
    }

    template<class O, class A, class B, class C, class D, class E, class F>
    bool call(T* obj, const vector<P>& params, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(6);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        CW_GET_VALUE(2,C,c);
        CW_GET_VALUE(3,D,d);
        CW_GET_VALUE(4,E,e);
        CW_GET_VALUE(5,F,f);
        (obj->*callback)( a, b, c, d, e, f ); return true;
    }

    template<class O, class A, class B, class C, class D, class E, class F, class G>
    bool call(T* obj, const vector<P>& params, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(7);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        CW_GET_VALUE(2,C,c);
        CW_GET_VALUE(3,D,d);
        CW_GET_VALUE(4,E,e);
        CW_GET_VALUE(5,F,f);
        CW_GET_VALUE(6,G,g);
        (obj->*callback)( a, b, c, d, e, f, g ); return true;
    }

    template<class O, class A, class B, class C, class D, class E, class F, class G, class H>
    bool call(T* obj, const vector<P>& params, const vector<string>& defaultParams) {
        CW_CHECK_SIZE(8);
        CW_GET_VALUE(0,A,a);
        CW_GET_VALUE(1,B,b);
        CW_GET_VALUE(2,C,c);
        CW_GET_VALUE(3,D,d);
        CW_GET_VALUE(4,E,e);
        CW_GET_VALUE(5,F,f);
        CW_GET_VALUE(6,G,g);
        CW_GET_VALUE(7,H,h);
        (obj->*callback)( a, b, c, d, e, f, g, h ); return true;
    }

    bool execute(void* o, const vector<P>& params, P& result) {
        if (!callback) return false;
        vector<string> defaultParams = splitString(U::str(),'|');
        if (!call<void, Args...>((T*)o, params, defaultParams)) return false;
        return true;
    }
};

#endif // VRCALLBACKWRAPPERT_H_INCLUDED
