// Project Dependencies
#include <jnipp.h>

// Standard Dependencies
#include <cmath>

// Local Dependencies
#include "testing.h"

TEST(Vm_detectsJreInstall)
{
    try
    {
        jni::Vm vm;
    }
    catch (jni::InitializationException&)
    {
        ASSERT(0);
        return;
    }

    ASSERT(1);
}

TEST(Vm_notAllowedMultipleVms)
{
    try
    {
        jni::Vm firstVm;
        jni::Vm secondVm;    // Throws an exception.
    }
    catch (jni::InitializationException&)
    {
        ASSERT(1);
        return;
    }

    ASSERT(0);
}

TEST(Class_findByName)
{
    jni::Class cls("java/lang/String");

    ASSERT(!cls.isNull());
}

TEST(Class_getName)
{
    jni::Class cls("java/lang/String");

    ASSERT(cls.getName() == "java.lang.String");
}

TEST(Class_getParent)
{
    jni::Class parent = jni::Class("java/lang/Integer").getParent();

    ASSERT(parent.getName() == "java.lang.Number");
}

TEST(Class_newInstance)
{
    jni::Class Integer("java/lang/Integer");
    jni::method_t constructor = Integer.getConstructor("(Ljava/lang/String;)V");
    jni::Object i = Integer.newInstance(constructor, "123");
    jni::Object str = jni::Class("java/lang/String").newInstance();

    ASSERT(!i.isNull());
    ASSERT(!str.isNull());
}

TEST(Class_newInstance_withArgs)
{
    jni::Object str1 = jni::Class("java/lang/String").newInstance("Testing...");
    jni::Object str2 = jni::Class("java/lang/String").newInstance(L"Testing...");

    ASSERT(!str1.isNull());
    ASSERT(!str2.isNull());
}

TEST(Class_getStaticField)
{
    jni::field_t field = jni::Class("java/lang/Integer").getStaticField("MAX_VALUE", "I");

    ASSERT(field);
}

TEST(Class_getMethod)
{
    jni::method_t method1 = jni::Class("java/lang/Integer").getMethod("intValue", "()I");
    jni::method_t method2 = jni::Class("java/lang/Integer").getMethod("intValue()I");

    ASSERT(method1);
    ASSERT(method2);
}

TEST(Class_getStaticMethod)
{
    jni::method_t method = jni::Class("java/lang/Integer").getStaticMethod("compare", "(II)I");

    ASSERT(method);
}

TEST(Class_get_staticField)
{
    jni::Class Integer("java/lang/Integer");
    jni::field_t field = Integer.getStaticField("SIZE", "I");

    ASSERT(Integer.get<int>(field) == 32);
}

TEST(Class_get_staticField_byName)
{
    jni::Class Integer("java/lang/Integer");
    jni::Class Character("java/lang/Character");
    jni::Class Short("java/lang/Short");
    jni::Class Long("java/lang/Long");
    jni::Class Float("java/lang/Float");
    jni::Class Double("java/lang/Double");

    ASSERT(Short.get<short>("MAX_VALUE") == short(0x7FFF));
    ASSERT(Character.get<wchar_t>("MAX_VALUE") == L'\xFFFF')
    ASSERT(Integer.get<int>("MAX_VALUE") == int(0x7FFFFFFF));
    ASSERT(Long.get<long long>("MAX_VALUE") == (long long) (0x7FFFFFFFFFFFFFFF));
    ASSERT(std::isnan(Float.get<float>("NaN")));
    ASSERT(std::isnan(Double.get<double>("NaN")));
}

TEST(Class_getConstructor)
{
    jni::Class Integer("java/lang/Integer");
    jni::method_t constructor = Integer.getConstructor("(Ljava/lang/String;)V");

    ASSERT(constructor);
}

TEST(Class_call_staticMethod)
{
    jni::Class Integer("java/lang/Integer");
    jni::method_t method = Integer.getStaticMethod("parseInt", "(Ljava/lang/String;)I");

    int i = Integer.call<int>(method, "1000");

    ASSERT(i == 1000);
}

