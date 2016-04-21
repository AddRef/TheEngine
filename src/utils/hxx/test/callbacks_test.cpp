#include "callbacks.hxx"

#include "gtest/gtest.h"

#include <functional>
#include <sstream>
#include <cstdint>

using TFunction = std::function<void(const std::string& str)>;

class DestructionTracker
{
public:
    DestructionTracker(uint32_t& destruction_counter)
        : m_destruction_counter(&destruction_counter)
    {
        ++(*m_destruction_counter);
    }
    virtual ~DestructionTracker()
    {
        --(*m_destruction_counter);
    }
private:
    uint32_t* m_destruction_counter;
};

class EmitterImpl
    : public Emitter
    , protected DestructionTracker
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

    struct ICallback2
    {
        virtual void MyInvalidCallback2() = 0; // Callback that will be used to trigger exceptions
    };

    EmitterImpl(uint32_t& destruction_counter) : DestructionTracker(destruction_counter)
    {
        Emitter::Init<ICallback0>();
        Emitter::Init<ICallback1>();
        // We intentionally don't register ICCallback2 to use it for exceptions validation
    }

    template <typename TCallback>
    std::list<TCallback*> GetCallback() const
    {
        return Emitter::GetCallback<TCallback>();
    }
};

class Receiver0Impl 
    : public Receiver<EmitterImpl::ICallback0>
    , protected DestructionTracker
{
public:
    Receiver0Impl(const char* str, uint32_t& destruction_counter) : DestructionTracker(destruction_counter)
    {
        m_string = str ? str : "";
    }
protected:
    virtual void MyCallback0(TFunction function) override
    {
        if (function)
            function(m_string);
    }
private:
    std::string m_string;
};

class Receiver1Impl 
    : public Receiver<EmitterImpl::ICallback1>
    , protected DestructionTracker
{
public:
    Receiver1Impl(const char* str, uint32_t& destruction_counter) : DestructionTracker(destruction_counter)
    {
        m_string = str ? str : "";
    }
protected:
    virtual void MyCallback1(TFunction function) override
    {
        if (function)
            function(m_string);
    }
private:
    std::string m_string;
};

class Receiver2Impl 
    : public Receiver<EmitterImpl::ICallback2>
    , protected DestructionTracker
{
public:
    Receiver2Impl(uint32_t& destruction_counter) : DestructionTracker(destruction_counter)
    {

    }
protected:
    virtual void MyInvalidCallback2() override
    {
        return;
    }
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
        m_destruction_counter = 0;
    }

    void TearDown() 
    {
        ASSERT_EQ(m_destruction_counter, 0);
    }
protected:
    uint32_t m_destruction_counter = 0;
};

TEST_F(CallbacksTest, CallbacksTest_MultipleEqualCallbacks)
{
    EmitterImpl emitter(m_destruction_counter);
    std::vector<std::string> stream;
    auto function = [&stream](const std::string& str) { stream.push_back(str); };
    Receiver0Impl receiver0("receiver0", m_destruction_counter);
    Receiver0Impl receiver1("receiver1", m_destruction_counter);
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
    EmitterImpl emitter(m_destruction_counter);
    std::vector<std::string> stream;
    auto function = [&stream](const std::string& str) { stream.push_back(str); };
    Receiver0Impl receiver0("receiver0", m_destruction_counter);
    {
        Receiver0Impl receiver1("receiver1", m_destruction_counter);
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

TEST_F(CallbacksTest, CallbackTest_RemovingCallbacks)
{
    std::vector<std::string> stream;
    auto function = [&stream](const std::string& str) { stream.push_back(str); };

    EmitterImpl emitter(m_destruction_counter);
    Receiver0Impl receiver0("receiver0", m_destruction_counter);
    Receiver0Impl receiver1("receiver1", m_destruction_counter);

    emitter.AddCallback(receiver0);
    emitter.AddCallback(receiver1);
    for (EmitterImpl::ICallback0* callback : emitter.GetCallback<EmitterImpl::ICallback0>())
    {
        callback->MyCallback0(function);
    }
    EXPECT_EQ(stream.size(), 2);
    EXPECT_EQ(stream[0], "receiver0");
    EXPECT_EQ(stream[1], "receiver1");
    stream.clear();
    emitter.RemoveCallback(receiver0);
    for (EmitterImpl::ICallback0* callback : emitter.GetCallback<EmitterImpl::ICallback0>())
    {
        callback->MyCallback0(function);
    }
    EXPECT_EQ(stream.size(), 1);
    EXPECT_EQ(stream[0], "receiver1");
}

TEST_F(CallbacksTest, CallbacksTest_Exceptions)
{
    EmitterImpl emitter(m_destruction_counter);
    Receiver2Impl invalid_callback(m_destruction_counter);
    EXPECT_THROW(emitter.AddCallback(invalid_callback), std::runtime_error);
    EXPECT_THROW(emitter.RemoveCallback(invalid_callback), std::runtime_error);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}