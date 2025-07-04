#include "extra/shared_value.h"

#include <stdlib.h>
#include <string.h>

// LOCAL DEFINITIONS //

#define IS_COUNTER_SAFE(counter_ptr) \
    ((counter_ptr) != NULL && (*counter_ptr) != NULL)

bool shared_value_init_empty_(shared_value_s* value, size_t mem_size);

// END LOCAL DEFINITIONS //

bool shared_value_init(shared_value_s* value, size_t mem_size)
{
    if (!shared_value_init_unused(value, mem_size))
        return false;

    **value->counter_ = 1;
    return true;
}

bool shared_value_init_unused(shared_value_s* value, size_t mem_size)
{
    if (value == NULL || mem_size == 0)
        return false;

    if (!shared_value_init_empty_(value, mem_size))
        return false;

    *value->counter_ = calloc(1, sizeof(int64_t));
    if (*value->counter_ == NULL)
    {
        free(value->counter_);
        return false;
    }

    value->value_ = calloc(1, mem_size);
    if (value->value_ == NULL)
    {
        free(*value->counter_);
        free(value->counter_);
        return false;
    }

    value->value_mem_size_ = mem_size;
    return true;
}

void shared_value_clean(shared_value_s* value)
{
    if (value == NULL || !IS_COUNTER_SAFE(value->counter_))
        return;

    (**value->counter_)--;
    if (**value->counter_ > 0)
    {
        free(value->counter_);
        return;
    }

    free(*value->counter_);
    *value->counter_ = NULL;
    free(value->counter_);
    free(value->value_);
}

int64_t shared_value_use_count(const shared_value_s* value)
{
    if (value == NULL || !IS_COUNTER_SAFE(value->counter_))
        return 0;

    return **value->counter_;
}

bool shared_value_copy_into(shared_value_s* restrict dest, shared_value_s* restrict src)
{
    if (dest == NULL || src == NULL || !IS_COUNTER_SAFE(src->counter_))
        return false;

    if (!shared_value_init_empty_(dest, src->value_mem_size_))
        return false;

    *dest->counter_ = *src->counter_;
    dest->value_ = src->value_;
    dest->value_mem_size_ = src->value_mem_size_;
    return shared_value_copy_directly(src);
}

bool shared_value_copy_directly(shared_value_s* value)
{
    if (value == NULL || !IS_COUNTER_SAFE(value->counter_))
        return false;

    (**value->counter_)++;
    return true;
}

void* shared_value_put(shared_value_s* value, void* data)
{
    if (value == NULL || data == NULL || !IS_COUNTER_SAFE(value->counter_))
        return NULL;

    return memcpy(value->value_, data, value->value_mem_size_);
}

void* shared_value_read(shared_value_s* value)
{
    if (value == NULL || !IS_COUNTER_SAFE(value->counter_))
        return NULL;

    return value->value_;
}

const void* shared_value_read_const(const shared_value_s* value)
{
    if (value == NULL || !IS_COUNTER_SAFE(value->counter_))
        return NULL;

    return value->value_;
}

// LOCAL IMPLEMENTATIONS //

bool shared_value_init_empty_(shared_value_s* value, size_t mem_size)
{
    if (value == NULL || mem_size == 0)
        return false;

    value->counter_ = malloc(sizeof(int64_t*));
    return value->counter_ != NULL;
}

// END LOCAL IMPLEMENTATIONS //