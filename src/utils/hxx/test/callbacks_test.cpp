#include "callbacks.hxx"

#include "gtest/gtest.h"

#include <functional>
#include <sstream>

using TFunction = std::function<void(const std::string& str)>;

class EmitterImpl : public Emitter
{
public:
    struct ICallback0
    {
        virtual void MyCallback0(TFunction function) = 0;
    };

    struct ICallback1
    {
        virtual void MyCallback1(TFunction function) = 0;
    };

   EmitterImpl::EmitterImpl()
   {
       Emitter::Init<ICallback0>();
       Emitter::Init<ICallback1>();
   }
};

class Receiver0Impl : public Receiver<EmitterImpl::ICallback0>
{
public:
    Receiver0Impl(const char* str)
    {
        m_string = str ? str : "";
    }
    virtual void MyCallback0(TFunction function) override
    {
        if (function)
            function(m_string);
    }
private:
    std::string m_string;
};

class Receiver1Impl : public Receiver<EmitterImpl::ICallback1>
{
public:
    Receiver1Impl(const char* str)
    {
        m_string = str ? str : "";
    }
    virtual void MyCallback1(TFunction function) override
    {
        if (function)
            function(m_string);
    }
private:
    std::string m_string;
};

class CallbacksTest : public ::testing::Test
{
public:
    CallbacksTest()
    {

    }

    ~CallbacksTest()
    {
        
    }

    void SetUp() 
    {
    }

    void TearDown() 
    {
    }
private:
};

TEST_F(CallbacksTest, CallbacksTest_MultipleEqualCallbacks)
{
    std::vector<std::string> stream;
    EmitterImpl emitter;
    auto function = [&stream](const std::string& str) { stream.push_back(str); };
    Receiver0Impl receiver0("receiver0");
    Receiver0Impl receiver1("receiver1");
    emitter.AddCallback(receiver0);
    emitter.AddCallback(receiver1);
    auto callbacks0 = emitter.GetCallback<EmitterImpl::ICallback0>();
    auto callbacks1 = emitter.GetCallback<EmitterImpl::ICallback1>();
    EXPECT_EQ(callbacks0.size(), 2);
    EXPECT_EQ(callbacks1.size(), 0);
    for (EmitterImpl::ICallback0* callback : callbacks0)
    {
        callback->MyCallback0(function);
    }
    EXPECT_EQ(stream.size(), 2);
    EXPECT_EQ(stream[0], "receiver0");
    EXPECT_EQ(stream[1], "receiver1");
}

TEST_F(CallbacksTest, CallbacksTest_DeadCallback)
{
    std::vector<std::string> stream;
    EmitterImpl emitter;
    auto function = [&stream](const std::string& str) { stream.push_back(str); };
    Receiver0Impl receiver0("receiver0");
    {
        Receiver0Impl receiver1("receiver1");
        emitter.AddCallback(receiver0);
        emitter.AddCallback(receiver1);
    }
    auto callbacks0 = emitter.GetCallback<EmitterImpl::ICallback0>();
    auto callbacks1 = emitter.GetCallback<EmitterImpl::ICallback1>();
    EXPECT_EQ(callbacks0.size(), 1);
    EXPECT_EQ(callbacks1.size(), 0);
    for (EmitterImpl::ICallback0* callback : callbacks0)
    {
        callback->MyCallback0(function);
    }
    EXPECT_EQ(stream.size(), 1);
    EXPECT_EQ(stream[0], "receiver0");
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}