TEST(Class_call_staticMethod_byName)
{
    int       i = jni::Class("java/lang/Integer").call<int>("parseInt", "1000");
    bool      b = jni::Class("java/lang/Boolean").call<bool>("parseBoolean", "true");
    wchar_t   c = jni::Class("java/lang/Character").call<wchar_t>("toLowerCase", L'X');
    short     s = jni::Class("java/lang/Short").call<short>("parseShort", "1000");
    long long l = jni::Class("java/lang/Long").call<long long>("parseLong", "1000");
    float     f = jni::Class("java/lang/Float").call<float>("parseFloat", "123.0");
    double    d = jni::Class("java/lang/Double").call<double>("parseDouble", "123.0");

    ASSERT(i == 1000);
    ASSERT(b == true);
    ASSERT(c == L'x');
    ASSERT(s == 1000);
    ASSERT(l == 1000);
    ASSERT(f == 123.0);    // Warning: floating point comparison.
    ASSERT(d == 123.0);    // Warning: floating point comparison.
}

TEST(Object_defaultConstructor_isNull)
{
    jni::Object o;

    ASSERT(o.isNull());
}

TEST(Object_copyConstructorIsSameObject)
{
    jni::Object a = jni::Class("java/lang/String").newInstance();
    jni::Object b = a;

    ASSERT(a == b);
}

TEST(Object_moveConstructor)
{
    jni::Object a = jni::Class("java/lang/String").newInstance();
    jni::Object b = std::move(a);

    ASSERT(a.isNull());
    ASSERT(!b.isNull());
}

TEST(Object_copyAssignmentOperator)
{
    jni::Object a = jni::Class("java/lang/String").newInstance();
    jni::Object b = jni::Class("java/lang/Integer").newInstance(0);

    a = b;

    ASSERT(a == b);
}

TEST(Object_moveAssignmentOperator)
{
    jni::Object a = jni::Class("java/lang/String").newInstance();
    jni::Object b = jni::Class("java/lang/Integer").newInstance(0);

    a = std::move(b);

    ASSERT(!a.isNull());
    ASSERT(b.isNull());
}

TEST(Object_call)
{
    jni::Class Integer("java/lang/Integer");
    jni::method_t intValue = Integer.getMethod("intValue", "()I");
    jni::Object i = Integer.newInstance(100);

    ASSERT(i.call<int>(intValue) == 100);
}

TEST(Object_call_byName)
{
    jni::Object i = jni::Class("java/lang/Integer").newInstance(100);
    jni::Object b = jni::Class("java/lang/Boolean").newInstance(true);
    jni::Object s = jni::Class("java/lang/Short").newInstance(short(100));
    jni::Object l = jni::Class("java/lang/Long").newInstance(100LL);
    jni::Object f = jni::Class("java/lang/Float").newInstance(100.0f);
    jni::Object d = jni::Class("java/lang/Double").newInstance(100.0);

    ASSERT(i.call<int>("intValue") == 100);
    ASSERT(s.call<short>("shortValue") == 100);
    ASSERT(b.call<bool>("booleanValue") == true);
    ASSERT(l.call<long long>("longValue") == 100LL);
    ASSERT(f.call<float>("floatValue") == 100.0f);    // Warning: Floating point comparison.
    ASSERT(d.call<double>("doubleValue") == 100.0);    // Warning: Floating point comparison.
    ASSERT(i.call<std::wstring>("toString") == L"100");
    ASSERT(i.call<std::string>("toString") == "100");
}

TEST(Object_call_withArgs)
{
    jni::Class String("java/lang/String");
    jni::method_t charAt = String.getMethod("charAt", "(I)C");
    jni::Object str = String.newInstance("Testing");

    ASSERT(str.call<wchar_t>(charAt, 1) == L'e');
}

TEST(Object_call_byNameWithArgs)
{
    jni::Object str = jni::Class("java/lang/String").newInstance("Testing");
    jni::Object str2 = jni::Class("java/lang/String").newInstance(L"Testing");

    ASSERT(str.call<wchar_t>("charAt", 1) == L'e');
    ASSERT(str2.call<wchar_t>("charAt", 1) == L'e');
}

TEST(Enum_get)
{
    jni::Class Thread("java/lang/Thread");
    jni::Enum State("java/lang/Thread$State");
    jni::method_t currentThread = Thread.getStaticMethod("currentThread", "()Ljava/lang/Thread;");
    jni::method_t getState = Thread.getMethod("getState", "()Ljava/lang/Thread$State;");

    jni::Object thread = Thread.call<jni::Object>(currentThread);
    jni::Object state = thread.call<jni::Object>(getState);

    ASSERT(state == State.get("RUNNABLE"));
}

