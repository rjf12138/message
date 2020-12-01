#include "msg_record.h"

using namespace my_utils;

int main(void)
{
    for (int i = 0; i < 10; ++i) {
        LOG_GLOBAL_TRACE("Hello, world! - %d", i);
    }

    for (int i = 0; i < 10; ++i) {
        LOG_GLOBAL_DEBUG("Hello, world! - %d", i);
    }

    for (int i = 0; i < 10; ++i) {
        LOG_GLOBAL_INFO("Hello, world! - %d", i);
    }

    for (int i = 0; i < 10; ++i) {
        LOG_GLOBAL_WARN("Hello, world! - %d", i);
    }

    for (int i = 0; i < 10; ++i) {
        LOG_GLOBAL_ERROR("Hello, world! - %d", i);
    }

    for (int i = 0; i < 10; ++i) {
        LOG_GLOBAL_FATAL("Hello, world! - %d", i);
    }

    return 0;
}