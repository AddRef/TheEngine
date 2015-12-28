#include "callbacks.hxx"

#include "gtest/gtest.h"


class BroadcasterImpl : protected Broadcaster<BroadcasterImpl::ICallback>
{
public:
    struct ICallback
    {

    };

};

class BroadcastCallbackImpl : protected Broadcaster<BroadcasterImpl::ICallback>::Callback
{

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
    Broadcaster m_broadcaster;
    BroadcastCallback m_broadcast_callback
};

class EmitterImpl : public Emitter<EmitterImpl, EmitterImpl::ICallback>
{
public:
    struct ICallback
    {
        virtual void Callback() = 0;
    };
};

class RecieverImpl : protected EmitterImpl::ICallback
{
public:
    virtual void Callback() override
    {
        std::cout << "Callback" << std::endl;
    }
};

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}