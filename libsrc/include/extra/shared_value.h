#ifndef COMMAND_PARSER__EXTRA__SHARED_VALUE_H__
#define COMMAND_PARSER__EXTRA__SHARED_VALUE_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct shared_value_
{
    size_t value_mem_size_;
    int64_t* counter_;
    void* value_;
} shared_value_s;

bool shared_value_init(shared_value_s* value, size_t mem_size);
bool shared_value_init_unused(shared_value_s* value, size_t mem_size);
void shared_value_clean(shared_value_s* value);
void shared_value_clean_ex(shared_value_s* value, void (*clean_func)(void*));
int64_t shared_value_use_count(const shared_value_s* value);

bool shared_value_copy_into(shared_value_s* restrict dest, shared_value_s* restrict src);
bool shared_value_copy_directly(shared_value_s* value);

void* shared_value_put(shared_value_s* value, void* data);
void* shared_value_read(shared_value_s* value);
const void* shared_value_read_const(const shared_value_s* value);

#endif // !COMMAND_PARSER__EXTRA__SHARED_VALUE_H__