TEST(Arg_bool)
{
    std::string str1 = jni::Class("java/lang/String").call<std::string>("valueOf", true);
    std::wstring str2 = jni::Class("java/lang/String").call<std::wstring>("valueOf", true);

    ASSERT(str1 == "true");
    ASSERT(str2 == L"true");
}

TEST(Arg_wchar)
{
    std::string str1 = jni::Class("java/lang/String").call<std::string>("valueOf", L'X');
    std::wstring str2 = jni::Class("java/lang/String").call<std::wstring>("valueOf", L'X');

    ASSERT(str1 == "X");
    ASSERT(str2 == L"X");
}

TEST(Arg_double)
{
    std::string str1 = jni::Class("java/lang/String").call<std::string>("valueOf", 123.0);
    std::wstring str2 = jni::Class("java/lang/String").call<std::wstring>("valueOf", 123.0);

    ASSERT(str1 == "123.0");
    ASSERT(str2 == L"123.0");
}

TEST(Arg_float)
{
    std::string str1 = jni::Class("java/lang/String").call<std::string>("valueOf", 123.0f);
    std::wstring str2 = jni::Class("java/lang/String").call<std::wstring>("valueOf", 123.0f);

    ASSERT(str1 == "123.0");
    ASSERT(str2 == L"123.0");
}

TEST(Arg_int)
{
    std::string str1 = jni::Class("java/lang/String").call<std::string>("valueOf", 123);
    std::wstring str2 = jni::Class("java/lang/String").call<std::wstring>("valueOf", 123);

    ASSERT(str1 == "123");
    ASSERT(str2 == L"123");
}

TEST(Arg_longLong)
{
    std::string str1 = jni::Class("java/lang/String").call<std::string>("valueOf", 123LL);
    std::wstring str2 = jni::Class("java/lang/String").call<std::wstring>("valueOf", 123LL);

    ASSERT(str1 == "123");
    ASSERT(str2 == L"123");
}

TEST(Arg_Object)
{
    jni::Object str1 = jni::Class("java/lang/String").newInstance("123");
    int i = jni::Class("java/lang/Integer").call<int>("parseInt", str1);

    ASSERT(i == 123);
}

TEST(Arg_ObjectPtr)
{
    jni::Object str1 = jni::Class("java/lang/String").newInstance("123");
    int i = jni::Class("java/lang/Integer").call<int>("parseInt", &str1);

    ASSERT(i == 123);
}

int main()
{
    // jni::Vm Tests
    RUN_TEST(Vm_detectsJreInstall);
    RUN_TEST(Vm_notAllowedMultipleVms);

    {
        jni::Vm vm;

        // jni::Class Tests
        RUN_TEST(Class_findByName);
        RUN_TEST(Class_getName);
        RUN_TEST(Class_getParent);
        RUN_TEST(Class_newInstance);
        RUN_TEST(Class_newInstance_withArgs);
        RUN_TEST(Class_getStaticField);
        RUN_TEST(Class_getMethod);
        RUN_TEST(Class_getStaticMethod);
        RUN_TEST(Class_get_staticField);
        RUN_TEST(Class_get_staticField_byName);
        RUN_TEST(Class_call_staticMethod_byName);
        RUN_TEST(Class_getConstructor);

        // jni::Object Tests
        RUN_TEST(Object_defaultConstructor_isNull);
        RUN_TEST(Object_copyConstructorIsSameObject);
        RUN_TEST(Object_moveConstructor);
        RUN_TEST(Object_copyAssignmentOperator);
        RUN_TEST(Object_moveAssignmentOperator);
        RUN_TEST(Object_call);
        RUN_TEST(Object_call_byName);
        RUN_TEST(Object_call_withArgs);
        RUN_TEST(Object_call_byNameWithArgs);

        // jni::Enum Tests
        RUN_TEST(Enum_get);

        // Argument Type Tests
        RUN_TEST(Arg_bool);
        RUN_TEST(Arg_wchar);
        RUN_TEST(Arg_double);
        RUN_TEST(Arg_float);
        RUN_TEST(Arg_int);
        RUN_TEST(Arg_longLong);
        RUN_TEST(Arg_Object);
        RUN_TEST(Arg_ObjectPtr);
    }

    std::cout << "Press a key to continue..." << std::endl;
    std::cin.get();
    return 0;
}

